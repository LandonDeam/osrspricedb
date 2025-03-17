// Copyright Landon Deam 2024

#include "item.h"

item::item(int ID) {
  this->ID = ID;
}

item::item(int ID, int high_price, int low_price,
 uint64_t time_high, uint64_t time_low) : item(ID) {
  update_price_data(high_price, low_price, time_high, time_low);
}

item::item(std::string examine, int ID, bool members, int lowalch,
 int limit, int value, int highalch, std::string icon, std::string name)
: item(ID) {
  update_info(examine, members, lowalch, limit, value, highalch, icon, name);
}

void item::update_info(std::string examine, bool members, int lowalch,
  int limit, int value, int highalch, std::string icon, std::string name) {
    this->examine = examine;
    this->members = members;
    this->lowalch = lowalch;
    this->limit = limit;
    this->value = value;
    this->highalch = highalch;
    this->icon = icon;
    this->name = name;
}

void item::update_price_data(int high_price, int low_price,
 uint64_t time_high, uint64_t time_low) {
    this->low.update(low_price, time_high);
    this->high.update(high_price, time_high);
    this->has_prices = true;
}

int item::getID() {
  return this->ID;
}

