<?php

header("X-Robots-Tag: noindex");

$db = new db();

if(isset($_GET['term'])) {
    $queryString = $_GET['term'];
    $at_least = config::getModuleIni('tags_min_chars');
    if (!isset($at_least)) {
        $at_least = 0;
    }

    if(strlen($queryString) > $at_least) {
        q::setSelect('tags', 'id, title as label')->filter('title LIKE ', "$queryString%");
        
        $per_page = config::getModuleIni('tags_per_page');
        if ($per_page) {
            q::limit(0,  $per_page);
        }
        
        $rows = q::fetch();
        $json = json_encode($rows);
        echo $json;
    }
}
die;