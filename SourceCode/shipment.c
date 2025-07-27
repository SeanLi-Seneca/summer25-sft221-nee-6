#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "shipment.h"

int validWeight(int weight)
{
    return (weight > 0 && weight <= 5000);
}

int validVolume(double volume)
{
    return (volume == 0.5 || volume == 2 || volume == 5);
}

int isPointOnMap(const struct Map* map, const struct Point p)
{
    int row = p.row - 1;
    int col = p.col - 'A';
    return (row >= 0 && col >=0) && (row < map->numRows && col < map->numCols);
}

int isBuilding(const struct Map* map, const struct Point p)
{
    int row = p.row - 1;
    int col = p.col - 'A';
    return map->squares[row][col] == 1;
}

int validDeliverableBuilding(const struct Map* map, struct Point destination)
{
    if (isPointOnMap(map, destination) && isBuilding(map, destination))
    {
        struct Route adjacentOpening = getPossibleMoves(map, destination, destination);
        return adjacentOpening.numPoints;
    }
    else
        return 0;
}

int isExiting(const char* input)
{
    return input[0] == '0' && input[2] == '0' && input[4] == 'x';
}

void parseInput(int* weight, double* volume, struct Point* p, const char* input)
{
    int tempRow = -1;
    char tempCol = '0';
    int check = sscanf(input, "%d %lf %d%c", weight, volume, &tempRow, &tempCol);
    if (check != 4)
    {
        return;
    }
    p->row = tempRow - 1;
    p->col = tempCol - 'A';
}

struct Route getDiversion(const struct Map* map, const struct Point closet, const struct Point destinationBuilding)
{
    struct Route diversion = { { 0, 0 }, MAX_ROUTE, DIVERSION }, adjacent, tempRoute;    
    adjacent = getPossibleMoves(&map, destinationBuilding, destinationBuilding);
    for (int i = 0; i < adjacent.numPoints; i++)
    {
        tempRoute = shortestPath(&map, closet, adjacent.points[i]);
        if (tempRoute.numPoints <= diversion.numPoints)
            diversion = tempRoute;
    }
    return diversion;
}

struct Shipment createShipment(int weight, double volume, struct Point destinationBuilding)
{
    struct Shipment temp = { weight, volume, destinationBuilding };
    return temp;
}

void canCarryShipment(struct Truck fleet[], int indexCanCarry[], int size, struct Shipment s)
{
    for (int i = 0; i < size; i++)
    {
        if ((fleet[i].currentWeight + s.weight) <= MAX_WEIGHT && (fleet[i].currentVolume + s.volume) <= MAX_VOLUME)
            indexCanCarry[i] == 1;
        else
            indexCanCarry[i] = 0;
    }
}

int shortestTruckDistance(struct Truck fleet[], int indexCanCarry[], int size, const struct Map* map, struct Shipment s)
{
    int shortestPossibleDistance = MAX_ROUTE;
    for (int i = 0; i < size; i++)
    {
        if (indexCanCarry[i])
        {
            int tempDistance = totalDistance(fleet[i], map, s.dest);
            if (tempDistance < shortestPossibleDistance)
                shortestPossibleDistance = tempDistance;
        }
    }
    return shortestPossibleDistance;
}

void canShorestDistance(struct Truck fleet[], int indexCanCarry[], int indexCanShortest[], int size, const struct Map* map, struct Shipment s, int shortestDistance)
{
    for (int i = 0; i < size; i++)
    {
        if (indexCanCarry[i])
        {
            int currentTruckDistance = totalDistance(fleet[i], map, s.dest);
            if (currentTruckDistance == shortestDistance)
                indexCanShortest[i] = 1;
            else
                indexCanShortest[i] = 0;
        }
        else
            indexCanShortest[i] = 0;
    }
}

void getTruck(struct Truck fleet[], int indexCanShortest[], int size)
{
    double leastLimiting = 100;
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        if (indexCanShortest[i])
        {
            double currentLimiting = limitingFactor(fleet[i]);
            if (currentLimiting < leastLimiting)
            {
                leastLimiting = currentLimiting;
                index = i;
            }
        }
    }
    return index;
}

int totalDistance(struct Truck t, const struct Map* map, struct Point destinationBuilding)
{
    int closestPointIndex = getClosestPoint(&t.route, destinationBuilding);
    struct Point closetPoint = t.route.points[closestPointIndex];
    struct Route diversion = getDiversion(map, closetPoint, destinationBuilding);
    return closestPointIndex + diversion.numPoints;
}

double percentWeightFull(struct Truck t)
{
    return ((double)t.currentWeight / MAX_WEIGHT) * 100;
}

double percentVolumeFull(struct Truck t)
{
    return (t.currentVolume / MAX_VOLUME) * 100;
}

double limitingFactor(struct Truck t)
{
    double weightFull = percentWeightFull(t);
    double volumeFull = percentVolumeFull(t);
    return weightFull > volumeFull ? weightFull : volumeFull; 
}

void loadPackage(struct Truck* t, struct Shipment* s)
{
    t->currentWeight += s->weight;
    t->currentVolume += s->volume;
}





