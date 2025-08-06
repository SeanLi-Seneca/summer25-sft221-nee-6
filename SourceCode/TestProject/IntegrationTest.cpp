#include "pch.h"
#include "CppUnitTest.h"
#include "mapping_r.h"
#include "shipment_r.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTest
{
	const struct Map map = populateMap(); //base map with only buildings but no routes
	struct Truck truckFleet[3]; //index 0-blue, 1-green, and 2-yellow
	struct Shipment s;

	int weight = -1;
	double volume = -1;
	struct Point inputPoint = { '-','-' };

	char input[20];

	int canCarry[3] = { 0, 0, 0 };
	int canShortest[3] = { 0, 0, 0 };
	int checkInput = -1;
	int truckIndex = -1;
	int shortestDistance = MAX_ROUTE;
	int capableTruckFound = 0;

	void initializeData()
	{
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

		for (int i = 0; i < 3; i++)
		{
			canCarry[i] = 0;
			canShortest[i] = 0;
		}
	}

	void processUserInput(const char* userInput)
	{
		checkInput = parseInput(&weight, &volume, &inputPoint, userInput);
		s = createShipment(weight, volume, inputPoint);
		capableTruckFound = canCarryShipment(truckFleet, canCarry, 3, s);
		shortestDistance = shortestPossibleDiversionDistance(truckFleet, canCarry, 3, &map, s);
		if (shortestDistance < MAX_ROUTE && capableTruckFound)
		{
			canShortestDiversion(truckFleet, canCarry, canShortest, 3, &map, s, shortestDistance);
			truckIndex = getTruckIndex(truckFleet, canShortest, 3);
			struct Route diversion = getOptimalTruckDiversion(truckFleet[truckIndex], &map, s.dest);
		}
	}

    TEST_CLASS(LimitingFactor)
    {
    public:
        TEST_METHOD(Overweight)
        {
			initializeData();
			truckFleet[0].currentWeight = 4900;
			truckFleet[0].currentVolume = 40;
			truckFleet[1].currentWeight = 4000;
			truckFleet[1].currentVolume = 100;
			truckFleet[2].currentWeight = 5000;
			truckFleet[2].currentVolume = 200;
			processUserInput("1 0.5 3B");
			Assert::AreEqual(1, truckIndex);			
        }
		TEST_METHOD(Overvolume)
		{
			initializeData();
			truckFleet[0].currentWeight = 1000;
			truckFleet[0].currentVolume = 150;
			truckFleet[1].currentWeight = 2500;
			truckFleet[1].currentVolume = 100;
			truckFleet[2].currentWeight = 5000;
			truckFleet[2].currentVolume = 200;
			processUserInput("1 0.5 3B");
			Assert::AreEqual(1, truckIndex);
		}
		TEST_METHOD(Equivalent)
		{
			initializeData();
			truckFleet[0].currentWeight = 1000;
			truckFleet[0].currentVolume = 150;
			truckFleet[1].currentWeight = 3750;
			truckFleet[1].currentVolume = 100;
			truckFleet[2].currentWeight = 5000;
			truckFleet[2].currentVolume = 200;
			processUserInput("1 0.5 3B");
			Assert::AreEqual(0, truckIndex);
		}
    };
}