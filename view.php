<?php

$id = tags::getEntryId();
$tag = tags::getTagSingle($id);
//print_r($tag);
$refs = tags::getAllReferenceTagWithoutReference($tag['id'], 0, 10);
print_r($refs);
