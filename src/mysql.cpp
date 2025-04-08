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
