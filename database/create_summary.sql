R""(
CREATE OR REPLACE VIEW osrs_market.item_summary_view AS
SELECT
  im.ID AS item_ID,
  im.item_name,
  im.icon,
  im.examine,
  im.members,
  im.item_value,
  im.lowalch,
  im.highalch,
  im.ge_limit,

  pu_buy.price AS buy_price,
  pu_sell.price AS sell_price,
  (pu_sell.price - pu_buy.price) AS profit,

  ps.fetched AS last_fetched,

  isrc.source,
  isrc.quantity_min,
  isrc.quantity_max,
  isrc.chance_min,
  isrc.chance_max,
  isrc.rolls,
  isrc.noted,
  isrc.skill

FROM osrs_market.item_map im
JOIN osrs_market.price_series ps ON ps.ID = im.ID
JOIN osrs_market.price_update pu_buy
  ON pu_buy.ID = im.ID
  AND pu_buy.price_type = 'buy'
  AND pu_buy.updated = ps.buy_updated
JOIN osrs_market.price_update pu_sell
  ON pu_sell.ID = im.ID
  AND pu_sell.price_type = 'sell'
  AND pu_sell.updated = ps.sell_updated
LEFT JOIN osrs_market.item_source isrc ON isrc.item_ID = im.ID;
)""