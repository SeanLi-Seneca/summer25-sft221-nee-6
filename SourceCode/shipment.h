#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "mapping.h"  

#define MAX_TRUCKS 3
#define MAX_WEIGHT 5000    // kg
#define MAX_VOLUME 200     // cubic meters

struct Shipment {
    int weight;           
    int volume;           
    struct Point dest;    // delivery destination on the map (don't know if this is needed as of milestone 2)
};


struct Truck {
    struct Route route;   // truck’s delivery route
    int currentWeight;    
    int currentVolume;    
    char name[10];

};

#endif
