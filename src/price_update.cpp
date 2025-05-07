// Copyright Landon Deam 2024

#include "price_update.h"

/**
* @brief Represents a price update for an item.
* @param ID The unique identifier for the item.
* @param buy The buy price for the item.
* @param sell The sell price for the item.
* @param buy_timestamp The timestamp of the buy update.
* @param sell_timestamp The timestamp of the sell update.
*/
price_update::price_update(const int&  ID, const int& buy, const int& sell,
  const uint64_t& buy_timestamp, const uint64_t& sell_timestamp) : item(ID) {
    update_buy(buy, buy_timestamp);
    update_sell(sell, sell_timestamp);
}

/**
* @brief Updates the buy price and timestamp.
* @param buy The new buy price.
* @param timestamp The timestamp of the update.
*/
void price_update::update_buy(const int& buy, const uint64_t& timestamp) {
  this->buy = buy;
  this->buy_timestamp = timestamp;
}

/**
* @brief Updates the sell price and timestamp.
* @param sell The new sell price.
* @param timestamp The timestamp of the update.
*/
void price_update::update_sell(const int& sell, const uint64_t& timestamp) {
  this->sell = sell;
  this->sell_timestamp = timestamp;
}
