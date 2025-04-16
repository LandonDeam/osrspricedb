// Copyright Landon Deam 2024

#include "item_map.h"
#include <string>


item_map::item_map(const int& ID, const std::string& name,
  const std::string& examine, const std::string& icon, const bool& members,
  const int& lowalch, const int& highalch, const int& limit, const int& value)
  : item(ID) {
    this->name = name;
    this->examine = examine;
    this->icon = icon;
    this->members = members;
    this->lowalch = lowalch;
    this->highalch = highalch;
    this->limit = limit;
    this->value = value;
}
