// Copyright Landon Deam 2025

#pragma once
#ifndef ITEM_SOURCE_H_
#define ITEM_SOURCE_H_

#include <string>
#include "item.h"

class item_source : item {
 public:
  item_source(const int& ID, const std::string& source,
    const bool& noted, const std::string& skill,
    const int& quantity_min, const int& quantity_max,
    const float& chance_min, const float& chance_max,
    const int& rolls);

  const std::string& GetSource();
  const bool& IsNoted();
  const std::string& GetSkill();
  const int& GetQuantityMin();
  const int& GetQuantityMax();
  const float& GetChanceMin();
  const float& GetChanceMax();
  const int& GetRolls();

 private:
  std::string source;
  bool noted;
  std::string skill;
  int quantity_min, quantity_max;
  float chance_min, chance_max;
  int rolls;
};

#endif  // ITEM_SOURCE_H
