DROP TABLE IF EXISTS `tags`;

CREATE TABLE `tags` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `description` text,
  `title` varchar(256) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS `tags_reference`;

CREATE TABLE `tags_reference` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `reference_id` int(10),
  `reference_name` varchar(256) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;