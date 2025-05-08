// Copyright Landon Deam 2025

#pragma once
#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "price_update.h"
#include "item_map.h"
#include "item_source.h"

class utils {
 public:
  static std::string urlEncode(const std::string& input);

  static void makeFolderIfNotExists(const std::string& path);

  static std::string readTextFile(const std::string& path);

  static const std::unordered_map<int, item_map> item_maps(
    const std::string& json);

  static const std::pair<std::unordered_map<int, price_update>, uint64_t>
    item_prices(const std::string& json);

  static const std::vector<item_source> item_sources(
    const std::string& json);
};

#endif  // UTILS_H_
