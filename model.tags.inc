<?php

/**
 * File containing main model class for module tags
 * @package tags
 */

/**
 * This is the model file of the module tags.
 * It provides user interface for adding tags, and also some options for
 * including the tag module both as a event system and as submodule.  
 * @package tags
 */

/**
 * Tags per page
 */
define ('TAGS_PER_PAGE', config::getModuleIni('tags_per_page'));

/**
 * class tags 
 * @package tags
 */
class tags {

    /**
     * var for holding errors when adding tags with form
     * @var array $errors
     */
    public static $errors = array();
    
    /**
     * name of tags db table
     * @var string $tagsTable
     */
    public static $tagsTable = 'tags';
    
    /**
     * name of tagsReferenceTable
     * @var string $tagsReferenceTable
     */
    public static $tagsReferenceTable = 'tags_reference';

    /**
     *
     * @param string $name name of the input
     * @param string $value value of the input. 
     * @return string  $str the tag widget
     */
    public static function defaultWidget ($name, $value, $options = array()){
        self::initJs();
        if (!isset($options['size'])){
            $options['size'] = HTML_FORM_TEXT_SIZE;
        }
       
        $options = html::parseExtra($options); 
        $str = <<<EOD
	<input type="text" name="$name" id="tags" $options value="$value" />
        <br />
EOD;
        return $str;
    }
    
    /**
     * method for loading css and javascript
     */
    public static function initJs () {
        template::setInlineCss(config::getModulePath('tags') . "/tags.css");
        template::setInlineJs(config::getModulePath('tags') . "/tags.js");
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
     * @return boolean $res database result from insert
     */
    public static function add ($values = null){
        $db = new db();
        if (!$values) {
            $values = db::prepareToPost($_POST);
            if (isset($_POST['is_main'])){
                $values['is_main'] = 1;
            }
        }
        
        // create a clean tag. 
        $values['title'] = strings::sanitizeUrlSimple($values['title']);
        if (empty($values['title'])) return false;
        $res = $db->insert(self::$tagsTable, $values);
        return $res;
    }

    /**
     * get a single tag
     * @param int $id
     * @return array $row
     */
    public static function getTagSingle ($id){
        $db = new db();
        $row = $db->selectOne(self::$tagsTable, 'id', $id);
        return $row;
    }

    /**
     * get a tag from title
     * @param string $title
     * @return array $row
     */
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

    /**
     * updates a reference
     * @param array $tags
     * @param string $reference
     * @param string $id 
     */
    public static function updateReference ($tags, $reference, $id){
        self::deleteReference($reference, $id);
        self::addReference($tags, $reference, $id);
    }

    /**
     * deletes references to a tag
     * @param string $reference
     * @param int $id
     * @return boolean $res database result from delete operation 
     */
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

    /**
     * gets all references from reference name and id
     * @param string $reference
     * @param int $id
     * @return array $references  
     */
    public static function getReferenceAsArray ($reference, $id) {
        $db = new db();
        $search = array ('reference_name' => $reference, 'reference_id' => $id);
        $references = $db->selectAll(self::$tagsReferenceTable, null, $search);
        return $references;
    }

    /**
     * get tags from references in database
     * @param type $reference
     * @param type $id
     * @return type 
     */
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
            $url = strings::utf8Slug($tag_page . "/$val[id]" , $val['title']);
            $extra = array ('title' => $val['description']);
            
            $str.=html::createLink(html::specialEncode($url), $val['title'], $extra);
            $num_tags--;
            if ($num_tags)$str.=MENU_SUB_SEPARATOR;
        }
        return $str;
    }
    
    public static function prepare ($action = 'insert'){
        $_POST['title'] = trim($_POST['title']);
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

        $_POST['title'] = strings::sanitizeUrlRigid($_POST['title']);
        $_POST['description'] = html::specialEncode($_POST['description']);
    }

    /**
     * method for getting a count of all tag in database
     * @return int $res number of tags
     */
    public static function getNumTags (){
        $db = new db();
        return $db->getNumRows(self::$tagsTable);
    }

    
    /**
     * return num rows with selected reference
     * @param type $reference
     * @return type 
     */
    public static function getNumTagsFromReference ($reference){
        return dbQ::setSelectNumRows(self::$tagsReferenceTable)->
                filter('reference_name =', $reference)->fetch();
    }
    

    /**
     * method for displaying add controller
     */
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
        templateView::includeModuleView('tags', 'add');        
    }

    /**
     * get all references connected to a tag
     * @param string $reference
     * @param int $tag_id
     * @param int $from
     * @param int $limit
     * @return array $rows 
     */
    public static function getAllReferenceTag ($reference, $tag_id, $from = 0, $limit = 10){
        $db = new dbQ();
        $db->setSelect(self::$tagsReferenceTable, 'reference_id');
        $db->filter('reference_name =', $reference);
        $db->condition('AND');
        $db->filter('tags_id =' , $tag_id);
        $db->limit($from, $limit);
        $rows = $db->fetch();

        $ary = array();
        foreach($rows as $key => $val) {
            $ary[] = $val['reference_id'];
        }

        return $ary;
    }



    /**
     * get count of all references connected to a tag id and a reference
     * @param string $reference
     * @param int $tag_id
     * @return int $num_rows 
     */
    public static function getAllReferenceTagNumRows($reference, $tag_id){
        $db = new dbQ();
        $db->setSelectNumRows(self::$tagsReferenceTable);
        $db->filter('reference_name =', $reference);
        $db->condition('AND');
        $db->filter('tags_id =', $tag_id);
        return $db->fetch();
    }


    /**
     * display index of tags page
     */
    public static function indexController (){
        include_once "pearPager.php";

        $num_tags = self::getNumTags();
        $pager = new pearPager($num_tags, config::getModuleIni('tags_per_page'));

        $db = new db();
        $rows = $db->selectAll(self::$tagsTable, null, null, $pager->from, TAGS_PER_PAGE, 'title');
        templateView::includeModuleView('tags', 'view', $rows);
        $pager->pearPage();
    }

    /**
     * displays update controller
     */
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
        templateView::includeModuleView('tags', 'edit', html::entitiesEncode($row));
    }

    /**
     * displays delete controller
     */
    public static function deleteController (){
        if (isset($_POST['submit'])){
            $res = self::delete();
            if ($res){
                session::setActionMessage(
                    lang::translate('tags_deleted_tag_action_message'));
                header("Location: /tags/index");
            }
        }
        templateView::includeModuleView('tags', 'delete');
    }

    /**
     * method for cleaning tags and inserting them in database if they don't
     * exists.  
     * @param array $tags
     * @return array $tags the cleaned tags
     */
    public static function parse ($tags){
        $db = new db();
        $ary = explode(',', $tags);
        foreach ($ary as $key => $val) {
            $val = trim($val);
            if (empty($val)){
                unset($ary[$key]);
            } else {
                $ary[$key] = $val;
            }
            
        }
        $ary = array_unique($ary);
        foreach($ary as $key => $val){
            $row = $db->selectOne(self::$tagsTable, 'title', $val);
            if ($row){
                $ary[$key] = $row['id'];
            } else {
                if (config::getModuleIni('tags_auto_add')){
                    $tag = array ();
                    $tag['title'] = $val;
                    self::add($tag);
                    $ary[$key] = db::$dbh->lastInsertId();
                } else {
                    unset($ary[$key]);
                }
            }
        }     
        return $ary;
    }

    /**
     * deletes a tag. Only used under tags module and not as a included module
     * @return type 
     */
    public static function delete (){
        $db = new db();
        return $db->delete(self::$tagsTable, 'id', self::getEntryId());

    }

    /**
     * method for updating a tag. 
     * @return boolean $res result from update operation  
     */
    public static function update (){
        $db = new db();
        $values = db::prepareToPost();
        if (!isset($_POST['is_main'])){
            $values['is_main'] = 0;
        } else {
            $values['is_main'] = 1;
        }
        return $db->update(self::$tagsTable, $values, self::getEntryId());
    }

    /**
     * method for getting all main tags.
     * @return array $tags all main tags 
     */
    public static function getMainTags (){
        $db = new db();
        $rows = $db->selectAll(self::$tagsTable, null, array('is_main' => 1));
        return $rows;
    }


    /**
     * gets all tags without using a reference
     * @param int $from
     * @param int $limit
     * @param string $order field to sort by title is default. 
     * @param type $sort ASC / DESC
     * @return type 
     */
    public static function getAllTags ($from = 0, $limit = 10, $order = 'title', $sort = 'ASC'){
        $db = new QBuilder();
        $db->setSelect(self::$tagsTable);
        $db->order($order, $sort);
        $db->limit($from, $limit);       
        $rows = $db->fetch();
        return $rows;
    } 

 
    /**
     * method for getting all tags from a reference 
     * @param string $reference
     * @param int $from
     * @param int $limit
     * @param string $order_by num_rows, tags.title
     * @return array $tags the tags returned from db.  
     */
    public static function getAllTagsFromReference ($reference, $from = 0, $limit = 10, $order_by ='tags.title ASC') {
        
        $tags_table = self::$tagsTable;
        $reference_table = self::$tagsReferenceTable;
        $sql=<<<EOF
SELECT tags.*, COUNT(tags_reference.tags_id) as num_rows 
FROM tags 
LEFT JOIN tags_reference ON tags.id=tags_reference.tags_id 
WHERE tags_reference.reference_name='$reference' 
GROUP BY tags.id 
ORDER by $order_by LIMIT $from, $limit
EOF;
        $stmt = db::$dbh->prepare($sql);
        $stmt->execute();
        $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
        return $rows;
    }
    
    

    public static function viewAdminLinks (&$val){
        echo html::createLink("/tags/edit/$val[id]", lang::translate('tags_admin_edit'));
        echo MENU_SUB_SEPARATOR;
        echo html::createLink("/tags/delete/$val[id]", lang::translate('tags_admin_delete'));
    }
    
    public static function eventForm ($label) {
        html::label('tags', lang::translate('blog_tags'));
        html::widget('tags', 'defaultWidget', 'tags');
    }
    
    public static function events ($params) {
        if (isset($_POST['tags'])) {
            $tags = $_POST['tags'];
            unset($_POST['tags']);
        }
        
        // should sanitize in above functions.
        if ($params['action'] == 'update') {
            $res = tags::updateReference(
                $tags, 
                $params['reference'], 
                $params['parent_id']
            );
        }
        
        if ($params['action'] == 'insert') {
            $res = tags::addReference(
                $tags, 
                $params['reference'], 
                $params['parent_id']
            );
        }
        
        if ($params['action'] == 'delete') {
            $res = tags::deleteReference(
                $params['reference'], 
                $params['parent_id']);
            
        }
        if ($params['action'] == 'form') {
            if (isset($params['parent_id'])  && isset($params['reference'])) {
                $value = tags::getReferenceAsString($params['reference'], $params['parent_id']);
            } else {
                $value = null;
            }

            if (isset($params['extra'])) {
                $extra = $params['extra'];
            } else {
                $extra = array();
            }
            $extra['id'] = 'tags';
            self::initJs();
            html::label('tags', lang::translate('tags_title'));
            html::text('tags', $value, $extra);
        }
        
        if ($params['action'] == 'view') {
            $params['parent_id'];
            $tags_html = tags::getTagReferenceAsHTML(
                $params['reference'], 
                $params['parent_id'], 
                $params['path'] =  '/' . $params['reference'] . '/tags' 
            );
            
            $str = '';
            if (!empty($tags_html)){
                $str.= lang::translate('tags_title') . MENU_SUB_SEPARATOR_SEC;
                $str.= $tags_html;
                $str.= "<br />\n";
            }
            echo "<div class=\"tags\">$str</div>\n";
        }
        
        if ($params['action'] == 'get') {
            //$params['parent_id'];
            $tags_html = tags::getTagReferenceAsHTML(
                $params['reference'], 
                $params['parent_id'], 
                $params['path'] =  '/' . $params['reference'] . '/tags' 
            );
            
            $str = '';
            if (!empty($tags_html)){
                $str.= lang::translate('tags_title') . MENU_SUB_SEPARATOR_SEC;
                $str.= $tags_html;
                $str.= "<br />\n";
                return "<div class=\"tags\">$str</div>\n";
            } else {
                return '';
            }           
        }
    }
}
