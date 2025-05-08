// Copyright 2025 Landon Deam

#pragma once
#include <cstdint>
#ifndef MYSQL_H_
#define MYSQL_H_

#include <mysqlx/xdevapi.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "price_update.h"
#include "item_map.h"
#include "item_source.h"

class db_connection {
 private:
  inline static bool initialized = false;

  inline static std::string username;
  inline static std::string password;
  inline static std::string port;
  inline static std::string host;
  inline static std::shared_ptr<mysqlx::Session> sess;
  inline static std::shared_ptr<mysqlx::Schema> db;
  inline static std::shared_ptr<mysqlx::Table> price_series;
  inline static std::shared_ptr<mysqlx::Table> price_update;
  inline static std::shared_ptr<mysqlx::Table> item_map;
  inline static std::shared_ptr<mysqlx::Table> item_source;

  static void connect_tables();
  static void build_schema();
  static void build_price_series();
  static void build_price_update();
  static void build_item_map();
  static void build_item_source();
  static void build_table(const std::string& table_name,
    const std::string& create_command);

 public:
  static void init();
  static void connect_db();
  static void close();

  static void writeItemMap(
    const std::unordered_map<int, class item_map>& items);
  static void writePrices(const std::unordered_map<int,
                            class price_update>& prices,
                          uint64_t timestamp);
  static void writeItemSources(
    const std::unordered_map<int, std::vector<class item_source>>& sources);
};

#endif  // MYSQL_H_
