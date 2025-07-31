#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "mapping.h"
#include "shipment.h"

int main(void)
{
	const struct Map map = populateMap(); //base map with only buildings but no routes
	struct Truck truckFleet[3]; //index 0-blue, 1-green, and 2-yellow
	struct Shipment s;

	truckFleet[0].currentVolume = 0;
	truckFleet[0].currentWeight = 0;
	truckFleet[0].route = getBlueRoute();
	strcpy(truckFleet[0].name, "Blue");

	truckFleet[1].currentVolume = 0;
	truckFleet[1].currentWeight = 0;
	truckFleet[1].route = getGreenRoute();
	strcpy(truckFleet[1].name, "Green");

	truckFleet[2].currentVolume = 0;
	truckFleet[2].currentWeight = 0;
	truckFleet[2].route = getYellowRoute();
	strcpy(truckFleet[2].name, "Yellow");

	int weight = -1;
	double volume = -1;
	struct Point inputPoint = { '-','-' };

	char input[20];

	int canCarry[3] = { 0, 0, 0 };
	int canShortest[3] = { 0, 0, 0 };
	int isExit = 0;
	int checkInput = -1;
	int truckIndex = -1;
	int shortestDistance = MAX_ROUTE;
	int capableTruckFound = 0;

	puts("Seneca Polytechnic Deliveries:");
	puts("==============================");
	do
	{
		puts("--------------");
		puts("Enter shipment weight, box size and destination (0 0 x to stop):");
		fgets(input, sizeof(input), stdin);
		isExit = isExiting(input);
		if (!isExit)
		{
			checkInput = parseInput(&weight, &volume, &inputPoint, input);

			if (checkInput != 4)
				puts("Invalid input format");
			else if (!validWeight(weight))
				puts("Invalid weight (must be 1-5000)");
			else if (!validVolume(volume))
				puts("Invalid volume (must be 0.5, 2, or 5)");
			else if (!validDeliverableBuilding(&map, inputPoint))
				puts("Invalid destination (refer to map)");
			else
			{
				s = createShipment(weight, volume, inputPoint);
				capableTruckFound = canCarryShipment(truckFleet, canCarry, 3, s);
				shortestDistance = shortestPossibleDiversionDistance(truckFleet, canCarry, 3, &map, s);
				if (shortestDistance < MAX_ROUTE && capableTruckFound)
				{
					canShortestDiversion(truckFleet, canCarry, canShortest, 3, &map, s, shortestDistance);
					truckIndex = getTruckIndex(truckFleet, canShortest, 3);
					printf("Shipping on %s route\n", truckFleet[truckIndex].name);
					struct Route diversion = getOptimalTruckDiversion(truckFleet[truckIndex], &map, s.dest);
					if (diversion.numPoints > 0)
					{
						puts("Diversion: ");
						for (int i = 0; i < diversion.numPoints; i++)
						{
							if (i != 0)
							{
								printf(" , ");
							}
							printf("%d%c", diversion.points[i].row + 1, diversion.points[i].col + 'A');
						}
					}
					else
						puts("No diversion");
					putchar('\n');
					loadPackage(&truckFleet[truckIndex], &s);
					for (int i = 0; i < 3; i++)
					{
						struct Truck temp = truckFleet[i];
						printf("%s weight: %d vol: %.2lf\n", temp.name, temp.currentWeight, temp.currentVolume);
					}
				}
				else if (!capableTruckFound)
				{
					printf("No more room on trucks\n");
				}
				else if (shortestDistance >= MAX_ROUTE)
				{
					printf("Destination cannot be reached by pathfinding\n");
				}
				else
				{
					printf("Unknown issue, cannot make this delivery\n");
				}
			}
		}
	} while (!isExit);
	puts("\n-----Thanks for shipping with Seneca Polytechnic!-----");
	return 0;

}