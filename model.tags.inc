<?php



define ('TAGS_PER_PAGE', get_module_ini('tags_per_page'));

class tags {

    public static $errors = array();
    public static $tagsTable = 'tags';
    public static $tagsReferenceTable = 'tags_reference';

    /**
     *
     * @param string $name name of the input
     * @param string $value value of the input. 
     * @return string   the tag widget
     */
    public static function defaultWidget ($name, $value, $options = array()){
        template::setInlineCss(_COS_PATH . "/modules/tags/tags.css");
        template::setInlineJs(_COS_PATH . "/modules/tags/tags.js");
        if (!isset($options['size'])){
            $options['size'] = HTML_FORM_TEXT_SIZE;
        }
        
        //$value = self::setValue($name, $value);
        $options = html::parseExtra($options);

        $str = <<<EOD
	<input type="text" name="$name" id="tags" $options value="$value" /><br />
EOD;
     return $str;
    }

    /**
     * method for getting a tag id
     * used only when on admin page.
     * @return string   uri fragment
     */
    public static function getEntryId (){
        return uri::$fragments[2];
    }

    /**
     * method for add a tag to database table
     * @return boolean  database result from insert
     */
    public static function add ($values = null){
        $db = new db();
        if (!$values) {
            $values = db::prepareToPost($_POST);
        }
        $res = $db->insert(self::$tagsTable, $values);
        return $res;
    }

    public static function getTagSingle ($id){
        $db = new db();
        $row = $db->selectOne(self::$tagsTable, 'id', $id);
        return $row;
    }

    public static function getTagSingleFromTitle ($title){
        $db = new db();
        $row = $db->selectOne(self::$tagsTable, 'title', $title);
        return $row;
    }

    /**
     * method for adding a reference to database.
     * @param string $tags (e.g. "Misc, Drupal, Another Tag, )
     * @param string $reference (e.g. 'blog'
     * @param int    $id (e.g. 27, a unique id for blog a blog entry)
     */
    public static function addReference($tags, $reference, $id) {
        $tags_ary = self::parse($tags);

        $db = new db();
        $values = array();
        $values['reference_name'] = $reference;
        $values['reference_id'] = $id;

        foreach ($tags_ary as $key => $val){
            $values['tags_id'] = $val;
            $db->insert(self::$tagsReferenceTable, $values);
        }
    }

    
    public static function updateReference ($tags, $reference, $id){
        self::deleteReference($reference, $id);
        self::addReference($tags, $reference, $id);
    }

    public static function deleteReference ($reference, $id) {
        $db = new db();
        $search = array ('reference_id' => $id, 'reference_name' => $reference);
        return $db->delete(self::$tagsReferenceTable, null, $search);
    }
    /**
     * returns tag references as a string
     * @param   string $reference (e.g. 'blog')
     * @param   int    $id (e.g. blog_id
     * @return  string $tags_str (tags as a string)
     */
    public static function getReferenceAsString($reference, $id) {
        $db = new db();
        $tags = self::getReferenceAsArray($reference, $id);

        $tags_str = '';
        foreach ($tags as $key => $val){
            $tag = $db->selectOne(self::$tagsTable, 'id', $val['tags_id']);
            $tags_str.= $tag['title'] . ", ";
        }
        return $tags_str;
    }

    public static function getReferenceAsArray ($reference, $id) {
        $db = new db();
        $search = array ('reference_name' => $reference, 'reference_id' => $id);
        $references = $db->selectAll(self::$tagsReferenceTable, null, $search);
        return $references;
    }

    public static function getReferenceAsTags ($reference, $id){
        $db = new db();
        $references = self::getReferenceAsArray($reference, $id);
        $tags = array();
        foreach ($references as $key => $val){
            $tags[] = $db->selectOne(self::$tagsTable, 'id', $val['tags_id']);
        }
        return $tags;
    }

    /**
     * returns a html string with all tags
     * conected to reference, id as links
     * 
     * @param   string $reference (e.g. 'blog')
     * @param   int    $id (the unique id of the entry e.g. 23)
     * @param   string $tag_page (path to base which will handle the tag.
     * @return  string $tag_str (a html string with all tags as links)
     */
    public static function getTagReferenceAsHTML ($reference, $id, $tag_page = ''){
        $tags = self::getReferenceAsTags($reference, $id);
        $str = '';
        $num_tags = count($tags);

        foreach ($tags as $key => $val){
            $url = $tag_page . "/$val[id]/" . cos_sanitize_url($val['title']);
            $extra = array ('title' => $val['description']);
            $str.=html::createLink($url, $val['title'], $extra);
            $num_tags--;
            if ($num_tags)$str.=MENU_SUB_SEPARATOR;
        }
        return $str;
    }
    
    public static function prepare ($action = 'insert'){
        if (empty($_POST['title'])){
            self::$errors['title'] = lang::translate('tags_error_no_title');
        }

        $row = self::getTagSingleFromTitle($_POST['title']);
        if (!empty($row)){
            if ($action == 'insert') {
                self::$errors['title'] = 'tags_error_exists';
            } else if ($action == 'update'){
                $id = self::getEntryId();
                if ($id  != $row['id']) {
                    self::$errors['title'] = 'tags_error_exists';
                }
            }
        }

        $_POST['title'] = cos_sanitize_url($_POST['title']);
        $_POST['description'] = cos_htmlentities($_POST['description']);
    }

    public static function getNumTags (){
        $db = new db();
        return $db->getNumRows(self::$tagsTable);
    }
    


    public static function addController (){
        if (isset($_POST['submit'])){
            self::prepare();
            if (!empty(self::$errors)){
                view_form_errors(self::$errors);
            } else {
                $res = self::add();
                if ($res){
                    if ($_POST['submit'] == lang::translate('tags_submit_add_another')){
                        $redirect = "/tags/add";
                    } else {
                        $redirect = "/tags/index";
                    }
                    session::setActionMessage(
                        lang::translate('tags_added_tag_action_message'));
                    header("Location: $redirect");
                }
            }
        }       
        include_view('tags', 'add');        
    }

    public static function getAllReferenceTag ($reference, $tag_id, $from = 0, $limit = 10){
        $db = new db();
        $sql = "SELECT reference_id FROM tags_reference WHERE ";
        $sql.= "`reference_name` = " . db::$dbh->quote($reference) . " AND ";
        $sql.= "`tags_id` = " . db::$dbh->quote($tag_id) ." ";
        $sql.= "LIMIT " . $from . " , " .  $limit;
        $rows = $db->selectQuery($sql);

        $ary = array();
        foreach($rows as $key => $val) {
            $ary[] = $val['reference_id'];
        }
        return $ary;
    }

    public static function getAllReferenceTagWithoutReference ($tag_id, $from = 0, $limit = 10){
        $db = new db();
        $sql = "SELECT * FROM tags_reference WHERE ";
        $sql.= "`tags_id` = " . db::$dbh->quote($tag_id) ." ";
        $sql.= "LIMIT " . $from . " , " .  $limit;
        $rows = $db->selectQuery($sql);
        return $rows;
    }

    public static function getAllReferenceTagNumRows($reference, $tag_id){
        $db = new db();
        $search = array ('reference_name' => $reference, 'tags_id' => $tag_id);
        return $db->getNumRows(self::$tagsReferenceTable, $search);

    }

    public static function indexController (){
        include_once "pearPager.php";

        $num_tags = self::getNumTags();
        $pager = new pearPager($num_tags, TAGS_PER_PAGE);

        $db = new db();
        $rows = $db->selectAll(self::$tagsTable, null, null, $pager->from, TAGS_PER_PAGE, 'title');
        include_view('tags', 'view', $rows);
        $pager->pearPage();
    }

    public static function updateController (){     
        if (isset($_POST['submit'])){
            self::prepare('update');
            if (!empty(self::$errors)){
                view_form_errors(self::$errors);
            } else {
                $res = self::update();
                if ($res){           
                    session::setActionMessage(
                        lang::translate('tags_updated_tag_action_message'));
                    header("Location: /tags/index");
                }
            }
        }       
        $db = new db();
        $row = $db->selectOne(self::$tagsTable, 'id', self::getEntryId());
        include_view('tags', 'edit', html::entitiesEncode($row));
    }

    public static function deleteController (){
        if (isset($_POST['submit'])){
            $res = self::delete();
            if ($res){
                session::setActionMessage(
                    lang::translate('tags_deleted_tag_action_message'));
                header("Location: /tags/index");
            }
        }
        include_view('tags', 'delete');
    }

    public static function parse ($tags){
        $db = new db();
        $ary = explode(',', $tags);
        foreach ($ary as $key => $val) {
            $ary[$key] = trim($val);
        }
        $ary = array_unique($ary);
        foreach($ary as $key => $val){
            $row = $db->selectOne(self::$tagsTable, 'title', $val);
            if ($row){
                $ary[$key] = $row['id'];
            } else {
                if (get_module_ini('tags_auto_add')){
                    $tag = array ();
                    $tag['title'] = $val;
                    $tag['title'] = cos_sanitize_url($tag['title']);
                    error_log(var_dump($tag));
                    self::add($tag);
                    $ary[$key] = db::$dbh->lastInsertId();
                } else {
                    unset($ary[$key]);
                }
            }
        }     
        return $ary;
    }

    public static function delete (){
        $db = new db();
        return $db->delete(self::$tagsTable, 'id', self::getEntryId());

    }

    public static function update (){
        $db = new db();
        $values = db::prepareToPost();
        return $db->update(self::$tagsTable, $values, self::getEntryId());
    }

    public static function getUserTags (){

    }

    public static function getAllTags (){

    }

    public static function viewAdminLinks (&$val){
        echo html::createLink("/tags/edit/$val[id]", lang::translate('tags_admin_edit'));
        echo MENU_SUB_SEPARATOR;
        echo html::createLink("/tags/delete/$val[id]", lang::translate('tags_admin_delete'));
    }

    
}