// Copyright 2025 Landon Deam

#include "mysql.h"
#include <mysqlx/xdevapi.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

void db_connection::init() {
  username = std::getenv("MYSQL_USERNAME");
  password = std::getenv("MYSQL_PASSWORD");
  port = std::getenv("MYSQL_PORT");
  host = std::getenv("MYSQL_HOST");

  try {
    std::string url("mysqlx://"+username+":"+password+"@"+port+":"+host);
    std::cout << "Creating session..."
              << "mysqlx://"+username+":"+"[PASSWORD]"+"@"+port+":"+host
              << "..." << std::endl;

    sess = std::make_shared<mysqlx::Session>(url);

    std::cout <<"Session accepted" << std::endl;
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }
}
