// Copyright Landon Deam 2024

#include "headers/main.h"

int main() {
  try {
    // Initializing variables
    // URI to connect to
    const Poco::URI uri("https://prices.runescape.wiki");
    // HTTP Response
    Poco::Net::HTTPResponse response;
    // Initialize for SSL connection
    Poco::Net::initializeSSL();
    // TODO(Landon Deam): Need to make an actual certificate handler :P
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler =
      new Poco::Net::AcceptCertificateHandler(false);
    Poco::Net::Context::Ptr ptrContext =
      new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "");
    Poco::Net::SSLManager::instance()
      .initializeClient(0, ptrHandler, ptrContext);
    // Creating HTTPS Session
    Poco::Net::HTTPSClientSession client(
      uri.getHost(),
      uri.getPort(),
      ptrContext);
    std::cout << "Created HTTPS Session" << std::endl;

    // Creating Request
    Poco::Net::HTTPRequest *request =
      new Poco::Net::HTTPRequest(
        Poco::Net::HTTPRequest::HTTP_GET,
        "/api/v1/osrs/latest/",
        Poco::Net::HTTPRequest::HTTP_1_1);
    request->add(
      "User-Agent",
      "Landon Deam (landondeam@gmail.com)\n  Testing a GE tracker");

    // Sending Request
    std::ostream& sentRequest = client.sendRequest(*request);
    std::ofstream requestFile("request.txt");
    request->write(requestFile);
    requestFile.close();
    std::cout << "Saved Request" << std::endl;

    // Getting Response and saving response
    std::cout << "Sending GET Request" << std::endl;
    std::istream& responseStream = client.receiveResponse(response);
    std::cout << "Received Response" << std::endl;
    std::ofstream responseFile("response.txt");
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
      Poco::StreamCopier::copyStream(responseStream, responseFile);
    } else {  // Saving Get Request for review
      std::cerr << "Error: " << response.getStatus()
      << " " << response.getReason() << std::endl;
      response.write(responseFile);
    }
    responseFile.close();
  } catch (Poco::Exception& e) {
    std::cout << e.displayText() << std::endl;
  }
  std::cout << "Exiting" << std::endl;
}
