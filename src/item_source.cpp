// Copyright Landon Deam 2025

#include "item_source.h"
#include <string>

item_source::item_source(const int& ID, const std::string& source,
  const bool& noted, const std::string& skill,
  const int& quantity_min, const int& quantity_max,
  const float& chance_min, const float& chance_max,
  const int& rolls) : item(ID) {
  this->source = source;
  this->noted = noted;
  this->skill = skill;
  this->quantity_min = quantity_min;
  this->quantity_max = quantity_max;
  this->chance_min = chance_min;
  this->chance_max = chance_max;
  this->rolls = rolls;
}
