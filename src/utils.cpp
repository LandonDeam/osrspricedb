// Copyright 2025 Landon Deam

#include "utils.h"
#include <iostream>
#include <filesystem>
#include <fstream>
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

std::string utils::readTextFile(const std::string& path) {
  std::filesystem::path file_path = path;

  if (!std::filesystem::exists(file_path)) {
    std::cerr << "File or path does not exist: " << path << std::endl;
    return nullptr;
  }

  if (std::filesystem::is_directory(file_path)) {
    std::cerr << "Pointed to directory, not file: " << path  << std::endl;
    return nullptr;
  }

  try {
    std::ifstream file(file_path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
      std::cerr << "Could not open file " << path << std::endl;
      return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }

  return nullptr;
}
