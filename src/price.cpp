// Copyright Landon Deam 2024

#include "price.h"

price::price() : price(-1, 0) {}

price::price(int pri, uint32_t timestamp) {
  update(pri, timestamp);
}

void price::update(int pri, uint32_t timestamp) {
  this->pri = pri;
  this->timestamp = timestamp;
}
