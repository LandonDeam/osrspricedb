// Copyright 2025 Landon Deam

#include "env.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

/**
 * This function loads environment variables from a specified file path.
 * 
 * @param filePath A `std::string` type that represents the path to the
 * file from which environment variables will be loaded.
 */
void env::loadEnvVars(const std::string& filePath) {
  try {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find("=");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                #ifdef __unix__
                setenv(key.c_str(), value.c_str(), 1);
                #else  // __unix__
                _putenv_s(key.c_str(), value.c_str());
                #endif  // __unix__
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
