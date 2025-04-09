// Copyright 2025 Landon Deam

#pragma once
#ifndef MYSQL_H_
#define MYSQL_H_

#include <mysqlx/xdevapi.h>
#include <string>
#include <memory>

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
  inline static std::shared_ptr<mysqlx::Table> price_updated;
  inline static std::shared_ptr<mysqlx::Table> item_map;

  static void connect_tables();
  static void build_schema();
  static void build_price_series();
  static void build_price_updated();
  static void build_item_map();

 public:
  static void init();
  static void connect_db();
  static void close();
};

#endif  // MYSQL_H_
