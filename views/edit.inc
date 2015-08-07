<?php

use diversen\html;
use diversen\lang;

html::$autoLoadTrigger = 'submit';
html::init($vars);
html::formStart('blog_form');
html::legend(lang::translate('Edit tag'));
html::label('title', lang::translate('Title'));
html::text('title');
html::label('is_main', lang::translate('Is a main tag'));
html::checkbox('is_main');
html::label('description', lang::translate('Description'));
html::textarea('description', null,
        array ('rows' => 5));
html::submit('submit', lang::translate('Update'));
html::formEnd();

echo html::getStr();