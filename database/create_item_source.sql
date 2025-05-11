R""(
CREATE TABLE IF NOT EXISTS osrs_market.item_source(
item_ID MEDIUMINT NOT NULL,
source_ID INT NOT NULL,
source VARCHAR(100) NOT NULL,
noted BOOLEAN NOT NULL,
skill varchar(100),
quantity_min INT,
quantity_max INT,
chance_min DECIMAL(21, 20),
chance_max DECIMAL(21, 20),
rolls INT NOT NULL,

FOREIGN KEY (item_ID) REFERENCES osrs_market.item_map(ID) ON UPDATE CASCADE ON DELETE CASCADE,

PRIMARY KEY (item_ID, source_ID)
);
)""