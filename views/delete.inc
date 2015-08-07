<?php

use diversen\html;
use diversen\lang;

html::formStart('tag_delete_form');
html::legend(lang::translate('Delete tag'));
html::submit('submit', lang::translate('Delete'));
html::formEnd();

echo html::getStr();