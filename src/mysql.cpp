// Copyright 2025 Landon Deam

#include "mysql.h"
#include <mysqlx/devapi/common.h>
#include <mysqlx/xdevapi.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include "utils.h"

void db_connection::init() {
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

void db_connection::build_schema() {
  // Build schema
  std::cout << "Building database schema..." << std::endl;
  sess->sql(utils::readTextFile("database/create_db.sql")).execute();
  // Create tables
  connect_tables();
}

void db_connection::build_price_series() {
  std::cout << "Building price series table..." << std::endl;
  try {
    sess->sql("START TRANSACTION;");
    mysqlx::SqlResult res = sess
      ->sql(utils::readTextFile("database/create_price_series.sql"))
      .execute();
    if (res.getWarningsCount() > 0) {
      std::cout << "Warning(s) while creating price series table:" << std::endl;
      for (mysqlx::Warning warn : res.getWarnings()) {
        std::cout << warn.getMessage() << std::endl;
      }
    }

    sess->sql("COMMIT;").execute();
  } catch (const mysqlx::Error& e) {
    std::cerr << "Error while creating price series table:"
              << e.what() << std::endl;
    sess->sql("ROLLBACK;").execute();
  }
}

void db_connection::build_price_update() {
  std::cout << "Building price update table..." << std::endl;
  try {
    sess->sql("START TRANSACTION;");
    mysqlx::SqlResult res = sess
      ->sql(utils::readTextFile("database/create_price_update.sql"))
      .execute();

    if (res.getWarningsCount() > 0) {
      std::cout << "Warning(s) while creating price update table:" << std::endl;
      for (mysqlx::Warning warn : res.getWarnings()) {
        std::cout << warn.getMessage() << std::endl;
      }
    }

    sess->sql("COMMIT;").execute();
  } catch (std::exception e) {
    std::cerr << "Error building price update table: " << e.what() << std::endl;
    sess->sql("ROLLBACK;").execute();
  }
}

void db_connection::build_item_map() {
  std::cout << "Building item map table..." << std::endl;
  try {
    sess->sql("START TRANSACTION;");
    mysqlx::SqlResult res = sess
      ->sql(utils::readTextFile("database/create_item_map.sql"))
      .execute();

    if (res.getWarningsCount() > 0) {
      std::cout << "Warning(s) while creating item map table:" << std::endl;
      for (mysqlx::Warning warn : res.getWarnings()) {
        std::cout << warn.getMessage() << std::endl;
      }
    }

    sess->sql("COMMIT;").execute();
  } catch (std::exception e) {
    std::cerr << "Error building item map table: " << e.what() << std::endl;
    sess->sql("ROLLBACK;").execute();
  }
}
