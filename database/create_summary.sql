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
  (bu.price - su.price) AS profit,

  UNIX_TIMESTAMP(GREATEST(bu.updated, su.updated)) * 1000 AS last_update,

  -- Standard deviation calculations
  buy_stats.stddev_buy,
  sell_stats.stddev_sell,

  GROUP_CONCAT(DISTINCT isrc.source ORDER BY isrc.source SEPARATOR ', ') AS sources

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

-- Stddev (buy)
LEFT JOIN (
  SELECT
    ps.ID,
    STDDEV_SAMP(pu.price) AS stddev_buy
  FROM osrs_market.price_series ps
  JOIN osrs_market.price_update pu
    ON pu.ID = ps.ID AND pu.updated = ps.buy_updated AND pu.price_type = 'buy'
  WHERE ps.fetched >= NOW() - INTERVAL 1 HOUR
  GROUP BY ps.ID
) buy_stats ON buy_stats.ID = im.ID

-- Stddev (sell)
LEFT JOIN (
  SELECT
    ps.ID,
    STDDEV_SAMP(pu.price) AS stddev_sell
  FROM osrs_market.price_series ps
  JOIN osrs_market.price_update pu
    ON pu.ID = ps.ID AND pu.updated = ps.sell_updated AND pu.price_type = 'sell'
  WHERE ps.fetched >= NOW() - INTERVAL 1 HOUR
  GROUP BY ps.ID
) sell_stats ON sell_stats.ID = im.ID

-- Drop sources
LEFT JOIN osrs_market.item_source isrc ON isrc.item_ID = im.ID

GROUP BY im.ID;
)""