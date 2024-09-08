// Copyright Landon Deam 2024

#include "main.h"

int main() {
  getter p("https://prices.runescape.wiki");
  p.get_prices();
  p.get_info();
  return 0;
}
