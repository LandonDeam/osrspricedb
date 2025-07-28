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

  bu.price AS buy_price,
  su.price AS sell_price,

  ( GREATEST(bu.price, su.price) -
    LEAST(bu.price, su.price) -
    FLOOR(LEAST(
      GREATEST(bu.price, su.price) * 2 / 100,
      5000000))) AS profit,

  ( CAST((GREATEST(bu.price, su.price) -
          LEAST(bu.price, su.price) -
          FLOOR(LEAST(
            GREATEST(bu.price, su.price) * 2 / 100,
            5000000))) AS DECIMAL(10,2))
    / LEAST(bu.price, su.price) * 100) AS roi,



  UNIX_TIMESTAMP(GREATEST(bu.updated, su.updated)) * 1000 AS last_update

FROM osrs_market.item_map im

-- Join the latest price_series row per item
JOIN (
  SELECT ps1.*
  FROM osrs_market.price_series ps1
  JOIN (
    SELECT ID, MAX(fetched) AS max_fetched
    FROM osrs_market.price_series
    GROUP BY ID
  ) latest
  ON ps1.ID = latest.ID AND ps1.fetched = latest.max_fetched
) ps ON ps.ID = im.ID

-- Join price_update rows using timestamps from price_series
JOIN osrs_market.price_update bu
  ON bu.ID = ps.ID AND bu.updated = ps.buy_updated AND bu.price_type = 'buy'

JOIN osrs_market.price_update su
  ON su.ID = ps.ID AND su.updated = ps.sell_updated AND su.price_type = 'sell'

GROUP BY im.ID;
)""