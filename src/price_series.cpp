// Copyright Landon Deam 2024

#include "price_series.h"


/**
* @brief Represents a price series item.
* @param ID The unique identifier for the item.
* @param timestamp The timestamp of the last update.
* @param updated The last updated value.
*/
price_series::price_series(const int&  ID, const uint64_t& timestamp,
  const uint64_t& updated) : item(ID) {
  update(timestamp, updated);
}

/**
* @brief Updates the timestamp and updated value of the price series.
* @param timestamp The new timestamp.
* @param updated The new updated value.
*/
void price_series::update(const uint64_t& timestamp, const uint64_t& updated) {
  this->timestamp = timestamp;
  this->updated = updated;
}

/**
* @brief Gets the timestamp of the price series.
* @return The timestamp.
*/
uint64_t price_series::get_timestamp() {
  return this->timestamp;
}
