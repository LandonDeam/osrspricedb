// Copyright Landon Deam 2025

#pragma once
#ifndef GET_API_H_
#define GET_API_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/URI.h"

class getter {
 public:
  explicit getter(Poco::URI uri);

  explicit getter(const std::string& str);

  void get_prices();

  static void manage_prices(getter* g);

  void get_info();

  void get_sources();

  static void manage_sources(getter* g);

 private:
  void get(const std::string& ep,
           const std::string& debug,
           const std::string& type);

  Poco::Net::HTTPSClientSession client;

  Poco::Net::HTTPRequest* generate_request(const std::string& ep);

  void new_connection(Poco::URI uri);

  static inline std::unordered_map<int, class item_map> map;

  static inline
    std::unordered_map<int, std::vector<class item_source>> all_sources;
};

#endif  // GET_API_H_
