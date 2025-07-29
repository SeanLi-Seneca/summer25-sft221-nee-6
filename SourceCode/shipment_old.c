//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include "shipment.h"
//
//int validWeight(int weight)
//{
//	return (weight > 0 && weight <= 5000);
//}
//
//int validVolume(double volume)
//{
//	return (volume == 0.5 || volume == 2 || volume == 5);
//}
//
//int isPointOnMap(const struct Map* map, const struct Point p)
//{
//	int row = p.row - 1;
//	int col = p.col - 'A';
//	return (row >= 0 && col >= 0) && (row < map->numRows && col < map->numCols);
//}
//
//int isBuilding(const struct Map* map, const struct Point p)
//{
//	int row = p.row - 1;
//	int col = p.col - 'A';
//	return map->squares[row][col] == 1;
//}
//
//int validDeliverableBuilding(const struct Map* map, struct Point destination)
//{
//	if (isPointOnMap(map, destination) && isBuilding(map, destination))
//	{
//		destination.row -= 1;
//		destination.col -= 'A';
//		struct Route adjacentOpening = getPossibleMoves(map, destination, destination);
//		return adjacentOpening.numPoints;
//	}
//	else
//		return 0;
//}
//
//int isExiting(const char* input)
//{
//	return input[0] == '0' && input[2] == '0' && input[4] == 'x';
//}
//
//int parseInput(int* weight, double* volume, struct Point* p, const char* input)
//{
//	int tempRow = -1;
//	char tempCol = '0';
//	int check = sscanf(input, "%d %lf %d%c", weight, volume, &tempRow, &tempCol);
//	p->row = tempRow;
//	p->col = tempCol;
//	return check;
//}
//
//struct Route getDiversion(const struct Map* map, struct Point p, struct Point destinationBuilding)
//{
//	struct Route diversionRoute;
//	diversionRoute.numPoints = MAX_ROUTE;
//	struct Route adjacent;
//	adjacent = getPossibleMoves(map, destinationBuilding, destinationBuilding);
//	struct Route tempRoute;
//	for (int i = 0; i < adjacent.numPoints; i++)
//	{
//		tempRoute = shortestPath_Enhanced(map, p, adjacent.points[i]);
//		if (tempRoute.numPoints <= diversionRoute.numPoints)
//			diversionRoute = tempRoute;
//	}
//	return diversionRoute;
//}
//
//int getClosetPointOnRoute(const struct Map* map, struct Truck t, struct Point destinationBuilding)
//{
//	int closestPointIndex = -1;
//	struct Route closestPointRoute;
//	closestPointRoute.numPoints = MAX_ROUTE;
//	struct Point tempPoint;
//	struct Route tempRoute;
//	for (int i = 0; i < t.route.numPoints; i++)
//	{
//		tempPoint = t.route.points[i];
//		tempRoute = getDiversion(map, tempPoint, destinationBuilding);
//		printf("From: %d %c\n", tempPoint.row + 1, tempPoint.col + 'A');
//		for (int i = 0; i < tempRoute.numPoints; i++)
//		{
//			printf("%d %c\n", tempRoute.points[i].row + 1, tempRoute.points[i].col + 'A');
//		}
//		if (tempRoute.numPoints < closestPointRoute.numPoints)
//		{
//			closestPointRoute = tempRoute;
//			closestPointIndex = i;
//		}
//	}
//	return closestPointIndex;
//}
//
//struct Shipment createShipment(int weight, double volume, struct Point destinationBuilding)
//{
//	struct Shipment temp = { weight, volume, destinationBuilding };
//	return temp;
//}
//
//void canCarryShipment(struct Truck fleet[], int indexCanCarry[], int size, struct Shipment s)
//{
//	for (int i = 0; i < size; i++)
//	{
//		if ((fleet[i].currentWeight + s.weight) <= MAX_WEIGHT && (fleet[i].currentVolume + s.volume) <= MAX_VOLUME)
//			indexCanCarry[i] = 1;
//		else
//			indexCanCarry[i] = 0;
//	}
//}
//
//int shortestPossibleDiversionDistance(struct Truck fleet[], int indexCanCarry[], int size, const struct Map* map, struct Shipment s)
//{
//	int shortestPossibleDistance = MAX_ROUTE;
//	for (int i = 0; i < size; i++)
//	{
//		if (indexCanCarry[i])
//		{
//			int tempDistance = truckDiversionDistance(fleet[i], map, s.dest);
//			if (tempDistance < shortestPossibleDistance)
//				shortestPossibleDistance = tempDistance;
//		}
//	}
//	return shortestPossibleDistance;
//}
//
//void canShortestDiversion(struct Truck fleet[], int indexCanCarry[], int indexCanShortest[], int size, const struct Map* map, struct Shipment s, int shortestDistance)
//{
//	for (int i = 0; i < size; i++)
//	{
//		if (indexCanCarry[i])
//		{
//			int currentTruckDistance = truckDiversionDistance(fleet[i], map, s.dest);
//			if (currentTruckDistance == shortestDistance)
//				indexCanShortest[i] = 1;
//			else
//				indexCanShortest[i] = 0;
//		}
//		else
//			indexCanShortest[i] = 0;
//	}
//}
//
//int getTruckIndex(struct Truck fleet[], int indexCanShortest[], int size)
//{
//	double leastLimiting = 100;
//	int index = -1;
//	for (int i = 0; i < size; i++)
//	{
//		if (indexCanShortest[i])
//		{
//			double currentLimiting = limitingFactor(fleet[i]);
//			if (currentLimiting < leastLimiting)
//			{
//				leastLimiting = currentLimiting;
//				index = i;
//			}
//		}
//	}
//	return index;
//}
//
//int truckDiversionDistance(struct Truck t, const struct Map* map, struct Point destinationBuilding)
//{
//	destinationBuilding.row -= 1;
//	destinationBuilding.col -= 'A';
//	int closestPointIndex = getClosetPointOnRoute(map, t, destinationBuilding);
//	struct Point closetPoint = t.route.points[closestPointIndex];
//	struct Route diversion = getDiversion(map, closetPoint, destinationBuilding);
//	printf("Dest: %d %c\n", destinationBuilding.row + 1, destinationBuilding.col + 'A');
//	printf("Truck: %s\n", t.name);
//	printf("Closest: %d %c\n", closetPoint.row + 1, closetPoint.col + 'A');
//	printf("Diversion:\n");
//	for (int i = 0; i < diversion.numPoints; i++)
//	{
//		printf("%d %c\n", diversion.points[i].row + 1, diversion.points[i].col + 'A');
//	}
//	return closestPointIndex + diversion.numPoints;
//}
//
//double percentWeightFull(struct Truck t)
//{
//	return ((double)t.currentWeight / MAX_WEIGHT) * 100;
//}
//
//double percentVolumeFull(struct Truck t)
//{
//	return (t.currentVolume / MAX_VOLUME) * 100;
//}
//
//double limitingFactor(struct Truck t)
//{
//	double weightFull = percentWeightFull(t);
//	double volumeFull = percentVolumeFull(t);
//	return weightFull > volumeFull ? weightFull : volumeFull;
//}
//
//void loadPackage(struct Truck* t, struct Shipment* s)
//{
//	t->currentWeight += s->weight;
//	t->currentVolume += s->volume;
//}
//
//
//
//
//
