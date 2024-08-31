// Copyright Landon Deam 2024

#pragma once
#ifndef GET_PRICES_H_
#define GET_PRICES_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <map>
#include <tuple>
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Context.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"

#include "item.h"

class price_getter {
 public:
  std::map<int, item> prices;
  explicit price_getter(Poco::URI uri);
  explicit price_getter(const std::string str);
  void get_prices();
 private:
  Poco::Net::HTTPSClientSession client;
  Poco::Net::HTTPRequest* generate_request();
  void new_connection(Poco::URI uri);
};

#endif  // GET_PRICES_H_
