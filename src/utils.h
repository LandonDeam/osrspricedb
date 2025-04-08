// Copyright 2025 Landon Deam

#pragma once
#ifndef UTILS_H_
#define UTILS_H_

#include <string>

class utils {
 public:
  static std::string urlEncode(const std::string& input);
  static void makeFolderIfNotExists(const std::string& path);
};

#endif  // UTILS_H_
