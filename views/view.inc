<?php


foreach ($vars as $key => $val){
    echo "<h3>$val[title]</h3>";
    echo $val['description'] . "<br />";
    tags::viewAdminLinks($val);

    echo "<hr />";
}