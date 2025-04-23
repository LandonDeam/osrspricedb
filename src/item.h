// Copyright Landon Deam 2024

#pragma once
#ifndef ITEM_H_
#define ITEM_H_

class item {
 public:
  explicit item(int ID);
  const int& getID() {return ID;}

 private:
  int ID;
};

#endif  // ITEM_H_
