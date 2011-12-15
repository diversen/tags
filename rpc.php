<?php

$db = new db();

if(isset($_GET['term'])) {
    $queryString = $_GET['term'];
    $at_least = get_module_ini('tags_min_chars');
    if (!isset($at_least)) {
        $at_least = 0;
    }

    if(strlen($queryString) > $at_least) {
        $query = "SELECT id, title as label FROM tags WHERE title LIKE ". db::$dbh->quote("" . $queryString . "%");
        error_log($query);
        $rows = $db->selectQuery($query);
        $json = json_encode($rows);
        echo $json;
    }
}
die;