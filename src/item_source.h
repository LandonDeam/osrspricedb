// Copyright Landon Deam 2025

#pragma once
#ifndef ITEM_SOURCE_H_
#define ITEM_SOURCE_H_

#include <string>
#include "item.h"

class item_source : public item {
 public:
  item_source(const int& ID, const std::string& source,
    const bool& noted, const std::string& skill,
    const int& quantity_min, const int& quantity_max,
    const float& chance_min, const float& chance_max,
    const int& rolls);

  /**
  * @brief Returns the source of the item.
  * @return The source.
  */
  const std::string& getSource() const {return source;}

  /**
  * @brief Returns whether the item has been noted.
  * @return Whether the item has been noted.
  */
  const bool& isNoted() const {return noted;}

  /**
  * @brief Returns the skill required to obtain the item.
  * @return The skill.
  */
  const std::string& getSkill() const {return skill;}


  /**
  * @brief Returns the minimum quantity of the item found.
  * @return The quantity_min.
  */
  const int& getQuantityMin() const {return quantity_min;}

  /**
  * @brief Returns the maximum quantity of the item found.
  * @return The quantity_max.
  */
  const int& getQuantityMax() const {return quantity_max;}

  /**
  * @brief Returns the minimum chance (percentage) of rolling the item.
  * @return The chance_min.
  */
  const float& getChanceMin() const {return chance_min;}

  /**
  * @brief Returns the maximum chance (percentage) of rolling the item.
  * @return The chance_max.
  */
  const float& getChanceMax() const {return chance_max;}

  /**
  * @brief Returns the number of rolls you get.
  * @return The rolls.
  */
  const int& getRolls() const {return rolls;}

 private:
  std::string source;
  bool noted;
  std::string skill;
  int quantity_min, quantity_max;
  float chance_min, chance_max;
  int rolls;
};

#endif  // ITEM_SOURCE_H
