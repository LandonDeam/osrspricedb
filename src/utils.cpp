// Copyright 2025 Landon Deam

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
#include <algorithm>
#include "price_update.h"

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
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }

  return nullptr;
}

const std::unordered_map<int, item_map>
  utils::item_maps(const std::string& json) {
  std::unordered_map<int, item_map> items;
  std::vector<std::unordered_map<std::string, std::string>> raw =
  json_arr_parse(json);
  for (const auto& item : raw) {
    int ID = std::stoi(item.at("id"));
    std::string name = item.at("name");
    std::string desc = item.at("examine");
    std::string icon = item.at("icon");
    int value = std::stoi(item.at("value"));
    bool members = item.at("members").compare("true") == 0;
    if (item.find("lowalch") == item.end()) {
      if (item.find("limit") == item.end()) {
        items.insert_or_assign(ID,
          item_map(ID, name, desc, icon, members, value));
      } else {
        int limit = std::stoi(item.at("limit"));
        items.insert_or_assign(ID,
          item_map(ID, name, desc, icon, members, value, -1, -1, limit));
      }
    } else {
      int lowalch = std::stoi(item.at("lowalch"));
      int highalch = std::stoi(item.at("highalch"));
      if (item.find("limit") == item.end()) {
        items.insert_or_assign(ID,
          item_map(ID, name, desc, icon,
                              members, value, lowalch, highalch));
      } else {
        int limit = std::stoi(item.at("limit"));
        items.insert_or_assign(ID,
          item_map(ID, name, desc, icon, members, value,
                              lowalch, highalch, limit));
      }
    }
  }
  return items;
}

const std::pair<std::unordered_map<int, price_update>, uint64_t>
  utils::item_prices(const std::string& json) {
  std::unordered_map<int, price_update> items;
  const uint64_t timestamp = std::chrono::duration_cast<
    std::chrono::seconds>(std::chrono::system_clock::now()
    .time_since_epoch()).count();

  // std::cout << "Parsing item price JSON" << "\n";
  std::string parse = json;
  size_t start = parse.find("{\"data\":{");
  parse.erase(start, start+9);
  size_t end = parse.find_last_of("}}");
  parse.erase(end);
  std::vector<std::string> items_json =
    split_enclosed_inclusive(parse, ',', '{', '}');
  // std::cout << "Iterating over items in JSON" << std::endl;
  for (const auto& item : items_json) {
    std::vector<std::string> id_kv =
      split_enclosed_exclusive(item, ':', '{', '}');
    std::string id = id_kv[0];
    erase_all(&id, '"');
    // std::cout << "ID: " << id << std::endl;
    int ID = std::stoi(id);
    std::unordered_map<std::string, std::string> kv;
    std::vector<std::string> price_data =
      split_enclosed_exclusive(id_kv[1], ',', '"');
    // std::cout << "Parsing price data" << std::endl;
    for (const auto& data : price_data) {
      // std::cout << "Parsing KV pair " << data << std::endl;
      std::vector<std::string> data_kv = split(data, ':');
      kv.insert_or_assign(data_kv[0], data_kv[1]);
    }
    // std::cout << "Inserting data" << std::endl;
    items.insert_or_assign(ID, price_update(
      ID,
      kv.at("low").compare("null") == 0 ? -1 : std::stoi(kv.at("low")),
      kv.at("high").compare("null") == 0 ? -1 : std::stoi(kv.at("high")),
      kv.at("lowTime").compare("null") == 0 ? 0 : std::stoull(kv.at("lowTime")),
      kv.at("highTime").compare("null") == 0 ?
        0 : std::stoull(kv.at("highTime"))));
  }

  return std::make_pair(items, timestamp);
}

const std::vector<std::unordered_map<std::string, std::string>>
  utils::json_arr_parse(const std::string& input) {
  std::vector<std::unordered_map<std::string, std::string>> arr;
  std::string parse = input;
  erase_all(&parse, '[');
  erase_all(&parse, ']');
  std::vector<std::string> tokens = split(parse, '}');
  for (auto token : tokens) {
    erase_all(&token, '{');
    if (token.compare("") == 0) {
      continue;
    }
    std::unordered_map<std::string, std::string> data;
    std::vector<std::string> kv_pairs =
      split_enclosed_inclusive(token, ',', '"');
    for (auto kv_pair : kv_pairs) {
      if (kv_pair.compare("") == 0) {
        continue;
      }
      std::vector<std::string> kv = split_enclosed_exclusive(kv_pair, ':', '"');
      if (kv.size() != 2) {
        continue;
      }
      data.insert_or_assign(kv[0], kv[1]);
    }
    arr.push_back(data);
  }
  return arr;
}

void utils::erase_all(std::string* str, char c) {
  str->erase(std::remove(str->begin(), str->end(), c), str->end());
}

std::vector<std::string> utils::split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;

  while (getline(ss, item, delim)) {
      result.push_back(item);
  }

  return result;
}

std::vector<std::string> utils::split_enclosed_inclusive(const std::string& s,
                                        char delimiter, char enclosure) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool inside_enclosure = false;

    for (char c : s) {
        if (c == enclosure) {
            inside_enclosure = !inside_enclosure;
            current_token += c;
        } else if (c == delimiter && !inside_enclosure) {
            tokens.push_back(current_token);
            current_token = "";
        } else {
            current_token += c;
        }
    }
    tokens.push_back(current_token);
    return tokens;
}

std::vector<std::string> utils::split_enclosed_inclusive(const std::string& s,
                                        char delimiter, char start_enclosure,
                                        char end_enclosure) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool inside_enclosure = false;

    for (char c : s) {
        if (c == start_enclosure) {
            inside_enclosure = true;
            current_token += c;
        } else if (c == end_enclosure) {
            inside_enclosure = false;
            current_token += c;
        } else if (c == delimiter && !inside_enclosure) {
            tokens.push_back(current_token);
            current_token = "";
        } else {
            current_token += c;
        }
    }
    tokens.push_back(current_token);
    return tokens;
}

std::vector<std::string> utils::split_enclosed_exclusive(const std::string& s,
                                        char delimiter, char enclosure) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool inside_enclosure = false;

    for (char c : s) {
        if (c == enclosure) {
            inside_enclosure = !inside_enclosure;
        } else if (c == delimiter && !inside_enclosure) {
            tokens.push_back(current_token);
            current_token = "";
        } else {
            current_token += c;
        }
    }
    tokens.push_back(current_token);
    return tokens;
}

std::vector<std::string> utils::split_enclosed_exclusive(const std::string& s,
                                        char delimiter, char start_enclosure,
                                        char end_enclosure) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool inside_enclosure = false;

    for (char c : s) {
        if (c == start_enclosure && !inside_enclosure) {
            inside_enclosure = true;
        } else if (c == end_enclosure && inside_enclosure) {
            inside_enclosure = false;
        } else if (c == delimiter && !inside_enclosure) {
            tokens.push_back(current_token);
            current_token = "";
        } else {
            current_token += c;
        }
    }
    tokens.push_back(current_token);
    return tokens;
}
