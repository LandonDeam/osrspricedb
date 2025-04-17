R""(
CREATE TABLE IF NOT EXISTS osrs_market.item_map(
ID MEDIUMINT NOT NULL,
item_name VARCHAR(100) NOT NULL,
icon VARCHAR(300) NOT NULL,
examine VARCHAR(1000) NOT NULL,
members BOOLEAN NOT NULL,
item_value INT NOT NULL,
lowalch INT,
highalch INT,
ge_limit INT,

PRIMARY KEY (ID)
);
)""