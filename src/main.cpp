// Copyright Landon Deam 2024

#include "main.h"
#include <string>
#include <thread>
#include "env.h"
#include "mysql.h"
#include "get_api.h"

int main() {
  env::loadEnvVars(".env");
  db_connection::init();
  db_connection::connect_db();
  getter price_getter("https://prices.runescape.wiki");
  getter source_getter("https://oldschool.runescape.wiki/");
  price_getter.get_info();  // Make sure to get info first for SQL tables

  std::thread prices(&getter::manage_prices, std::ref(price_getter));
  std::thread sources(&getter::manage_sources, std::ref(source_getter));
  prices.join();
  sources.join();
  db_connection::close();
  return 0;
}
