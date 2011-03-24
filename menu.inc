<?php

$_MODULE_MENU[] = array(
    'title' => lang::translate('tags_menu_list'),
    'url' => '/tags/index',
);

$tags_allow = get_module_ini('tags_allow');

$_MODULE_MENU[] = array(
    'title' => lang::translate('tags_menu_add'),
    'url' => '/tags/add',
    // note that we have set an auth element!
    'auth' => $tags_allow
);