// Copyright Landon Deam 2024

#pragma once
#ifndef PRICE_H_
#define PRICE_H_

#include <inttypes.h>

class price {
 public:
  price();
  price(int pri, uint64_t timestamp);
  void update(int new_price, uint64_t timestamp);
  int get_price();
  uint64_t get_timestamp();
 private:
  int pri;
  uint64_t timestamp;
};

#endif  // PRICE_H_
