// Copyright Landon Deam 2025

#pragma once
#ifndef ENV_H_
#define ENV_H_

#include <string>

class env {
 public:
  static void loadEnvVars(const std::string& filepath);
};

#endif  // ENV_H_
