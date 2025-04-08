// Copyright 2025 Landon Deam

#pragma once
#ifndef ENV_H_
#define ENV_H_

#include <string>

class env {
 public:
  static void loadEnvVars(const std::string& filepath);
};

#endif  // ENV_H_
