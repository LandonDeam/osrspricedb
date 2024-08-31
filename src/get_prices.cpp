// Copyright Landon Deam 2024

#include "get_prices.h"

/// @brief Creates a connection with a URI
/// @param uri URI to connect to
price_getter::price_getter(Poco::URI uri) {
  new_connection(uri);
}

/// @brief Creates a connectio to a URI represented with a string
/// @param str String representing a URI
price_getter::price_getter(const std::string str)
  : price_getter(Poco::URI(str)) {}

/// @brief Gets all of the prices
void price_getter::get_prices() {
  try {
    Poco::Net::HTTPResponse response;
    Poco::Net::HTTPRequest* request = generate_request();

    // Sending Request
    std::ostream& sentRequest = client.sendRequest(*request);

    // Getting Response and saving response
    std::istream& responseStream = client.receiveResponse(response);
    std::ofstream responseFile("response.txt");
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
      Poco::StreamCopier::copyStream(responseStream, responseFile);
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

/// @brief Generates and returns a pointer to an HTTP GET request
/// @return Returns an HTTP GET request pointer
Poco::Net::HTTPRequest* price_getter::generate_request() {
  Poco::Net::HTTPRequest *request =
      new Poco::Net::HTTPRequest(
        Poco::Net::HTTPRequest::HTTP_GET,
        "/api/v1/osrs/latest/",
        Poco::Net::HTTPRequest::HTTP_1_1);
    request->add(
      "User-Agent",
      "Landon Deam (landondeam@gmail.com)\n  Testing a GE tracker");
    return request;
}

/// @brief Creates a new HTTPS connection to the given URI
/// @param uri URI to create a connection with
void price_getter::new_connection(Poco::URI uri) {
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
