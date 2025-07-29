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
	int row = p.row;
	int col = p.col;
	return (row >= 0 && col >= 0) && (row < map->numRows && col < map->numCols);
}

int isBuilding(const struct Map* map, const struct Point p)
{
	int row = p.row;
	int col = p.col;
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

int parseInput(int* weight, double* volume, struct Point* p, const char* input)
{
	int tempRow = -1;
	char tempCol = '0';
	int check = sscanf(input, "%d %lf %d%c", weight, volume, &tempRow, &tempCol);
	p->row = tempRow - 1;
	p->col = tempCol - 'A';
	return check;
}

struct Shipment createShipment(int weight, double volume, struct Point destinationBuilding)
{
	struct Shipment temp = { weight, volume, destinationBuilding };
	return temp;
}

int canCarryShipment(struct Truck fleet[], int indexCanCarry[], int size, struct Shipment s)
{
	int capableTruckFound = 0;
	for (int i = 0; i < size; i++)
	{
		if ((fleet[i].currentWeight + s.weight) <= MAX_WEIGHT && (fleet[i].currentVolume + s.volume) <= MAX_VOLUME)
		{
			indexCanCarry[i] = 1;
			capableTruckFound = 1;
		}
		else
			indexCanCarry[i] = 0;
	}
	return capableTruckFound;
}

int truckDiversionDistance(struct Truck t, const struct Map* map, struct Point destinationBuilding)
{
	int shortestDiversionPoints = MAX_ROUTE;
	struct Point currentPoint;
	struct Route currentDiversion;
	for (int i = 0; i < t.route.numPoints; i++)
	{
		currentPoint = t.route.points[i];
		if (distance(&currentPoint, &destinationBuilding) == 1.0)
		{
			return 0;
		}
		currentDiversion = shortestPath_Enhanced(map, currentPoint, destinationBuilding);
		if (currentDiversion.numPoints < shortestDiversionPoints)
		{	
			shortestDiversionPoints = currentDiversion.numPoints;
		}
	}
	return shortestDiversionPoints;
}

int shortestPossibleDiversionDistance(struct Truck fleet[], int indexCanCarry[], int size, const struct Map* map, struct Shipment s)
{
	int shortestPossibleDistance = MAX_ROUTE;
	for (int i = 0; i < size; i++)
	{
		if (indexCanCarry[i])
		{
			int tempDistance = truckDiversionDistance(fleet[i], map, s.dest);
			if (tempDistance < shortestPossibleDistance)
				shortestPossibleDistance = tempDistance;
		}
	}
	return shortestPossibleDistance;
}

void canShortestDiversion(struct Truck fleet[], int indexCanCarry[], int indexCanShortest[], int size, const struct Map* map, struct Shipment s, int shortestDistance)
{
	for (int i = 0; i < size; i++)
	{
		if (indexCanCarry[i])
		{
			int currentTruckDistance = truckDiversionDistance(fleet[i], map, s.dest);
			if (currentTruckDistance == shortestDistance)
				indexCanShortest[i] = 1;
			else
				indexCanShortest[i] = 0;
		}
		else
			indexCanShortest[i] = 0;
	}
}

int getTruckIndex(struct Truck fleet[], int indexCanShortest[], int size)
{
	double leastLimiting = 100.0;
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

struct Route getOptimalTruckDiversion(struct Truck t, const struct Map* map, struct Point destinationBuilding)
{
	struct Route shortestDiversion;	
	shortestDiversion.numPoints = MAX_ROUTE;
	struct Point currentPoint;
	struct Route currentDiversion;
	for (int i = 0; i < t.route.numPoints; i++)
	{
		currentPoint = t.route.points[i];
		if (distance(&currentPoint, &destinationBuilding) == 1.0)
		{
			struct Route onRoute = { {0,0}, 0, DIVERSION };
			return onRoute;
		}
		currentDiversion = shortestPath_Enhanced(map, currentPoint, destinationBuilding);
		if (currentDiversion.numPoints < shortestDiversion.numPoints)
		{
			shortestDiversion = currentDiversion;
		}
	}
	return shortestDiversion;
}

void loadPackage(struct Truck* t, struct Shipment* s)
{
	t->currentWeight += s->weight;
	t->currentVolume += s->volume;
}