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

  const std::string& getSource() const {return source;}
  const bool& isNoted() const {return noted;}
  const std::string& getSkill() const {return skill;}
  const int& getQuantityMin() const {return quantity_min;}
  const int& getQuantityMax() const {return quantity_max;}
  const float& getChanceMin() const {return chance_min;}
  const float& getChanceMax() const {return chance_max;}
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
