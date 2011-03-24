<?php

$db = new db();

if(isset($_GET['term'])) {
    $queryString = cos_htmlentities($_GET['term']);

    if(strlen($queryString) > 2) {
        $query = "SELECT id, title as value FROM tags WHERE title LIKE ". db::$dbh->quote("%" . $queryString . "%");
        $rows = $db->selectQuery($query);
        $json = json_encode($rows);
        echo $json;
    }
}
die;