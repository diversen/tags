<?php

use diversen\session;

foreach ($vars as $key => $val){
    //echo "<h3>"  . html::createLink("/tags/view/$val[id]", $val['title']) /* $val['title']*/ . "</h3>";
    echo "<h3><a title=\"$val[description]\">$val[title]</a></h3>";

    echo "<br />";
    if (session::isAdmin()){
        tags::viewAdminLinks($val);
    }

    echo "<hr />";
}