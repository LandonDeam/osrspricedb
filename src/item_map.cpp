// Copyright Landon Deam 2024

#include "item_map.h"

std::map<int, item> item_map::items;

void item_map::update(std::ostream& data, const std::string& type) {
  if (!type.compare("price")) {
    update_price(data);
  } else if (!type.compare("info")) {
    update_info(data);
  }
}

item item_map::get(int ID) {
  return items.at(ID);
}

void item_map::update_price(std::ostream& data) {

}

void item_map::update_info(std::ostream& data) {

}
