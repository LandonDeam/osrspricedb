// Copyright 2025 Landon Deam

#include "utils.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <cctype>

std::string utils::urlEncode(const std::string& input) {
  std::stringstream encoded;
  encoded.fill('0');
  encoded << std::hex;

  for (char c : input) {
      if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
          encoded << c;
      } else {
          encoded << std::uppercase;
          encoded << '%' << std::setw(2) << int((unsigned char)c);
          encoded << std::nouppercase;
      }
  }
  return encoded.str();
}

void utils::makeFolderIfNotExists(const std::string& path) {
  std::filesystem::path dir_path = path;
  if (!std::filesystem::exists(dir_path)) {
    if (std::filesystem::create_directory(dir_path)) {
      std::cout << "Directory create successfully: " << dir_path << std::endl;
    } else {
      std::cerr << "Failed to create directory: " << dir_path << std::endl;
    }
  } else {
    std::cout << "Directory already exists: " << dir_path << std::endl;
  }
}
