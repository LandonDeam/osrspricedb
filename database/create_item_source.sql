R""(
CREATE TABLE IF NOT EXISTS osrs_market.item_source(
ID MEDIUMINT NOT NULL,
source VARCHAR(100) NOT NULL,
quantity_min INT NOT NULL,
quantity_max INT,
noted BOOLEAN NOT NULL,

FOREIGN KEY (ID) REFERENCES osrs_market.item_map(ID) ON UPDATE CASCADE ON DELETE CASCADE,

PRIMARY KEY (ID, source)
);
)""