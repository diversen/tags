<?php

html::formStart('tag_delete_form');
html::legend(lang::translate('tags_delete_tags_legend'));
html::submit('submit', lang::translate('tags_submit_delete'));
html::formEnd();

echo html::$formStr;