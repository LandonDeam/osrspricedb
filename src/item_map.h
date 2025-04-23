// Copyright Landon Deam 2024

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
  const std::string& getName() {return name;}
  const std::string& getExamine() {return examine;}
  const std::string& getIcon() {return icon;}
  const bool& isMembers() {return members;}
  const int& getValue() {return value;}
  const int& getLowAlch() {return lowalch;}
  const int& getHighAlch() {return highalch;}
  const int& getLimit() {return limit;}

 private:
  std::string name, examine, icon;
  bool members;
  int value;
  int lowalch, highalch;  // optional
  int limit;  // optional
};

#endif  // ITEM_MAP_H_
