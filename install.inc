<?php

use diversen\lang;

$_INSTALL['MAIN_MENU_ITEM'] = array (
    'title' => lang::translate('Tags', null, array ('no_translate' => true)),
    'url' => '/tags/index',
    'module_name' => 'tags',
    'parent' => 0,
    'weight' => 2,
    'admin_only' => 1,
    'section' => 'content'
);

$_INSTALL['PUBLIC_CLONE_URL'] = "git://github.com/diversen/tags.git";
$_INSTALL['PRIVATE_CLONE_URL'] = 'git@github.com:diversen/tags.git';
