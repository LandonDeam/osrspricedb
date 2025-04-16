// Copyright Landon Deam 2024

#include "price_update.h"


price_update::price_update(const int&  ID, const int& buy, const int& sell,
  const uint64_t& timestamp) : item(ID) {
  update(buy, sell, timestamp);
}

void price_update::update(const int& buy, const int& sell,
    const uint64_t& timestamp) {
  this->buy = buy;
  this->sell = sell;
  this->timestamp = timestamp;
}

int price_update::get_buy() {
  return this->buy;
}

int price_update::get_sell() {
  return this->buy;
}

uint64_t price_update::get_timestamp() {
  return this->timestamp;
}
