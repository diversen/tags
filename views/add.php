<?php

html::$autoLoadTrigger = 'submit';
html::init($values);
html::formStart('blog_form');
html::legend(lang::translate('tags_add_tags_legend'));
html::label('title', lang::translate('tags_title'));
html::text('title');

html::label('description', lang::translate('tags_label_description'));
html::textarea('description');
html::submit('submit', lang::translate('tags_submit_add'));
html::formEnd();

echo html::$formStr;
