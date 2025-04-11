// Copyright Landon Deam 2024

#include "price.h"

price::price() : price(-1, 0) {}

price::price(int pri, uint64_t timestamp) {
  update(pri, timestamp);
}

void price::update(int pri, uint64_t timestamp) {
  this->pri = pri;
  this->timestamp = timestamp;
}

int price::get_price() {
  return this->pri;
}

uint64_t price::get_timestamp() {
  return this->timestamp;
}
