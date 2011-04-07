<?php

if (!session::checkAccessControl('tags_allow_edit')){
    return;
}

tags::indexController();