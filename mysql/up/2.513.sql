ALTER table `tags_reference` ADD column `published` tinyint(1) DEFAULT 0;

UPDATE `tags_reference` SET published = 1;

DROP index `reference_name_index` ON `tags_reference`;

DROP index  `reference_id_index` ON `tags_reference`;

DROP index `user_id_index` ON `tags`;

DROP index `tags_id_index` ON `tags_reference`;

CREATE INDEX ref_tag_pub ON `tags_reference` (`reference_name`, `tags_id`, `published`);