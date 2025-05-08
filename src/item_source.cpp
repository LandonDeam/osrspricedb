// Copyright Landon Deam 2025

#include "item_source.h"
#include <string>

/**
* @brief Constructor for the item_source class.
* @param ID The unique identifier for the item.
* @param source The source of the item.
* @param noted Whether the item has been noted.
* @param skill The skill required to obtain the item.
* @param quantity_min The minimum quantity of the item found.
* @param quantity_max The maximum quantity of the item found.
* @param chance_min The minimum chance (percentage) of rolling the item.
* @param chance_max The maximum chance (percentage) of rolling the item.
* @param rolls The number of rolls you get.
*/
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
