// Copyright Landon Deam 2024

#include "item_map.h"
#include <string>

/**
 * @brief Initializes an item object.
 * 
 * @param ID Unique identifier of the item.
 * @param name Name of the item.
 * @param examine Description of the item.
 * @param icon Icon associated with the item.
 * @param members Whether the item is available only to members.
 * @param lowalch Low alchemy value of the item.
 * @param highalch High alchemy value of the item.
 * @param limit Maximum quantity of this item that can be held by a player.
 * @param value Value or price of the item.
 */
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
