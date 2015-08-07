<?php

use diversen\conf;
use diversen\lang;

$_MODULE_MENU[] = array(
    'title' => lang::translate('List tags', null ,array ('no_translate' => true)),
    'url' => '/tags/index',
);

$tags_allow = conf::getModuleIni('tags_allow');

$_MODULE_MENU[] = array(
    'title' => lang::translate('Add tag', null ,array ('no_translate' => true)),
    'url' => '/tags/add',
    // note that we have set an auth element!
    'auth' => $tags_allow
);