CREATE TABLE IF NOT EXISTS osrs_market.price_update(
ID MEDIUMINT NOT NULL,
price_buy INT NOT NULL,
price_sell INT NOT NULL,
updated TIMESTAMP(0) NOT NULL,

FOREIGN KEY (ID) REFERENCES osrs_market.item_map(ID) ON UPDATE CASCADE,

PRIMARY KEY (ID, updated)
);
