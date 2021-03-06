<?php

use diversen\html;
use diversen\lang;
use diversen\moduleloader;

moduleloader::includeModule ('tags');


function block_tags (){
    $str = '';
    $str.= "<h3>" . lang::translate('Main tags') . "</h3>\n";
    $str.= "<div class =\"block\">\n";
    $str.= "<ul>\n";
    $elements = tags::getMainTags();
    foreach ($elements as $key => $val) {
        $str.="<li>\n";
        $str.= html::createLink("/tags/view/$val[id]", $val['title']);
        $str.="</li>\n";
    }   
    $str.= "</div>\n";
    return $str;
}