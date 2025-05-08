// Copyright Landon Deam 2025

#include "utils.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <unordered_map>
#include <utility>
#include <vector>
#include "price_update.h"
#include "rapidjson/document.h"
#include "gumbo.h"

/**
* @brief URL-encodes a string.
* @param input The string to encode.
* @return The URL-encoded string.
*/
std::string utils::urlEncode(const std::string& input) {
  std::stringstream encoded;
  encoded.fill('0');
  encoded << std::hex;

  for (char c : input) {
      if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
          encoded << c;
      } else {
          encoded << std::uppercase;
          encoded << '%' << std::setw(2) << int((unsigned char)c);
          encoded << std::nouppercase;
      }
  }
  return encoded.str();
}

/**
* @brief Creates a directory if it doesn't exist.
* @param path The path to the directory.
*/
void utils::makeFolderIfNotExists(const std::string& path) {
  std::filesystem::path dir_path = path;
  if (!std::filesystem::exists(dir_path)) {
    if (std::filesystem::create_directory(dir_path)) {
      std::cout << "Directory create successfully: " << dir_path << std::endl;
    } else {
      std::cerr << "Failed to create directory: " << dir_path << std::endl;
    }
  } else {
    std::cout << "Directory already exists: " << dir_path << std::endl;
  }
}

/**
* @brief Reads the content of a text file.
* @param path The path to the file.
* @return The content of the file, or null if the file doesn't exist or is a directory.
*/
std::string utils::readTextFile(const std::string& path) {
  std::filesystem::path file_path = path;

  if (!std::filesystem::exists(file_path)) {
    std::cerr << "File or path does not exist: " << path << std::endl;
    return nullptr;
  }

  if (std::filesystem::is_directory(file_path)) {
    std::cerr << "Pointed to directory, not file: " << path  << std::endl;
    return nullptr;
  }

  try {
    std::ifstream file(file_path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
      std::cerr << "Could not open file " << path << std::endl;
      return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return nullptr;
}

/**
* @brief Takes a JSON response and parses it into an item map
* @param json The JSON response to parse
* @return An unordered map of item IDs to their corresponding item mappings
*/
const std::unordered_map<int, item_map> utils::item_maps(
  const std::string& json) {
  std::unordered_map<int, item_map> items;
  rapidjson::Document arr;
  arr.Parse(json.c_str());
  for (rapidjson::Value::ConstValueIterator itr = arr.Begin();
      itr != arr.End();
      ++itr) {
    int ID, value, lowalch, highalch, limit;
    std::string name, desc, icon;
    bool members;

    ID = (*itr)["id"].GetInt();
    name = (*itr)["name"].GetString();
    desc = (*itr)["examine"].GetString();
    icon = (*itr)["icon"].GetString();
    members = (*itr)["members"].GetBool();
    if (itr->HasMember("value")) {
      value = (*itr)["value"].GetInt();
    } else {
      value = -1;
    }
    if (itr->HasMember("highalch")) {
      highalch = (*itr)["highalch"].GetInt();
    } else {
      highalch = -1;
    }
    if (itr->HasMember("lowalch")) {
      lowalch = (*itr)["lowalch"].GetInt();
    } else {
      lowalch = -1;
    }
    if (itr->HasMember("limit")) {
      limit = (*itr)["limit"].GetInt();
    } else {
      limit = -1;
    }

    items.insert_or_assign(ID,
      item_map(ID, name, desc, icon, members, value, lowalch, highalch, limit));
  }
  return items;
}

/**
* @brief Takes a JSON response and parses it into item prices
* @param json The JSON response to parse
* @return An unordered map of item IDs to their corresponding price information
*/
const std::pair<std::unordered_map<int, price_update>, uint64_t>
  utils::item_prices(const std::string& json) {
  std::unordered_map<int, price_update> items;
  const uint64_t timestamp = std::chrono::duration_cast<
    std::chrono::seconds>(std::chrono::system_clock::now()
    .time_since_epoch()).count();

  rapidjson::Document parse;
  parse.Parse(json.c_str());

  for (auto const& p : parse["data"].GetObject()) {
    int ID = std::stoi(p.name.GetString());

    auto const& item_data = p.value.GetObject();
    const auto& low = item_data["low"];
    const auto& high = item_data["high"];
    const auto& lowTime = item_data["lowTime"];
    const auto& highTime = item_data["highTime"];

    items.insert_or_assign(ID, price_update(
      ID,
      low.IsInt() ? low.GetInt() : -1,
      high.IsInt() ? high.GetInt() : -1,
      lowTime.IsInt64() ? lowTime.GetInt64() : 0,
      highTime.IsInt64() ? highTime.GetInt64() : 0));
  }

  return std::make_pair(items, timestamp);
}

/**
* @brief Takes a JSON response and parses it into a list of drop sources
* @param json The JSON response to parse
* @return A vector containing all of the drop sources of a given item
*/
const std::vector<item_source> utils::item_sources(
  const std::string& json) {
  std::vector<item_source> items;
  // TODO(LandonDeam): Actually parse the JSON (and subsequent HTML) response

  rapidjson::Document parse;
  parse.Parse(json.c_str());

  if (!parse.HasMember("parse")) {
    return items;
  }

  const auto& parse_obj = parse["parse"].GetObject();

  if (!parse_obj.HasMember("text")) {
    return items;
  }

  const auto& text_obj = parse_obj["text"].GetObject();

  if (!text_obj.HasMember("*")) {
    return items;
  }

  const auto& html_obj = text_obj["*"];

  if (!html_obj.IsString()) {
    return items;
  }

  const auto& html = html_obj.GetString();

  std::string no_drops = R"(<div class=\"mw-parser-output\"><dl><dd><i>No drop sources found.)";

  if (std::string(html).starts_with(no_drops)) {
    return items;
  }

  GumboOutput* output = gumbo_parse(html);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return items;
}
