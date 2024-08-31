// Copyright Landon Deam 2024

#pragma once
#ifndef ITEM_H_
#define ITEM_H_

#include <string>
#include "price.h"

class item {
 public:
  item(int ID,
    int high_price, int low_price,
    uint32_t time_high, uint32_t time_low);
  item(std::string examine, int ID, bool members, int lowalch,
    int limit, int value, int highalch, std::string icon, std::string name);
  void update_price_data(
    int high_price, int low_price,
    uint32_t time_high, uint32_t time_low);
  void update_info(std::string examine, bool members, int lowalch,
    int limit, int value, int highalch, std::string icon, std::string name);
  int getID();
 private:
  int ID;
  price low, high;
  bool has_prices = false;
  bool has_info = false;
  std::string name, examine, icon;
  bool members;
  int lowalch, highalch;
  int limit;
  int value;
};

#endif  // ITEM_H_
