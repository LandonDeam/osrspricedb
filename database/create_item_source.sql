R""(
CREATE TABLE IF NOT EXISTS osrs_market.item_source(
ID MEDIUMINT NOT NULL,
source VARCHAR(100) NOT NULL,
noted BOOLEAN NOT NULL,
skill varchar(100),
quantity_min INT,
quantity_max INT,
chance_min FLOAT,
chance_max FLOAT,
rolls INT NOT NULL,

FOREIGN KEY (ID) REFERENCES osrs_market.item_map(ID) ON UPDATE CASCADE ON DELETE CASCADE,

PRIMARY KEY (ID, source, chance_min, chance_max)
);
)""