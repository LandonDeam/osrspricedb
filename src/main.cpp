#include "./main.h"

using namespace Poco;
using namespace Net;

int main() {
  try {
    // Initializing variables
    const URI uri("https://prices.runescape.wiki");
    initializeSSL();
    // TODO - Need to make an actual certificate handler :P
    SharedPtr<InvalidCertificateHandler> ptrHandler =
      new AcceptCertificateHandler(false);
    Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "");
    SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
    // Creating HTTPS Session
    HTTPSClientSession client(uri.getHost(), uri.getPort(), ptrContext);
    std::cout << "Created HTTPS Session" << std::endl;

    // Creating Request
    HTTPRequest *request =
      new HTTPRequest(
        HTTPRequest::HTTP_GET,
        "/api/v1/osrs/latest/",
        HTTPRequest::HTTP_1_1
      );
    request->add(
      "User-Agent",
      "Landon Deam (landondeam@gmail.com)\n  Testing a GE tracker"
    );

    // Sending Request
    std::ostream& sentRequest = client.sendRequest(*request);
    std::ofstream requestFile("request.txt");
    request->write(requestFile);
    requestFile.close();
    std::cout << "Sent Request" << std::endl;

    // Getting Response and saving response header
    HTTPResponse response;
    std::istream& rs = client.receiveResponse(response);
    std::cout << "Received Response" << std::endl;
    std::ofstream responseFile("response.txt");
    response.write(responseFile);
    responseFile.close();

  } catch (Exception& e) {
    std::cout << e.displayText() << std::endl;
  }
  std::cout << "Exiting" << std::endl;
}

