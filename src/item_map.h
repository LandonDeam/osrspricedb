// Copyright Landon Deam 2024

#pragma once
#ifndef ITEM_MAP_H_
#define ITEM_MAP_H_

#include <unordered_map>
#include <string>
#include <iostream>
#include <regex>
#include "item.h"

class item_map {
 public:
  static void update(std::ostringstream& data, const std::string& type);
  static item get(int ID);
 private:
  static std::unordered_map<int, item> items;
  static void verifID(int ID);
  static void update_price(std::ostringstream& data);
  static void update_info(std::ostringstream& data);
};

#endif  // ITEM_MAP_H_
