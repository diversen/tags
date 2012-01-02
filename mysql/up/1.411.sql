CREATE INDEX `reference_name_index` ON `tags_reference`(`reference_name`);

CREATE INDEX `reference_id_index` ON `tags_reference`(`reference_id`);

CREATE INDEX `tags_id_index` ON `tags_reference`(`tags_id`);

CREATE INDEX `user_id_index` ON `tags`(`user_id`);

CREATE INDEX `title_index` ON `tags`(`title`);