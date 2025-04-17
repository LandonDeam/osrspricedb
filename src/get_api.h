// Copyright Landon Deam 2024

#pragma once
#ifndef GET_PRICES_H_
#define GET_PRICES_H_

#include <string>
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/URI.h"

class getter {
 public:
  explicit getter(Poco::URI uri);
  explicit getter(const std::string& str);
  void get_prices();
  void get_info();
 private:
  void get(const std::string& ep,
           const std::string& debug,
           const std::string& type);
  Poco::Net::HTTPSClientSession client;
  Poco::Net::HTTPRequest* generate_request(const std::string& ep);
  void new_connection(Poco::URI uri);
};

#endif  // GET_PRICES_H_
