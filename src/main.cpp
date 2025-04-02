// Copyright Landon Deam 2024

#include "main.h"
#include <cstdlib>
#include <stdlib.h>

int main() {
  loadEnvVars(".env");
  getter p("https://prices.runescape.wiki");
  p.get_prices();
  p.get_info();
  return 0;
}

void loadEnvVars(const std::string& filePath) {
  try {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find("=");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                #ifdef _POSIX_VERSION
                std::setenv(key.c_str(), value.c_str(), 1);
                #else // _POSIX_VERSION
                _putenv_s(key.c_str(), value.c_str());
                #endif // _POSIX_VERSION
            }
        }
        file.close();
    } else {
        std::cerr << "Error opening .env file: " << filePath << std::endl;
    }
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }
}
