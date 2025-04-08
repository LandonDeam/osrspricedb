// Copyright Landon Deam 2024

#include "main.h"
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include "env.h"
#include "mysql.h"

int main() {
  env::loadEnvVars(".env");
  db_connection::init();
  getter p("https://prices.runescape.wiki");
  p.get_prices();
  p.get_info();
  db_connection::close();
  return 0;
}
