#include "./main.h"

int main() {
  try {
    const Poco::URI uri("https://prices.runescape.wiki");
    Poco::Net::initializeSSL();
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler = new Poco::Net::AcceptCertificateHandler(false);
    Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "");
    Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

    Poco::Net::HTTPSClientSession client(uri.getHost(), uri.getPort(), ptrContext);
    std::cout << "Created HTTPS Session" << std::endl;
    Poco::Net::HTTPRequest *test = new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_GET, "/api/v1/osrs/latest/", Poco::Net::HTTPRequest::HTTP_1_1);
    test->add("User-Agent", "Landon Deam (landondeam@gmail.com) - Testing a GE tracker");

    std::ostream& sentRequest = client.sendRequest(*test);
    std::cout << "Sent Request" << std::endl;

    Poco::Net::HTTPResponse response;
    std::istream& rs = client.receiveResponse(response);
    std::cout << "Received Response" << std::endl;
    std::ofstream ofs("response.txt");
    response.write(ofs);
    std::cout << "Wrote To File" << std::endl;
    ofs.close();
  } catch (Poco::Exception& e) {
    std::cout << e.displayText() << std::endl;
  }
  std::cout << "Exiting" << std::endl;
}

