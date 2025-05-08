// Copyright Landon Deam 2025

#include "get_api.h"
#include <format>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Context.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "item_source.h"
#include "utils.h"
#include "mysql.h"

/// @brief Creates a connection with a URI
/// @param uri URI to connect to
getter::getter(Poco::URI uri) {
  new_connection(uri);
}

/**
* @brief Creates a connectio to a URI represented with a string
* @param str String representing a URI
*/
getter::getter(const std::string& str)
  : getter(Poco::URI(str)) {}

/**
* @brief Gets and parses data from the given endpoint, saving the response in
* the debug file
* @param ep Endpoint to connect to
* @param debug File to save debug info to
* @param type The type of data to receive and input int the item map
*/
void getter::get(const std::string& ep,
                 const std::string& debug,
                 const std::string& type) {
  try {
    Poco::Net::HTTPResponse response;
    Poco::Net::HTTPRequest* request = generate_request(ep);

    // Sending Request
    std::ostream& sentRequest = client.sendRequest(*request);

    // Getting Response and saving response
    std::istream& responseStream = client.receiveResponse(response);
    std::ofstream responseFile(debug);
    std::ostringstream responseStr;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
      Poco::StreamCopier::copyStream(responseStream, responseStr);
      if (type.compare("price") == 0) {
        const auto& [prices, time] = utils::item_prices(responseStr.str());
        db_connection::writePrices(prices, time);
      } else if (type.compare("info") == 0) {
        map = utils::item_maps(responseStr.str());
        db_connection::writeItemMap(map);
      } else if (type.compare("source") == 0) {
        std::vector<item_source> sources =
          utils::item_sources(responseStr.str());
        if (sources.size() > 0) {
          all_sources.insert_or_assign(sources.at(0).getID(), sources);
        }
      }
      responseFile << responseStr.str();
    } else {
      std::cerr << "Error: " << response.getStatus()
                << " " << response.getReason() << std::endl;
      response.write(responseFile);
    }
    responseFile.close();
  } catch (Poco::Exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

/**
* @brief Gets all of the prices
*/
void getter::get_prices() {
  std::cout << "Attempting to get price data from /api/v1/osrs/latest/"
    << std::endl;
  this->get("/api/v1/osrs/latest/", "debug/prices.json", "price");
}

/**
* @brief Gets all item info
*/
void getter::get_info() {
  std::cout << "Attempting to get item data from /api/v1/osrs/mapping/        "
    << std::endl;
  this->get("/api/v1/osrs/mapping/", "debug/mapping.json", "info");
}

/**
* @brief Gets all item sources
*/
void getter::get_sources() {
  std::cout << "Attempting to get item drops data from "
    << "MediaWiki endpoint /api.php" << std::endl;
  try {
    for (const auto& [ID, item] : map) {
      std::string endpoint = std::format(
        R"(/api.php?format=json&action=parse&text={}&title={}&disablelimitreport=true&contentmodel=wikitext&prop=text)",
        utils::urlEncode(
          std::format(
            R"({{{{Drop sources|{}|limit=100000|incrdt=y}}}})",
            item.getName())),
        utils::urlEncode(item.getName()));
      std::cout << "Attempting to get " << std::setw(6) << ID
        << " " << item.getName()<< "                         \r" << std::flush;
      get(endpoint, std::format("debug/sources/{}.json", ID), "source");
    }
    db_connection::writeItemSources(all_sources);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std::cout << "Finished getting sources.                       " << std::endl;
}

/**
* @brief Generates and returns a pointer to an HTTP GET request
* @param ep Which endpoint to point the request to
* @return Returns an HTTP GET request pointer
*/
Poco::Net::HTTPRequest* getter::generate_request(const std::string& ep) {
  Poco::Net::HTTPRequest *request =
      new Poco::Net::HTTPRequest(
        Poco::Net::HTTPRequest::HTTP_GET,
        ep,
        Poco::Net::HTTPRequest::HTTP_1_1);
    request->add(
      "User-Agent",
      "Landon Deam (landondeam@gmail.com)\n  Creating a GE tracker");
    return request;
}

/**
* @brief Creates a new HTTPS connection to the given URI
* @param uri URI to create a connection with
*/
void getter::new_connection(Poco::URI uri) {
  Poco::Net::initializeSSL();
  // TODO(Landon Deam): Need to make an actual certificate handler :P
  Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler =
    new Poco::Net::AcceptCertificateHandler(false);
  Poco::Net::Context::Ptr ptrContext =
    new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "");
  Poco::Net::SSLManager::instance()
    .initializeClient(0, ptrHandler, ptrContext);
  // Creating HTTPS Session
  new (&client) Poco::Net::HTTPSClientSession(
    uri.getHost(),
    uri.getPort(),
    ptrContext);
}

/**
* @brief Manages gathering item sources at a 24 hour interval
* @param g pointer to a `getter` object that is set up to pull data
*/
void getter::manage_sources(getter* g) {
  if (map.size() == 0) {
    std::cout << "No items to look for sources from." << std::endl;
    return;
  }

  while (true) {
    g->get_sources();
    sleep(60 * 60 * 24);  // 24 hours
  }
}

/**
* @brief Manages gathering price data at a 1 minute interval
* @param g pointer to a `getter` object that is set up to pull data
*/
void getter::manage_prices(getter* g) {
  while (true) {
    g->get_prices();
    std::cout << "Successfully wrote prices.                    " << std::endl;
    sleep(60);  // 1 minute
  }
}
