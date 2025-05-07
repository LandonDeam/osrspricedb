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
  /**
  * @brief Returns the buy price.
  * @return The buy price.
  */
  const int& get_buy() const {return buy;}
  /**
  * @brief Returns the sell price.
  * @return The sell price.
  */
  const int& get_sell() const {return sell;}
  /**
  * @brief Returns the buy timestamp.
  * @return The buy timestamp.
  */
  const uint64_t& get_buy_timestamp() const {return buy_timestamp;}
  /**
  * @brief Returns the sell timestamp.
  * @return The sell timestamp.
  */
  const uint64_t& get_sell_timestamp() const {return sell_timestamp;}

 private:
  int buy;
  int sell;
  uint64_t buy_timestamp;
  uint64_t sell_timestamp;
};

#endif  // PRICE_UPDATE_H_
