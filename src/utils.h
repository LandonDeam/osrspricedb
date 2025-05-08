// Copyright 2025 Landon Deam

#pragma once
#include <cstdint>
#ifndef UTILS_H_
#define UTILS_H_

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
  static const std::vector<std::unordered_map<std::string, std::string>>
    json_arr_parse(const std::string& input);
  static void erase_all(std::string* str, char c);
  static std::vector<std::string> split(const std::string &s, char delim);
  static std::vector<std::string> split_enclosed_inclusive(const std::string& s,
    char delimiter, char enclosure);
  static std::vector<std::string> split_enclosed_inclusive(const std::string& s,
    char delimiter, char start_enclosure,
    char end_enclosure);
  static std::vector<std::string> split_enclosed_exclusive(const std::string& s,
    char delimiter, char enclosure);
  static std::vector<std::string> split_enclosed_exclusive(const std::string& s,
    char delimiter, char start_enclosure,
    char end_enclosure);
  static const inline int GetIntFromStringIfNot(const std::string& str,
    const std::string& not_str, const int& ordinal = -1);
  static const inline uint64_t GetUInt64FromStringIfNot(const std::string& str,
    const std::string& not_str, const uint64_t& ordinal = 0);
};

#endif  // UTILS_H_
