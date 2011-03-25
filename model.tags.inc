<?php

template::setInlineCss(_COS_PATH . "/modules/tags/tags.css");
template::setInlineJs(_COS_PATH . "/modules/tags/tags.js");

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
    public static function defaultWidget ($name, $value){
        $str = <<<EOD
<div class="ui-widget">
	<input type="text" name="$name" id="tags" size="50" value="$value" />
</div>
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
    public static function add (){
        $db = new db();
        $values = db::prepareToPost($_POST);
        $res = $db->insert(self::$tagsTable, $values);
        return $res;
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
        
    }

    public static function deleteReference ($reference, $id) {

    }
    /**
     *
     * @param string $reference (e.g. 'blog')
     * @param int    $id (e.g. blog_id
     */
    public static function getReference($reference, $id) {
        $db = new db();
        $search = array ('reference_name' => $reference, 'reference_id' => $id);
        $tags = $db->selectAll(self::$tagsReferenceTable, null, $search);

        $tags_str = '';
        foreach ($tags as $key => $val){
            $tag = $db->selectOne(self::$tagsTable, 'id', $val['tags_id']);
            $tags_str.= $tag['title'] . ", ";
        }
        return $tags_str;
    }
    
    public static function prepare (){
        if (empty($_POST['title'])){
            self::$errors['title'] = lang::translate('tags_error_no_title');
        }
        $_POST = html::entitiesEncode($_POST);
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
            self::prepare();
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
                unset($ary[$key]);
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