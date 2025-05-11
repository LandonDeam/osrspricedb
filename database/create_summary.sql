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
  (su.price - bu.price) AS profit,
  ps.fetched AS last_fetched,

  GROUP_CONCAT(DISTINCT isrc.source ORDER BY isrc.source SEPARATOR ', ') AS sources

FROM osrs_market.item_map im

-- Join to most recent price_series record per item
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

-- Join to the actual price_update rows
JOIN osrs_market.price_update bu
  ON bu.ID = ps.ID AND bu.updated = ps.buy_updated AND bu.price_type = 'buy'

JOIN osrs_market.price_update su
  ON su.ID = ps.ID AND su.updated = ps.sell_updated AND su.price_type = 'sell'

-- Optional: join to drop sources
LEFT JOIN osrs_market.item_source isrc ON isrc.item_ID = im.ID

GROUP BY im.ID;
)""