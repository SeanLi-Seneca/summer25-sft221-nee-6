#include <stdbool.h>
#include <stdlib.h>
#include "shipment_functions.h"
#include "shipment.h"

// MAX_WEIGHT and MAX_VOLUME from shipment.h

double percentageWeightFull(const struct Truck* truck) {
    if (truck == NULL || MAX_WEIGHT == 0) {
        return 0.0;
    }
    return ((double)truck->currentWeight / MAX_WEIGHT) * 100.0;
}

double percentageVolumeFull(const struct Truck* truck) {
    if (truck == NULL || MAX_VOLUME == 0) {
        return 0.0;
    }
    return ((double)truck->currentVolume / MAX_VOLUME) * 100.0;
}

bool validateDestination(const int map[25][25], const struct Point* dest) {
    if (!dest || dest->row < 0 || dest->row >= 25 || dest->col < 0 || dest->col >= 25) {
        return false;
    }
    return map[dest->row][dest->col] == 1;  // 1 = valid delivery location, 0 = building
}

bool canHandleShipment(const struct Truck* truck, const struct Shipment* shipment) {
    if (!truck || !shipment) return false;

    if ((truck->currentWeight + shipment->weight) > MAX_WEIGHT) return false;
    if ((truck->currentVolume + shipment->volume) > MAX_VOLUME) return false;

    return true;
}

int findTruckForShipment(const int map[25][25], struct Truck trucks[], int numTrucks, const struct Shipment* shipment) {
    int bestIndex = -1;
    int minDistance = 1000000;

    for (int i = 0; i < numTrucks; i++) {
        if (canHandleShipment(&trucks[i], shipment)) {
            // Calculate Manhattan distance from last point on truck's route to shipment destination
            struct Point lastPoint = trucks[i].route.points[trucks[i].route.numPoints - 1];
            int distance = abs(lastPoint.row - shipment->dest.row) + abs(lastPoint.col - shipment->dest.col);

            if (distance < minDistance) {
                minDistance = distance;
                bestIndex = i;
            }
        }
    }

    return bestIndex;
}
