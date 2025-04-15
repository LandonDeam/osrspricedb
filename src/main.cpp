// Copyright Landon Deam 2024

#include "main.h"
#include <string>
#include "env.h"
#include "mysql.h"
#include "get_api.h"

int main() {
  env::loadEnvVars(".env");
  db_connection::init();
  db_connection::connect_db();
  getter p("https://prices.runescape.wiki");
  p.get_prices();
  p.get_info();
  db_connection::close();
  return 0;
}
