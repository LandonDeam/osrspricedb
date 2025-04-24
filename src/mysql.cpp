// Copyright 2025 Landon Deam

#include "mysql.h"
#include <mysqlx/devapi/common.h>
#include <mysqlx/xdevapi.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <unordered_map>
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

const char* schema_sql =
  #include "../database/create_db.sql"
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

void db_connection::writeItemMap(
  const std::unordered_map<int, class item_map>& items) {
  sess->sql("START TRANSACTION;");
  std::string last_query = "START TRANSACTION;";
  try {
    for (auto [ID, item] : items) {
      std::string query = R"(REPLACE INTO osrs_market.item_map (ID, item_name, icon, examine, members, item_value, lowalch, highalch, ge_limit) VALUES ()";
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
      query+=");";
      last_query = query;
      sess->sql(query).execute();
    }

    sess->sql("COMMIT;").execute();
  } catch (std::exception& e) {
    std::cerr << "Error writing item map: " << e.what() << std::endl;
    std::cout << "Last query: " << last_query << std::endl;
    sess->sql("ROLLBACK;").execute();
  }
}

void db_connection::writePrices(const std::unordered_map<int,
  class price_update>& prices,
uint64_t timestamp) {
  sess->sql("START TRANSACTION;");
  std::string last_query = "START TRANSACTION;";
  try {
    for (auto [id, item] : prices) {
      std::string query = R"(REPLACE INTO osrs_market.price_update (ID, price_type, price, updated) VALUES ()";
      query+=std::to_string(id)+", ";
      query+="buy, ";
      query+=std::to_string(item.get_buy())+", ";
      query+=std::to_string(item.get_buy_timestamp())+");";
      last_query = query;
      sess->sql(query).execute();

      query = R"(REPLACE INTO osrs_market.price_update (ID, price_type, price, updated) VALUES ()";
      query+=std::to_string(id)+", ";
      query+="sell, ";
      query+=std::to_string(item.get_sell())+", ";
      query+=std::to_string(item.get_sell_timestamp())+");";
      last_query = query;
      sess->sql(query).execute();

      query = R"(REPLACE INTO osrs_market.price_series (ID, fetched, buy, sell) VALUES ()";
      query+=std::to_string(id)+", ";
      query+=std::to_string(timestamp)+", ";
      query+=std::to_string(item.get_buy_timestamp())+", ";
      query+=std::to_string(item.get_sell_timestamp())+");";
      last_query = query;
      sess->sql(query).execute();
    }
  } catch (std::exception& e) {
    std::cerr << "Error writing prices: " << e.what() << std::endl;
    std::cout << "Last query: " << last_query << std::endl;
    sess->sql("ROLLBACK;").execute();
  }
}
