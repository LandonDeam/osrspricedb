// Copyright Landon Deam 2024

#pragma once
#ifndef ITEM_MAP_H_
#define ITEM_MAP_H_

#include <sstream>
#include <string>
#include "item.h"

class item_map : item {
 public:
  item_map(const int& ID, const std::string& name, const std::string& examine,
    const std::string& icon, const bool& members, const int& value,
    const int& lowalch = -1, const int& highalch = -1, const int& limit = -1);
  void update(const std::ostringstream& data);
 private:
  std::string name, examine, icon;
  bool members;
  int value;
  int lowalch, highalch;  // optional
  int limit;  // optional
};

#endif  // ITEM_MAP_H_
