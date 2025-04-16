// Copyright Landon Deam 2024

#pragma once
#ifndef PRICE_UPDATE_H_
#define PRICE_UPDATE_H_

#include <inttypes.h>
#include "item.h"

class price_update : item {
 public:
  price_update(const int& ID, const int& buy, const int& sell,
    const uint64_t& timestamp);
  void update(const int& buy, const int& sell, const uint64_t& timestamp);
  int get_buy();
  int get_sell();
  uint64_t get_timestamp();
 private:
  int buy;
  int sell;
  uint64_t timestamp;
};

#endif  // PRICE_UPDATE_H_
