#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "mapping.h"
#include "shipment.h"

void old_tests()
{
	/*original code
	//struct Map baseMap = populateMap();
	//struct Route blueRoute = getBlueRoute();
	//struct Map routeMap = addRoute(&baseMap, &blueRoute);

	//printMap(&routeMap, 1, 1);
	*/

	//initializing up the map and trucks array
	struct Map baseMap = populateMap(); //base map with only buildings but no routes
	struct Truck truckFleet[3]; //index 0-blue, 1-green, and 2-yellow

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

	struct Map routeMap = addRoute(&baseMap, &truckFleet[0].route);//map with buildings AND all routes
	routeMap = addRoute(&routeMap, &truckFleet[1].route);
	routeMap = addRoute(&routeMap, &truckFleet[2].route);
	printf("Base map\n");
	printMap(&routeMap, 1, 1);//print out the route map in full

	//make a route from origin (1, A) to a non-building point like (9, F) with shortestPath_Enhanced, and print it out
	//there is no more diagonal movement due to the disabling of diagonal points in getPossibleMoves
	struct Point a = { 1 - 1, 'A' - 'A' };
	struct Point b = { 9 - 1, 'F' - 'A' };
	struct Route testRoute = shortestPath_Enhanced(&routeMap, a, b);

	printf("\nRoute from (1, A) to (9, F)\n");
	for (int i = 0; i < testRoute.numPoints; i++)
	{
		printf("%d %c\n", (testRoute.points[i].row + 1), (testRoute.points[i].col + 'A'));
	}

	/*Show the map with the test route, displayed as the letter 'P'.
	Overlap of test route and truck route shows a "null" character.
	*/
	putchar('\n');
	routeMap = addRoute(&routeMap, &testRoute);
	printMap(&routeMap, 1, 1);//print out the route map in full

	struct Point c = { 12 - 1, 'A' - 'A' };
	struct Route testRoute2 = getPossibleMoves(&routeMap, c, c);
	printf("\nSurrounding non-building points of the building point (12, A)\n");
	for (int i = 0; i < testRoute2.numPoints; i++)
	{
		printf("%d %c\n", (testRoute2.points[i].row + 1), (testRoute2.points[i].col + 'A'));
	}

	struct Point d = { 23 - 1, 'C' - 'A' };
	struct Route testRoute3 = getPossibleMoves(&routeMap, d, d);
	printf("\nSurrounding non-building points of the inaccessible building point (23, C)");
	printf("\nThere are none\n");
	for (int i = 0; i < testRoute3.numPoints; i++)
	{
		printf("%d %c\n", (testRoute3.points[i].row + 1), (testRoute3.points[i].col + 'A'));
	}

	//testing maximum movement: from origin to bottom right corner
	struct Point f = { 1 - 1, 'A' - 'A' };
	struct Route testRoute4 = shortestPath_Enhanced(&routeMap, a, f);
	for (int i = 0; i < testRoute4.numPoints; i++)
	{
		printf("%d %c\n", (testRoute4.points[i].row + 1), (testRoute4.points[i].col + 'A'));
	}

	/*DEPRECATED: cannot use shortestPath_Enhanced to a building point like(3, B) - either crashes or infinitely loops
	//b.row = 3 - 1;
	//b.col = 'B' - 'A';
	//testRoute = shortestPath_Enhanced(&routeMap, a, b);
	*/

	//testing parseInput
	int w;
	double s;
	struct Point p;
	parseInput(&w, &s, &p, "1000 0.5 20Y");

	printf("Going to: %d %c\n", p.row + 1, p.col + 'A');
	int bpi = getClosestPoint(&truckFleet[0].route, p);
	int gpi = getClosestPoint(&truckFleet[1].route, p);
	int ypi = getClosestPoint(&truckFleet[2].route, p);
	struct Point bp = truckFleet[0].route.points[bpi];
	struct Point gp = truckFleet[1].route.points[gpi];
	struct Point yp = truckFleet[2].route.points[ypi];
	printf("Blue closest: %d %c\n", bp.row + 1, bp.col + 'A');
	printf("Green closest: %d %c\n", gp.row + 1, gp.col + 'A');
	printf("Yellow closest: %d %c\n", yp.row + 1, yp.col + 'A');
	struct Route bro = shortestPath_Enhanced(&baseMap, bp, p);
	struct Route gro = shortestPath_Enhanced(&baseMap, gp, p);
	struct Route yro = shortestPath_Enhanced(&baseMap, yp, p);
	return 0;
}