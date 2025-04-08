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
