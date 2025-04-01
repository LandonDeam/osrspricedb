// Copyright 2025 Landon Deam

#pragma once
#ifndef MYSQL_H_
#define MYSQL_H_

#include <iostream>
#include <string>
#include <cstdlib>

class db_connection {
 private:
  static std::string username;
  static std::string password;

 public:
  void init() {
    username = std::getenv("MYSQL_USERNAME");
    password = std::getenv("MYSQL_PASSWORD");
  }

};

#endif // MYSQL_H_