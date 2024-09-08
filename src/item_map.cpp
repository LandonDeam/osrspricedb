// Copyright Landon Deam 2024

#include "item_map.h"

/// @brief unordered map storing all items' current data
std::unordered_map<int, item> item_map::items;

/// @brief Uses `data` to update the item map with the given `type`
/// @param data `std::ostringstream` containing JSON data to parse
/// @param type `std::string` representing what is in `data`
void item_map::update(std::ostringstream& data, const std::string& type) {
  if (!type.compare("price")) {
    update_price(data);
  } else if (!type.compare("info")) {
    update_info(data);
  }
}

/// @brief Gets the `item` at the given `ID` in `item_map`
/// @param ID Integer representing the in-game ID of the desired item
/// @return `item` with the given `ID`
item item_map::get(int ID) {
  return items.at(ID);
}

/// @brief Ensures that an `item` with `ID` exists by creating it if it doesn't
/// @param ID ID to verify exists
void item_map::verifID(int ID) {
  if (items.find(ID) == items.end()) {
    items.emplace(ID, ID);
  }
}

/// @brief Updates the price of all `item`s
/// @param data `std::ostringstream` with JSON data to parse into price data
void item_map::update_price(std::ostringstream& data) {
  std::regex srch(R"#("(\d+)":\{"high":(\d+),"highTime":(\d+),"low":(\d+),"lowTime":(\d+)\})#",
    std::regex_constants::ECMAScript);
  std::string dat = data.str();
  std::sregex_iterator it(dat.begin(), dat.end(), srch), it_end;
  for (; it != it_end; ++it) {
    const std::smatch& match = *it;
    int ID = std::atoi(match.str(1).c_str());
    verifID(ID);
    item matched = items.at(ID);
    matched.update_price_data(
      std::atoi(match.str(2).c_str()),    // high
      std::atoi(match.str(4).c_str()),    // low
      std::atoll(match.str(3).c_str()),   // highTime
      std::atoll(match.str(5).c_str()));  // lowTime
  }
}

/// @brief Updates all of the mapping data for the items
/// @param data `std::ostringstream` with JSON data to parse into mapping data
void item_map::update_info(std::ostringstream& data) {
  std::regex srch(R"#(\{"examine":"(.+?)","id":(\d+),"members":(true|false),"lowalch":(\d+),"limit":(\d+),"value":(\d+),"highalch":(\d+),"icon":"(.+?)","name":"(.+?)"\})#",
    std::regex_constants::ECMAScript);
  std::string dat = data.str();
  std::sregex_iterator it(dat.begin(), dat.end(), srch), it_end;
  for (; it != it_end; ++it) {
    const std::smatch& match = *it;
    int ID = std::atoi(match.str(2).c_str());
    verifID(ID);
    item matched = items.at(ID);
    matched.update_info(
      match.str(1),
      match.str(3).compare("false"),
      std::atoi(match.str(4).c_str()),
      std::atoi(match.str(5).c_str()),
      std::atoi(match.str(6).c_str()),
      std::atoi(match.str(7).c_str()),
      match.str(8),
      match.str(9));
  }
}
