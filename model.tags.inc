<?php

template::setInlineCss(_COS_PATH . "/modules/tags/tags.css");
template::setInlineJs(_COS_PATH . "/modules/tags/tags.js");

class tags {
    public static function defaultWidget (){ 
        $str = <<<EOD
<div class="ui-widget">
	<input type="text" name="tags" id="tags" size="50" />
</div>
EOD;
     return $str;
     }
}