// Copyright 2025 Landon Deam

#include "mysql.h"
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

    std::cout <<"Session accepted" << std::endl;
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }
}

void db_connection::close() {
  try {
    std::cout << "Attempting to close session..." << std::endl;
    sess->close();
    std::cout << "Session closed." << std::endl;
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }
}

void db_connection::connect_db() {
  db =
    std::make_shared<mysqlx::Schema>(sess->getSchema("osrs_market"));

  if (db->existsInDatabase()) {
    connect_tables();
  } else {
    build_schema();
  }
}

void db_connection::connect_tables() {
  item_map =
      std::make_shared<mysqlx::Table>(db->getTable("item_map"));
  if (!item_map->existsInDatabase()) {
    build_item_map();  // Create item_map table if it doesn't exist
  }

  price_updated =
    std::make_shared<mysqlx::Table>(db->getTable("price_updated"));
  if (!price_updated->existsInDatabase()) {
    build_price_updated();  // Create price_updated table if it doesn't exist
  }

  price_series =
    std::make_shared<mysqlx::Table>(db->getTable("price_series"));
  if (!price_series->existsInDatabase()) {
    build_price_series();  // Create price_series table if it doesn't exist
  }
}

void db_connection::build_schema() {
  // Build schema
  // Create tables
  connect_tables();
}

void db_connection::build_price_series() {
}

void db_connection::build_price_updated() {
}

void db_connection::build_item_map() {
}
