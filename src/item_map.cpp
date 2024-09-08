// Copyright Landon Deam 2024

#include "item_map.h"

std::map<int, item> item_map::items;

void item_map::update(std::ostringstream& data, const std::string& type) {
  if (!type.compare("price")) {
    update_price(data);
  } else if (!type.compare("info")) {
    update_info(data);
  }
}

item item_map::get(int ID) {
  return items.at(ID);
}

void item_map::update_price(std::ostringstream& data) {
  std::regex srch(R"#("(\d+)":\{"high":(\d+),"highTime":(\d+),"low":(\d+),"lowTime":(\d+)\})#",
    std::regex_constants::ECMAScript);
  std::string dat = data.str();
  std::sregex_iterator it(dat.begin(), dat.end(), srch), it_end;
  for (; it != it_end; ++it) {
    const std::smatch& match = *it;
    int ID = std::atoi(match.str(1).c_str());
    if (items.find(ID) == items.end()) {
      items.emplace(ID, ID);
    }
    item matched = items.at(ID);
    matched.update_price_data(
      std::atoi(match.str(2).c_str()),
      std::atoi(match.str(4).c_str()),
      std::atoi(match.str(3).c_str()),
      std::atoi(match.str(5).c_str()));
  }
}

void item_map::update_info(std::ostringstream& data) {

}
