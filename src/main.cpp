// Copyright Landon Deam 2024

#include "main.h"

int main() {
  price_getter p("https://prices.runescape.wiki");
  p.get_prices();
  return 0;
}
