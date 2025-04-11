// Copyright Landon Deam 2024

#pragma once
#ifndef ITEM_MAP_H_
#define ITEM_MAP_H_

#include <unordered_map>
#include <string>
#include <sstream>
#include "item.h"

class item_map {
 public:
  static void update(const std::ostringstream& data, const std::string& type);
  static item get(int ID);
 private:
  static std::unordered_map<int, item> items;
  static void verifID(int ID);
  static void update_price(const std::ostringstream& data);
  static void update_info(const std::ostringstream& data);
};

#endif  // ITEM_MAP_H_
