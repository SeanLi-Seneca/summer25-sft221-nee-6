#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "shipment_functions.h"
#include "shipment.h"

struct Point {
    int row;
    int col;
};

struct Shipment {
    double weight;
    double volume;
    struct Point origin;
    struct Point destination;
};

struct Truck {
    double totalWeight;
    double usedWeight;
    double totalVolume;
    double usedVolume;
    struct Point location;
};

double percentageWeightFull(const struct Truck* truck) {
    if (truck == NULL || truck->totalWeight == 0.0) {
        return 0.0;
    }
    return (truck->usedWeight / truck->totalWeight) * 100.0;
}

double percentageVolumeFull(const struct Truck* truck) {
    if (truck == NULL || truck->totalVolume == 0.0) {
        return 0.0;
    }
    return (truck->usedVolume / truck->totalVolume) * 100.0;
}

bool validateDestination(const char map[25][25], const struct Point* dest) {
    if (!dest || dest->row < 0 || dest->row >= 25 || dest->col < 0 || dest->col >= 25) {
        return false;
    }
    return map[dest->row][dest->col] == 1; // 1 = valid, 0 = building
}

int calculateRouteDistance(const char map[25][25], struct Point start, struct Point end) {
    // Manhattan distance (placeholder for real routing logic)
    return abs(start.row - end.row) + abs(start.col - end.col);
}

int findTruckForShipment(const char map[25][25], struct Truck trucks[], int numTrucks, const struct Shipment* shipment) {
    int bestIndex = -1;
    int minDistance = 1000000; // Arbitrary large value

    for (int i = 0; i < numTrucks; i++) {
        if (canHandleShipment(&trucks[i], shipment)) {
            int distance = calculateRouteDistance(map, trucks[i].location, shipment->origin);
            if (distance < minDistance) {
                minDistance = distance;
                bestIndex = i;
            }
        }
    }

    return bestIndex;
}
