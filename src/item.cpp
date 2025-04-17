// Copyright Landon Deam 2024

#include "item.h"

/**
* @brief Initializes an item object.
*
* @param ID Unique identifier of the item.
*/
item::item(int ID) {
  this->ID = ID;
}

/**
* @brief Gets the ID of the item.
*
* @return The unique identifier of the item.
*/
int item::getID() {
  return this->ID;
}
