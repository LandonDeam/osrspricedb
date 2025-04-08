// Copyright 2025 Landon Deam

#pragma once
#ifndef MYSQL_H_
#define MYSQL_H_

#include <mysqlx/xdevapi.h>
#include <string>
#include <memory>

class db_connection {
 private:
  inline static std::string username;
  inline static std::string password;
  inline static std::string port;
  inline static std::string host;
  inline static std::shared_ptr<mysqlx::Session> sess;

 public:
  static void init();
};

#endif  // MYSQL_H_
