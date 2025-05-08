// Copyright Landon Deam 2025

#pragma once
#ifndef ITEM_H_
#define ITEM_H_

class item {
 public:
  explicit item(int ID);

  /**
  * @brief Gets the item's ID
  * @return Returns the ID of this item.
  */
  const int& getID() const {return ID;}

 private:
  int ID;
};

#endif  // ITEM_H_
