// Copyright Landon Deam 2025

#include "utils.h"
#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <filesystem>
#include <regex>
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
#include "tidy.h"
#include "tidybuffio.h"
#include "pugixml.hpp"

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
  const std::string& json, const int& ID) {
  std::vector<item_source> items;

  try {
    rapidjson::Document parse;
    parse.Parse(json.c_str());

    if (!parse.HasMember("parse")) {
      std::cerr << "No parse object found " << std::endl;
      return items;
    }

    const auto& parse_obj = parse["parse"].GetObject();

    if (!parse_obj.HasMember("text")) {
      std::cerr << "No text field found " << std::endl;
      return items;
    }

    const auto& text_obj = parse_obj["text"].GetObject();

    if (!text_obj.HasMember("*")) {
      std::cerr << "Does not contain correct type of data " << std::endl;
      return items;
    }

    const auto& html_obj = text_obj["*"];

    if (!html_obj.IsString()) {
      std::cerr << "Data is in incorrect format " << std::endl;
      return items;
    }

    const auto& html = std::string(html_obj.GetString());

    const char* no_drops = R"(<div class="mw-parser-output"><dl><dd><i>No drop sources found. )";

    if (html.starts_with(no_drops)) {
      // std::cerr << "No drop source found " << std::endl;
      return items;
    }

    pugi::xml_document doc;

    std::string xml = tidy_html_to_xml(html);

    if (xml.empty()) {
      std::cerr << "Could not convert HTML to XHTML             " << std::endl;
      return items;
    }

    auto result = doc.load_string(xml.c_str());
    if (!result) {
      std::cerr << "Failed to convert " << ID << " to XML (likely bad data)   "
        << std::endl;
      std::cerr << result.description() << std::endl;
      std::cerr << xml << std::endl;
      return items;
    }

    for (auto&& row : doc.select_nodes("//table//tr")) {
      auto&& node = row.node();

      std::string source;
      bool noted = false;
      std::string skill;
      int quantity_min = 0, quantity_max = 0;
      float chance_min = 0.0f, chance_max = 0.0f;
      int rolls = 1;

      // std::cout << "Parsing source...    " << std::endl;
      auto source_td = node.select_node("td[1]").node();
      for (auto child : source_td.children()) {
          if (child.type() == pugi::node_pcdata) {
              source = child.value();  // First text node (e.g., "Gorak")
              break;
          }
      }
      if (source.empty()) {
          auto link = source_td.select_node("a");
          source = link.node().text().as_string();  // fallback
      }

      // std::cout << "Parsing skill...    " << std::endl;
      auto skill_td = node.select_node("td[2]//a");
      if (skill_td) {
        skill = skill_td.node().attribute("title").as_string();
      }

      // std::cout << "Parsing quantity...    " << std::endl;
      // Quantities: split on `;` andaccept numbers or ranges
      // (e.g., 100 or 100-200)
      std::string quantities_str =
        node.select_node("td[3]").node().text().as_string();
      std::stringstream qty_ss(quantities_str);
      std::string part;

      while (std::getline(qty_ss, part, ';')) {
          // Trim whitespace
          part.erase(0, part.find_first_not_of(" \t"));
          part.erase(part.find_last_not_of(" \t") + 1);

          // Check for " (noted)" suffix
          const std::string noted_suffix = " (noted)";
          if (part.size() >= noted_suffix.size() &&
              part.compare(part.size() - noted_suffix.size(),
                noted_suffix.size(), noted_suffix) == 0) {
              noted = true;
              part = part.substr(0, part.size() - noted_suffix.size());
          }

          // Generalized check for presence of digits
          bool has_digit = std::any_of(part.begin(), part.end(), ::isdigit);
          if (!has_digit) {
              quantity_min = -1;
              quantity_max = -1;
              continue;  // Skip further parsing
          }

          // Parse range or single number
          auto range = part.find('-');
          try {
              if (range != std::string::npos) {
                  int qmin = std::stoi(part.substr(0, range));
                  int qmax = std::stoi(part.substr(range + 1));

                  if (quantity_min > qmin || quantity_min == 0)
                      quantity_min = qmin;
                  if (quantity_max < qmax)
                      quantity_max = qmax;
              } else {
                  int q = std::stoi(part);
                  if (quantity_min > q || quantity_min == 0)
                      quantity_min = q;
                  if (quantity_max < q)
                      quantity_max = q;
              }
          } catch (const std::exception&) {
              quantity_min = -1;
              quantity_max = -1;
          }
      }

      // std::cout << "Parsing rarity...    " << std::endl;
      // Drop chances and multipliers
      auto drop_span = node.select_node("td[4]/span").node();
      std::string title = drop_span.attribute("title").as_string();
      std::string text = drop_span.text().as_string();

      // Normalize whitespace
      title.erase(std::remove_if(
        title.begin(), title.end(), ::isspace), title.end());

      // e.g., "2×2.02%", "2.34%", or "Always"
      std::regex drop_re(R"((\d+)[×xX*](\d+(\.\d+)?)(%?)|(\d+(\.\d+)?)(%?))");
      std::smatch match;

      if (!title.empty() && std::regex_match(title, match, drop_re)) {
          if (match[1].matched && match[2].matched) {
              // Has multiplier
              rolls = std::stoi(match[1]);
              float chance = std::stof(match[2]) / 100.0f;
              if (chance < chance_min || chance_min == 0.0f)
                chance_min = chance;
              if (chance > chance_max) chance_max = chance;
          } else if (match[5].matched) {
              // No multiplier
              float chance = std::stof(match[5]) / 100.0f;
              if (chance < chance_min || chance_min == 0.0f)
                chance_min = chance;
              if (chance > chance_max) chance_max = chance;
              rolls = 1;
          }
      } else {
          // Fallback
          float fallback_chance = -1.0f;
          if (text == "Always") {
              fallback_chance = 1.0f;
          }
          if (fallback_chance < chance_min || chance_min == 0.0f)
            chance_min = fallback_chance;
          if (fallback_chance > chance_max) chance_max = fallback_chance;
          rolls = 1;
      }

      items.push_back(item_source(
        ID,
        source,
        noted,
        skill,
        quantity_min, quantity_max,
        chance_min, chance_max,
        rolls));
    }
  } catch (std::exception& e) {
    std::cerr << "FAILED ON " << ID << std::endl;
    std::cerr << e.what() << std::endl;
  }

  return items;
}

const std::string utils::tidy_html_to_xml(const std::string& html_input) {
  TidyDoc tdoc = tidyCreate();
  TidyBuffer output = {0};
  TidyBuffer errbuf = {0};
  int rc = -1;

  // Set options
  tidyOptSetBool(tdoc, TidyXhtmlOut, yes);       // Output XHTML
  tidyOptSetBool(tdoc, TidyXmlOut, yes);         // Also tag it as XML
  tidyOptSetBool(tdoc, TidyQuiet, yes);          // No output
  tidyOptSetBool(tdoc, TidyForceOutput, yes);    // Force output even if errors
  tidyOptSetInt(tdoc, TidyWrapLen, 0);           // Don't wrap lines
  tidyOptSetValue(tdoc, TidyCharEncoding, "utf8");


  rc = tidySetErrorBuffer(tdoc, &errbuf);
  if (rc >= 0)
      rc = tidyParseString(tdoc, html_input.c_str());
  if (rc >= 0)
      rc = tidyCleanAndRepair(tdoc);
  if (rc >= 0)
      rc = tidySaveBuffer(tdoc, &output);

  std::string xml_output;
  if (rc >= 0) {
      xml_output.assign(reinterpret_cast<char*>(output.bp), output.size);
  } else {
      std::cerr << "Tidy failed:\n" << errbuf.bp << "\n";
  }

  tidyBufFree(&output);
  tidyBufFree(&errbuf);
  tidyRelease(tdoc);

  return xml_output;
}
