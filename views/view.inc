<?php


foreach ($vars as $key => $val){
    echo "<h3>" . html::createLink("/tags/view/$val[id]", $val['title']) . "</h3>";
    echo $val['description'] . "<br />";
    if (session::isAdmin()){
        tags::viewAdminLinks($val);
    }

    echo "<hr />";
}