// Copyright Landon Deam 2024

#include "price_update.h"

/**
* @brief Represents a price update for an item.
* @param ID The unique identifier for the item.
* @param buy The buy price for the item.
* @param sell The sell price for the item.
* @param timestamp The timestamp of the update.
*/
price_update::price_update(const int&  ID, const int& buy, const int& sell,
  const uint64_t& timestamp) : item(ID) {
  update(buy, sell, timestamp);
}

/**
* @brief Updates the buy and sell prices and timestamp.
* @param buy The new buy price.
* @param sell The new sell price.
* @param timestamp The timestamp of the update.
*/
void price_update::update(const int& buy, const int& sell,
    const uint64_t& timestamp) {
  this->buy = buy;
  this->sell = sell;
  this->timestamp = timestamp;
}

/**
* @brief Returns the buy price.
* @return The buy price.
*/
int price_update::get_buy() {
  return this->buy;
}

/**
* @brief Returns the sell price.
* @return The sell price.
*/
int price_update::get_sell() {
  return this->buy;
}

/**
* @brief Returns the timestamp.
* @return The timestamp.
*/
uint64_t price_update::get_timestamp() {
  return this->timestamp;
}
