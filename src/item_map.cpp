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
    // std::cout << match.str() << std::endl;
    std::cout << "Match found:\n"
              << "Key: " << match.str(1) << "\n"
              << "High: " << match.str(2) << "\n"
              << "High Time: " << match.str(3) << "\n"
              << "Low: " << match.str(4) << "\n"
              << "Low Time: " << match.str(5) << std::endl;
  }
  // items[std::atoi()];
}

void item_map::update_info(std::ostringstream& data) {

}
