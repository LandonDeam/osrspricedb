// Copyright Landon Deam 2024

#include "get_api.h"

/// @brief Creates a connection with a URI
/// @param uri URI to connect to
getter::getter(Poco::URI uri) {
  new_connection(uri);
}

/// @brief Creates a connectio to a URI represented with a string
/// @param str String representing a URI
getter::getter(const std::string str)
  : getter(Poco::URI(str)) {}

/// @brief Gets and parses data from the given endpoint, saving the response in
/// the debug file
/// @param ep Endpoint to connect to
/// @param debug File to save debug info to
/// @param type The type of data to receive and input int the item map
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
      responseFile << responseStr.str();
      item_map::update(responseStr, type);
    } else {
      std::cerr << "Error: " << response.getStatus()
                << " " << response.getReason() << std::endl;
      response.write(responseFile);
    }
    responseFile.close();
  } catch (Poco::Exception& e) {
    std::cerr << e.displayText() << std::endl;
  }
}

/// @brief Gets all of the prices
void getter::get_prices() {
  this->get("/api/v1/osrs/latest/", "debug/prices.json", "price");
}

/// @brief Gets all item info
void getter::get_info() {
  this->get("/api/v1/osrs/mapping/", "debug/mapping.json", "info");
}

/// @brief Generates and returns a pointer to an HTTP GET request
/// @param ep Which endpoint to point the request to
/// @return Returns an HTTP GET request pointer
Poco::Net::HTTPRequest* getter::generate_request(const std::string& ep) {
  Poco::Net::HTTPRequest *request =
      new Poco::Net::HTTPRequest(
        Poco::Net::HTTPRequest::HTTP_GET,
        ep,
        Poco::Net::HTTPRequest::HTTP_1_1);
    request->add(
      "User-Agent",
      "Landon Deam (landondeam@gmail.com)\n  Testing a GE tracker");
    return request;
}

/// @brief Creates a new HTTPS connection to the given URI
/// @param uri URI to create a connection with
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
