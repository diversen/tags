<?php

class tags_api {
    
    /**
     * get tags from /tags/api/[action]/1/1+2
     */
    public function getTags () {
        $tags = uri::fragment(4);
        $ary = explode("-", $tags);
        return $ary;
        
    }
    
    public function deleteTagsAndReferences () {
        $ary = $this->getTags();
        foreach ($ary as $val) {
            db_q::delete('tags_reference')->filter('tags_id =', $val)->exec();
            db_q::delete('tags')->filter('id =', $val)->exec();
        }
    }
    
    public function deleteAction () {
        if (session::isSuper()) {
            $this->deleteTagsAndReferences();
            echo lang::translate('Tags has been deleted!');
        } else {
            moduleloader::setStatus(403);
            error_module::$message = lang::translate('Not sufficient privileges. Super user is required');
            return;
        }
    }
}