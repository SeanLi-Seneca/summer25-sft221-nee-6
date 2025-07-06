#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "mapping.h"  // Needed for struct Point and struct Route

#define MAX_TRUCKS 3
#define MAX_WEIGHT 5000    // kg
#define MAX_VOLUME 200     // cubic meters

// Structure to represent a single shipment
struct Shipment {
    int weight;           // in kilograms (1–5000)
    int volume;           // in cubic meters (0.5, 2, or 5)
    struct Point dest;    // delivery destination on the map
};

// Structure to represent a delivery truck
struct Truck {
    struct Route route;   // truck’s delivery route
    int currentWeight;    // how much weight the truck is carrying
    int currentVolume;    // how much volume is used in the truck
    char name[10];        // truck name (e.g., "BLUE")
};

#endif
