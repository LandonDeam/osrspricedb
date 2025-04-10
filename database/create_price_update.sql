USE osrs_market;

CREATE TABLE IF NOT EXISTS `price_update`(
`ID` MEDIUMINT NOT NULL,
`price_buy` INT NOT NULL,
`price_sell` INT NOT NULL,
`updated` TIMESTAMP(0) NOT NULL,

FOREIGN KEY (`ID`) REFERENCES `item_map`(`ID`) ON UPDATE CASCADE,

PRIMARY KEY (`ID`, `updated`)
);
