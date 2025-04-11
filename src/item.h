// Copyright Landon Deam 2024

#pragma once
#ifndef ITEM_H_
#define ITEM_H_

#include <string>
#include "price.h"

class item {
 public:
  explicit item(int ID);
  item(int ID,
    int high_price, int low_price,
    uint64_t time_high, uint64_t time_low);
  item(const std::string& examine, int ID, bool members, int lowalch,
    int limit, int value, int highalch, const std::string& icon,
    const std::string& name);
  void update_price_data(
    int high_price, int low_price,
    uint64_t time_high, uint64_t time_low);
  void update_info(const std::string& examine, bool members, int lowalch,
    int limit, int value, int highalch, const std::string& icon,
    const std::string& name);
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
  uint64_t time_last_update;
};

#endif  // ITEM_H_
