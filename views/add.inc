<?php

use diversen\html;
use diversen\lang;

html::$autoLoadTrigger = 'submit';
html::init(array());
html::formStart('blog_form');
html::legend(lang::translate('Add a tag'));
html::label('title', lang::translate('Title'));
html::text('title');
html::label('is_main', lang::translate('Is a main tag'));
html::checkbox('is_main');
html::label('description', lang::translate('Description'));
html::textarea('description', null,
        array ('rows' => 5));

html::label('submit', '');
html::disableBr();
html::submit('submit', lang::translate('Add another'));
html::submit('submit', lang::translate('Submit'));
html::formEnd();

echo html::getStr();
