DROP TABLE IF EXISTS `tags`;

CREATE TABLE `tags` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `title` varchar(256) NOT NULL,
  `description` text DEFAULT '',
  `user_id` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS `tags_reference`;

CREATE TABLE `tags_reference` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `tags_id` int(10) NOT NULL,
  `reference_id` int(10),
  `reference_name` varchar(256) DEFAULT '',
  FOREIGN KEY (`tags_id`) REFERENCES `tags` (id)
    ON DELETE CASCADE ON UPDATE CASCADE,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;