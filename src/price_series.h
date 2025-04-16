// Copyright Landon Deam 2024

#pragma once
#ifndef PRICE_SERIES_H_
#define PRICE_SERIES_H_

#include <inttypes.h>
#include "item.h"

class price_series : item {
 public:
  price_series(const int& ID, const uint64_t& timestamp,
    const uint64_t& updated);
  void update(const uint64_t& timestamp, const uint64_t& updated);
  uint64_t get_timestamp();
  uint64_t get_updated();
 private:
  uint64_t timestamp;
  uint64_t updated;
};

#endif  // PRICE_SERIES_H_
