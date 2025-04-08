CREATE SCHEMA osrs_market;
USE osrs_market;

CREATE TABLE `prices_series`(
`ID` MEDIUMINT NOT NULL,
`updated` TIMESTAMP(0) NOT NULL,
`fetched` TIMESTAMP(0) NOT NULL DEFAULT CURRENT_TIMESTAMP,

FOREIGN KEY (`updated`) REFERENCES `price_update`(`updated`),

PRIMARY KEY (`ID`, `fetched`)
);

CREATE TABLE `price_update`(
`ID` MEDIUMINT NOT NULL,
`price_buy` INT NOT NULL,
`price_sell` INT NOT NULL,
`updated` TIMESTAMP(0) NOT NULL,

PRIMARY KEY (`ID`, `updated`)
);