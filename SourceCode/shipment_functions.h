#ifndef SHIPMENT_FUNCTIONS_H
#define SHIPMENT_FUNCTIONS_H

#include <stdbool.h>
#include "shipment.h"

// Returns the percentage of the truck's weight capacity currently used.
double percentageWeightFull(const struct Truck* truck);

// Returns the percentage of the truck's volume capacity currently used.
double percentageVolumeFull(const struct Truck* truck);

// Checks if the destination is a valid delivery location (not a building).
// Returns true if valid, false otherwise.
bool validateDestination(const int map[25][25], const struct Point* dest);

// Finds the best truck index for a shipment based on route proximity and capacity.
// Returns index of the best truck, or -1 if no suitable truck is found.
int findTruckForShipment(const int map[25][25], struct Truck trucks[], int numTrucks, const struct Shipment* shipment);

// Checks if a truck can handle the shipment based on weight and volume.
bool canHandleShipment(const struct Truck* truck, const struct Shipment* shipment);

#endif
