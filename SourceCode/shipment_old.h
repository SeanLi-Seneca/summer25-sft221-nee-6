//#pragma once
//#ifndef SHIPMENT_H
//#define SHIPMENT_H
//
//#include "mapping.h"  
//
//#define MAX_TRUCKS 3
//#define MAX_WEIGHT 5000    // kg
//#define MAX_VOLUME 200     // cubic meters
//
//struct Shipment {
//    int weight;           
//    double volume;           
//    struct Point dest;    // delivery destination on the map (don't know if this is needed as of milestone 2)
//};
//
//
//struct Truck {
//    struct Route route;   // truck’s delivery route
//    int currentWeight;    
//    double currentVolume;    
//    char name[10];
//};
//
////validates weight
//int validWeight(int weight);
//
////validates volume
//int validVolume(double volume);
//
////validates point on the map, not out of bounds
//int isPointOnMap(const struct Map* map, const struct Point p);
//
////validates if point is a building (and not an open space/route)
//int isBuilding(const struct Map* map, const struct Point p);
//
////validates if the destination building can be delivered to
////(meaing it must have at least one adjacent space that's open)
//int validDeliverableBuilding(const struct Map* map, struct Point destination);
//
////check for exit prompt (0 0 x)
//int isExiting(const char* input);
//
////converts the input into weight, volume, and potential destination respectively
////does not perform validatation
////returns the number of variables read
//int parseInput(int* weight, double* volume, struct Point* p, const char* input);
//
////shortest path from a point to a destination building
////diversion cannot cross a route
//struct Route getDiversion(const struct Map* map, struct Point p, struct Point destinationBuilding);
//
////gets the closest point on a truck route to a destination building
//int getClosetPointOnRoute(const struct Map* map, struct Truck t, struct Point destinationBuilding);
//
////constructor for shipment
//struct Shipment createShipment(int weight, double volume, struct Point destinationBuilding);
//
////indicates if a truck at an index can carry the shipment
////if so, set the same index at indexCanCarry as 1, otherwise 0
//void canCarryShipment(struct Truck fleet[], int indexCanCarry[], int size, struct Shipment s);
//
////finds the shortest distance possible, out of all the trucks, to the destination,
////as multiple trucks may be tied for the shortest distance
////does not indicate the actual index of the truck(s)
//int shortestPossibleDiversionDistance(struct Truck fleet[], int indexCanCarry[], int size, const struct Map* map, struct Shipment s);
//
////indicates if a truck at an index can carry the shipment in the shortest distance
////if so, set the same index at indexCanShortest as 1, otherwise 0
//void canShortestDiversion(struct Truck fleet[], int indexCanCarry[], int indexCanShortest[], int size, const struct Map* map, struct Shipment s, int shortestDistance);
//
////returns the index of the optimal truck to carry the shipment
////it can not only carry in the shortest distance
////but also has the least limiting factor
//int getTruckIndex(struct Truck fleet[], int indexCanShortest[], int size);
//
////total distance of a truck to a building, taking account diversion if required
//int truckDiversionDistance(struct Truck t, const struct Map* map, struct Point destinationBuilding);
//
////percentage full by weight
//double percentWeightFull(struct Truck t);
//
////percentage full by volume
//double percentVolumeFull(struct Truck t);
//
////calculates the limiting factor of a truck
//double limitingFactor(struct Truck t);
//
////loads the package onto the truck
//void loadPackage(struct Truck* t, struct Shipment* s);
//
//#endif
