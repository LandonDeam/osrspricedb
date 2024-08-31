// Copyright Landon Deam 2024

#pragma once
#ifndef PRICE_H_
#define PRICE_H_

#include <inttypes.h>

class price {
 public:
  price();
  price(int pri, uint32_t timestamp);
  void update(int new_price, uint32_t timestamp);
  int get_price();
  uint32_t get_timestamp();
 private:
  int pri;
  uint32_t timestamp;
};

#endif  // PRICE_H_
