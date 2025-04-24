R""(
CREATE TABLE IF NOT EXISTS osrs_market.price_series(
ID MEDIUMINT NOT NULL,
fetched TIMESTAMP(0) NOT NULL DEFAULT CURRENT_TIMESTAMP,
buy_updated TIMESTAMP(0) NOT NULL REFERENCES osrs_market.price_update(updated),
sell_updated TIMESTAMP(0) NOT NULL REFERENCES osrs_market.price_update(updated),

FOREIGN KEY (ID) REFERENCES osrs_market.item_map(ID) ON UPDATE CASCADE ON DELETE CASCADE,

PRIMARY KEY (fetched, ID)
);
)""