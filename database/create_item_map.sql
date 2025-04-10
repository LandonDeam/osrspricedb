CREATE TABLE IF NOT EXISTS `item_map`(
`ID` MEDIUMINT NOT NULL,
`name` VARCHAR(100) NOT NULL,
`icon` VARCHAR(300) NOT NULL,
`examine` VARCHAR(1000) NOT NULL,
`members` BOOLEAN NOT NULL,
`value` INT NOT NULL,
`lowalch` INT,
`highalch` INT,
`limit` INT,

PRIMARY KEY (`ID`)
);
