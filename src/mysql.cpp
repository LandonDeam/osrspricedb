// Copyright Landon Deam 2025

#include "mysql.h"
#include <mysqlx/devapi/common.h>
#include <mysqlx/xdevapi.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <vector>
#include "utils.h"

// Evil pre-processor shenanigans for importing files as string literals
const char* price_series_sql =
  #include "../database/create_price_series.sql"
;

const char* price_update_sql =
  #include "../database/create_price_update.sql"
;

const char* item_map_sql =
  #include "../database/create_item_map.sql"
;

const char* item_source_sql =
  #include "../database/create_item_source.sql"
;

const char* schema_sql =
  #include "../database/create_db.sql"
;

const char* summary_view_sql =
  #include "../database/create_summary.sql"
;

/**
* @brief Initialize the database connection.
*/
void db_connection::init() {
  size_t bufferSize;

  username = utils::urlEncode(std::getenv("MYSQL_USERNAME"));
  password = utils::urlEncode(std::getenv("MYSQL_PASSWORD"));
  port = std::getenv("MYSQL_PORT");
  host = std::getenv("MYSQL_HOST");

  try {
    std::string url("mysqlx://"+username+":"+password+"@"+host+":"+port);
    std::cout << "Creating session..."
              << "mysqlx://"+username+":"+"[PASSWORD]"+"@"+host+":"+port
              << "..." << std::endl;

    sess = std::make_shared<mysqlx::Session>(url);

    std::cout <<"Session accepted." << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  initialized = true;
}

/**
* @brief Close the database connection.
*/
void db_connection::close() {
  if (initialized == false) {
    std::cout << "Session is not open." << std::endl;
    return;
  }

  try {
    std::cout << "Attempting to close session..." << std::endl;
    sess->close();
    std::cout << "Session closed." << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  initialized = false;
}

/**
* @brief Connect to the database.
*/
void db_connection::connect_db() {
  if (!initialized) {
    init();
  }  // Init connection if not already connected

  db =
    std::make_shared<mysqlx::Schema>(sess->getSchema("osrs_market"));

  if (db->existsInDatabase()) {
    connect_tables();
  } else {
    build_schema();
  }
}

/**
* @brief Connect to the tables.
*/
void db_connection::connect_tables() {
  std::cout << "Attempting to connect tables..." << std::endl;
  item_map =
      std::make_shared<mysqlx::Table>(db->getTable("item_map"));
  if (!item_map->existsInDatabase()) {
    build_item_map();  // Create item_map table if it doesn't exist
  }
  std::cout << "Item map table connected." << std::endl;

  price_update =
    std::make_shared<mysqlx::Table>(db->getTable("price_update"));
  if (!price_update->existsInDatabase()) {
    build_price_update();  // Create price_updated table if it doesn't exist
  }
  std::cout << "Price update table connected." << std::endl;

  price_series =
    std::make_shared<mysqlx::Table>(db->getTable("price_series"));
  if (!price_series->existsInDatabase()) {
    build_price_series();  // Create price_series table if it doesn't exist
  }
  std::cout << "Price series table connected." << std::endl;

  item_source =
      std::make_shared<mysqlx::Table>(db->getTable("item_source"));
  if (!item_source->existsInDatabase()) {
    build_item_source();  // Create item_map table if it doesn't exist
  }
  std::cout << "Item source table connected." << std::endl;

  try {
    std::cout << "Creating/Replacing Summary View..." << std::endl;
    build_summary_view();
  } catch (const std::exception& e) {
    std::cerr << "Error creating summary view: "
      << e.what() << std::endl;
  }
}

/**
* @brief Builds the database schema and connects to the tables.
*/
void db_connection::build_schema() {
  // Build schema
  std::cout << "Building database schema..." << std::endl;
  sess->sql(schema_sql).execute();
  // Create tables
  connect_tables();
}

/**
* @brief Builds the price series table.
*/
void db_connection::build_price_series() {
  build_table("price series", price_series_sql);
}

/**
* @brief Builds the price update table.
*/
void db_connection::build_price_update() {
  build_table("price update", price_update_sql);
}

/**
* @brief Builds the item map table.
*/
void db_connection::build_item_map() {
  build_table("item map", item_map_sql);
}

/**
* @brief Builds the item source table.
*/
void db_connection::build_item_source() {
  build_table("item source", item_source_sql);
}

/**
* @brief Builds the view for the summary
*/
void db_connection::build_summary_view() {
  build_table("summary view", summary_view_sql);
}

/**
* @brief Builds the given table with the specified create command.
* @param table_name The name of the table to build.
* @param create_command The SQL command used to create the table.
*/
void db_connection::build_table(const std::string& table_name,
  const std::string& create_command) {
  std::cout << "Building " << table_name << " table..." << std::endl;
  try {
    sess->sql("START TRANSACTION;");
    mysqlx::SqlResult res = sess
      ->sql(create_command)
      .execute();

    if (res.getWarningsCount() > 0) {
      std::cout << "Warning(s) while creating " << table_name << " table:"
        << std::endl;
      for (mysqlx::Warning warn : res.getWarnings()) {
        std::cout << warn.getMessage() << std::endl;
      }
    }

    sess->sql("COMMIT;").execute();
  } catch (std::exception& e) {
    std::cerr << "Error building " << table_name << " table: " << e.what()
      << std::endl;
    sess->sql("ROLLBACK;").execute();
  }
}

/**
* @brief Writes a given item map into the MySQL database
* @param items Unordered map of item IDs and their corresponding mappings
*/
void db_connection::writeItemMap(
  const std::unordered_map<int, class item_map>& items) {
  std::cout << "Writing item map with " << items.size()
    << " items(s)..." << std::endl;
  while (transaction)
    sleep(1);
  transaction = true;
  sess->sql("START TRANSACTION;");
  std::string last_query = "START TRANSACTION;";
  std::cout << "Starting transaction..." << std::endl;
  try {
    std::string query = R"(REPLACE INTO osrs_market.item_map (ID, item_name, icon, examine, members, item_value, lowalch, highalch, ge_limit) VALUES)";
    bool first = true;
    for (const auto& [ID, item] : items) {
      query+= first ? "\n(" : ",\n(" ;
      first = false;
      query+=std::to_string(item.getID())+", ";
      query+="\""+item.getName()+"\", ";
      query+="\""+item.getIcon()+"\", ";
      query+="\""+item.getExamine()+"\", ";
      query+=item.isMembers() ? "true, " : "false, ";
      query+=std::to_string(item.getValue())+", ";
      if (item.getLowAlch() >= 0) {
        query+=std::to_string(item.getLowAlch())+", ";
      } else {
        query+="NULL, ";
      }
      if (item.getHighAlch() >= 0) {
        query+=std::to_string(item.getHighAlch())+", ";
      } else {
        query+="NULL, ";
      }
      if (item.getLimit() >= 0) {
        query+=std::to_string(item.getLimit());
      } else {
        query+="NULL";
      }
      query+=")";
    }
    query+=";";
    last_query = query;
    sess->sql(query).execute();

    sess->sql("COMMIT;").execute();
    transaction = false;
  } catch (std::exception& e) {
    std::cerr << "Error writing item map: " << e.what() << std::endl;
    std::cout << "Last query: " << last_query << std::endl;
    sess->sql("ROLLBACK;").execute();
    transaction = false;
  }
}

/**
* @brief Writes item prices into the MySQL database
* @param prices Unordered map of item IDs and their corresponding prices
* @param timestamp Timestamp in Unix seconds to save to database as "fetched"
* time
*/
void db_connection::writePrices(const std::unordered_map<int,
  class price_update>& prices, uint64_t timestamp) {
  std::cout << "Writing prices...                               " << std::endl;
  std::string last_query;
  try {
    while (transaction)
      sleep(1);
    sess->sql("START TRANSACTION;");
    last_query = "START TRANSACTION;";
    transaction = true;
    bool first = true;

    std::string buy_query = R"(REPLACE INTO osrs_market.price_update (ID, price_type, price, updated) VALUES)";
    std::string sell_query = R"(REPLACE INTO osrs_market.price_update (ID, price_type, price, updated) VALUES)";
    std::string series_query = R"(REPLACE INTO osrs_market.price_series (ID, fetched, buy_updated, sell_updated) VALUES)";
    for (const auto& [id, item] : prices) {
      std::string test_query =
        "SELECT ID FROM osrs_market.item_map WHERE ID = "
        + std::to_string(id) + ";";
      last_query = test_query;
      auto res = sess->sql(test_query).execute();
      if (!res.hasData()) {
        continue;
      } else if (res.count() == 0) {
        continue;
      }

      buy_query+= first ? "\n(" : ",\n(" ;
      buy_query+=std::to_string(id)+", ";
      buy_query+="\"buy\", ";
      buy_query+=item.get_buy() >= 0 ?
        std::to_string(item.get_buy())+", " : "NULL, ";
      buy_query+=item.get_buy_timestamp() > 0 ?
        "FROM_UNIXTIME("+std::to_string(item.get_buy_timestamp())+"))"
        : "FROM_UNIXTIME(1361491200))";

      sell_query+= first ? "\n(" : ",\n(" ;
      sell_query+=std::to_string(id)+", ";
      sell_query+="\"sell\", ";
      sell_query+=item.get_sell() >= 0 ?
        std::to_string(item.get_sell())+", " : "NULL, ";
      sell_query+=item.get_sell_timestamp() > 0 ?
        "FROM_UNIXTIME("+std::to_string(item.get_sell_timestamp())+"))"
        : "FROM_UNIXTIME(1361491200))";

      series_query+= first ? "\n(" : ",\n(" ;
      series_query+=std::to_string(id)+", ";
      series_query+="FROM_UNIXTIME("+std::to_string(timestamp)+"), ";
      series_query+=item.get_buy_timestamp() > 0 ?
        "FROM_UNIXTIME("+std::to_string(item.get_buy_timestamp())+"), "
        : "FROM_UNIXTIME(1361491200), ";
      series_query+=item.get_sell_timestamp() > 0 ?
        "FROM_UNIXTIME("+std::to_string(item.get_sell_timestamp())+"))"
        : "FROM_UNIXTIME(1361491200))";
      first = false;
    }

    buy_query+=";";
    last_query = buy_query;
    sess->sql(buy_query).execute();
    sell_query+=";";
    last_query = sell_query;
    sess->sql(sell_query).execute();
    series_query+=";";
    last_query = series_query;
    sess->sql(series_query).execute();
    sess->sql("COMMIT;").execute();
    transaction = false;
  } catch (std::exception& e) {
    std::cerr << "Error writing prices: " << e.what() << std::endl;
    std::cout << "Last query: " << last_query << std::endl;
    sess->sql("ROLLBACK;").execute();
    transaction = false;
  }
}

/**
* @brief Writes item drop sources into the MySQL database
* @param all_sources Unordered map of item IDs and their corresponding lists of
* drop sources.
*/
void db_connection::writeItemSources(
  const std::unordered_map<int, std::vector<class item_source>>& all_sources) {
  std::cout << "Writing sources... " << std::endl;
  std::string last_query;
  try {
    while (transaction)
      sleep(1);
    sess->sql("START TRANSACTION;");
    last_query = "START TRANSACTION;";
    transaction = true;
    bool first = true;
    std::string query = R"(REPLACE INTO osrs_market.item_source (item_ID, source_ID, source, noted, skill, quantity_min, quantity_max, chance_min, chance_max, rolls) VALUES)";
    for (auto&& [ID, sources] : all_sources) {
      for (size_t i = 0; i < sources.size(); i++) {
        auto&& source = sources.begin()+i;
        query+= first ? "\n(" : ",\n(";
        first = false;
        query+=std::to_string(ID) + ",";
        query+=std::to_string(i) + ",";
        query+="\""+source->getSource()+"\",";
        query+=std::to_string(source->isNoted()) + ",";
        query+="\""+source->getSkill()+"\",";
        query+=source->getQuantityMin() >= 0 ?
          std::to_string(source->getQuantityMin()) + "," : "NULL,";
        query+=source->getQuantityMax() >= 0 ?
          std::to_string(source->getQuantityMax()) + "," : "NULL,";
        query+=source->getChanceMin() >= 0.0f ?
          std::to_string(source->getChanceMin())+"," : "NULL,";
        query+=source->getChanceMax() >= 0.0f ?
          std::to_string(source->getChanceMax()) + "," : "NULL,";
        query+=std::to_string(source->getRolls()) + ")";
      }
    }

    query+=";";
    last_query = query;
    sess->sql(query).execute();

    sess->sql("COMMIT;").execute();
    transaction = false;
  } catch (std::exception& e) {
    std::cerr << "Error writing sources: " << e.what() << std::endl;
    std::cout << "Last query: " << last_query << std::endl;
    sess->sql("ROLLBACK;").execute();
    transaction = false;
  }
}

/**
* @brief Writes a single item's drop sources into the MySQL database.
* Meant for debugging purposes
* @param sources Vector of item drop sources.
*/
void db_connection::writeItemSource(
  const std::vector<class item_source>& sources) {
  std::cout << "Writing source... " << std::endl;
  std::string last_query;
  try {
    while (transaction)
      sleep(1);
    sess->sql("START TRANSACTION;");
    last_query = "START TRANSACTION;";
    std::string query = R"(REPLACE INTO osrs_market.item_source (item_ID, source_ID, source, noted, skill, quantity_min, quantity_max, chance_min, chance_max, rolls) VALUES)";
    transaction = true;
    bool first = true;
    for (size_t i = 0; i < sources.size(); i++) {
      auto&& source = sources.begin()+i;
      query+= first ? "\n(" : ",\n(";
      query+=std::to_string(source->getID()) + ",";
      query+=std::to_string(i) + ",";
      query+="\""+source->getSource()+"\",";
      query+=std::to_string(source->isNoted()) + ",";
      query+="\""+source->getSkill()+"\",";
      query+=source->getQuantityMin() >= 0 ?
        std::to_string(source->getQuantityMin()) + "," : "NULL,";
      query+=source->getQuantityMax() >= 0 ?
      std::to_string(source->getQuantityMax()) + "," : "NULL,";

      std::stringstream chance_stream;
      chance_stream << std::fixed <<
        std::setprecision(19) << source->getChanceMin();
      query+=source->getChanceMin() >= 0.0f ?
        chance_stream.str()+"," : "NULL,";
      chance_stream.str("");
      chance_stream << std::fixed <<
        std::setprecision(19) << source->getChanceMax();
      query+=source->getChanceMax() >= 0.0f ?
        chance_stream.str() + "," : "NULL,";
      query+=std::to_string(source->getRolls()) + ")";
    }
    query+=";";

    last_query = query;
    sess->sql(query).execute();

    sess->sql("COMMIT;").execute();
    transaction = false;
  } catch (std::exception& e) {
    std::cerr << "Error writing sources: " << e.what() << std::endl;
    std::cout << "Last query: " << last_query << std::endl;
    sess->sql("ROLLBACK;").execute();
    transaction = false;
  }
}

// void db_connection::writeItemVolumes(
//   const std::vector<class item_volume>& sources, uint64_t timestamp) {

// }
