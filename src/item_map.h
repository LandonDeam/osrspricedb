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
  const std::string& getName() const {return name;}
  const std::string& getExamine() const {return examine;}
  const std::string& getIcon() const {return icon;}
  const bool& isMembers() const {return members;}
  const int& getValue() const {return value;}
  const int& getLowAlch() const {return lowalch;}
  const int& getHighAlch() const {return highalch;}
  const int& getLimit() const {return limit;}

 private:
  std::string name, examine, icon;
  bool members;
  int value;
  int lowalch, highalch;  // optional
  int limit;  // optional
};

#endif  // ITEM_MAP_H_
