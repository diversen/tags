<?php

template::setInlineCss(_COS_PATH . "/modules/tags/tags.css");
template::setInlineJs(_COS_PATH . "/modules/tags/tags.js");

define ('TAGS_PER_PAGE', get_module_ini('tags_per_page'));

class tags {

    public static $errors = array();
    public static $tagsTable = 'tags';

    public static function defaultWidget (){
        $str = <<<EOD
<div class="ui-widget">
	<input type="text" name="tags" id="tags" size="50" />
</div>
EOD;
     return $str;
    }

    public static function getEntryId (){
        return uri::$fragments[2];
    }

    public static function add (){
        $db = new db();
        $values = db::prepareToPost($_POST);
        $res = $db->insert(self::$tagsTable, $values);
        return $res;
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