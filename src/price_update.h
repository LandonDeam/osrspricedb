// Copyright Landon Deam 2024

#pragma once
#ifndef PRICE_UPDATE_H_
#define PRICE_UPDATE_H_

#include <inttypes.h>
#include "item.h"

class price_update : public item {
 public:
  price_update(const int& ID, const int& buy, const int& sell,
    const uint64_t& buy_timestamp, const uint64_t& sell_timestamp);
  void update_buy(const int& buy, const uint64_t& timestamp);
  void update_sell(const int& sell, const uint64_t& timestamp);
  const int& get_buy();
  const int& get_sell();
  const uint64_t& get_buy_timestamp();
  const uint64_t& get_sell_timestamp();
 private:
  int buy;
  int sell;
  uint64_t buy_timestamp;
  uint64_t sell_timestamp;
};

#endif  // PRICE_UPDATE_H_
