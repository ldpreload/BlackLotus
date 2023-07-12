/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP DATABASE IF EXISTS `panel`;
CREATE DATABASE IF NOT EXISTS `panel` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `panel`;

DROP TABLE IF EXISTS `bots`;
CREATE TABLE IF NOT EXISTS `bots` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` varchar(50) NOT NULL,
  `ip` int(11) unsigned NOT NULL,
  `os` tinyint(4) unsigned NOT NULL,
  `arch` tinyint(4) unsigned NOT NULL,
  `country` char(2) NOT NULL,
  `username` varchar(50) NOT NULL,
  `last_seen` int(11) unsigned NOT NULL,
  `first_seen` int(11) unsigned NOT NULL,
  `last_command` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `commands`;
CREATE TABLE IF NOT EXISTS `commands` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `execs` int(11) unsigned NOT NULL,
  `limit` int(11) unsigned NOT NULL,
  `enabled` tinyint(4) unsigned NOT NULL,
  `created` int(11) unsigned NOT NULL,
  `type` tinyint(4) unsigned NOT NULL,
  `param` text NOT NULL,
  `countries` text NOT NULL,
  `guids` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
