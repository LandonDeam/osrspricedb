// Copyright Landon Deam 2024

#include "price_series.h"


price_series::price_series(const int&  ID, const uint64_t& timestamp,
  const uint64_t& updated) : item(ID) {
  update(timestamp, updated);
}

void price_series::update(const uint64_t& timestamp, const uint64_t& updated) {
  this->timestamp = timestamp;
  this->updated = updated;
}
uint64_t price_series::get_timestamp() {
  return this->timestamp;
}
