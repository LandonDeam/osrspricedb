// Copyright Landon Deam 2025

#include "main.h"
#include <iostream>
#include <string>
#include <thread>
#include "signal.h"
#include "env.h"
#include "mysql.h"
#include "get_api.h"

int main() {
  // Handler for signal interrupts
  signal(SIGINT, [](int signum) {
    std::cout << "Received SIGINT. Exiting..." << std::endl;
    db_connection::close();
    exit(0);
  });

  // Creating SQL Connection
  env::loadEnvVars(".env");
  db_connection::init();
  db_connection::connect_db();

  // Creating objects for talking to the wiki's endpoints
  getter price_getter("https://prices.runescape.wiki");
  getter source_getter("https://oldschool.runescape.wiki/");

  // Getting baseline item info for creating main table
  price_getter.get_info();

  // Create threads for getting the prices every minute, and sources every
  // 24 hours
  std::thread prices(&getter::manage_prices, &price_getter);
  std::thread sources(&getter::manage_sources, &source_getter);

  // Wait on threads to finish executing
  prices.join();
  sources.join();

  // Close connection and return if threads finish executing successfully
  db_connection::close();
  return 0;
}
