R""(
CREATE TABLE IF NOT EXISTS osrs_market.item_source(
ID MEDIUMINT NOT NULL,
source VARCHAR(100) NOT NULL,
noted BOOLEAN NOT NULL DEFAULT(False),
skill varchar(100),
quantity_min INT NOT NULL,
quantity_max INT NOT NULL DEFAULT(quantity_min),
chance_min FLOAT NOT NULL,
chance_max FLOAT NOT NULL DEFAULT(quantity_min),
rolls INT NOT NULL DEFAULT(1),

FOREIGN KEY (ID) REFERENCES osrs_market.item_map(ID) ON UPDATE CASCADE ON DELETE CASCADE,

PRIMARY KEY (ID, source, chance_min, chance_max)
);
)""