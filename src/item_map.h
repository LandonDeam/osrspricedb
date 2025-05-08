// Copyright Landon Deam 2025

#pragma once
#ifndef ITEM_MAP_H_
#define ITEM_MAP_H_

#include <string>
#include "item.h"

class item_map : public item {
 public:
  item_map(const int& ID, const std::string& name, const std::string& examine,
    const std::string& icon, const bool& members, const int& value,
    const int& lowalch = -1, const int& highalch = -1, const int& limit = -1);

  /**
  * @brief Returns the name of the item.
  * @return The name of the item.
  */
  const std::string& getName() const {return name;}

  /**
  * @brief Returns the examine description of the item.
  * @return The examine description.
  */
  const std::string& getExamine() const {return examine;}

  /**
  * @brief Returns the icon associated with the item.
  * @return The icon.
  */
  const std::string& getIcon() const {return icon;}

  /**
  * @brief Returns whether the item is only available to members.
  * @return Whether the item is only available to members.
  */
  const bool& isMembers() const {return members;}

  /**
  * @brief Returns the value or price of the item.
  * @return The value.
  */
  const int& getValue() const {return value;}

  /**
  * @brief Returns the low alchemy value of the item.
  * @return The lowalch value.
  */
  const int& getLowAlch() const {return lowalch;}

  /**
  * @brief Returns the high alchemy value of the item.
  * @return The highalch value.
  */
  const int& getHighAlch() const {return highalch;}

  /**
  * @brief Returns the maximum quantity that can be held by a player.
  * @return The limit.
  */
  const int& getLimit() const {return limit;}

 private:
  std::string name, examine, icon;
  bool members;
  int value;
  int lowalch, highalch;  // optional
  int limit;  // optional
};

#endif  // ITEM_MAP_H_
