#include "pch.h"
#include "CppUnitTest.h"
#include "mapping_r.h"
#include "shipment_r.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UserAcceptanceTest
{
	const struct Map map = populateMap(); //base map with only buildings but no routes
	struct Truck truckFleet[3]; //index 0-blue, 1-green, and 2-yellow
	struct Shipment s;

	int weight = -1;
	double volume = -1;
	struct Point inputPoint = { '-','-' };

	int canCarry[3] = { 0, 0, 0 };
	int canShortest[3] = { 0, 0, 0 };
	int isExit = 0;
	int checkInput = -1;
	int truckIndex = -1;
	int shortestDistance = MAX_ROUTE;
	int capableTruckFound = 0;

	int testLoadSuccessful = 0;

	struct Route diversionRoute = { {0,0}, 0, DIVERSION };

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
			isExit = isExiting(userInput);
			if (!isExit)
			{
				checkInput = parseInput(&weight, &volume, &inputPoint, userInput);

				if (checkInput != 4)
					testLoadSuccessful = 0;
				else if (!validWeight(weight))
					testLoadSuccessful = 0;
				else if (!validVolume(volume))
					testLoadSuccessful = 0;
				else if (!validDeliverableBuilding(&map, inputPoint))
					testLoadSuccessful = 0;
				else
				{
					s = createShipment(weight, volume, inputPoint);
					capableTruckFound = canCarryShipment(truckFleet, canCarry, 3, s);
					shortestDistance = shortestPossibleDiversionDistance(truckFleet, canCarry, 3, &map, s);
					if (shortestDistance < MAX_ROUTE && capableTruckFound)
					{
						canShortestDiversion(truckFleet, canCarry, canShortest, 3, &map, s, shortestDistance);
						truckIndex = getTruckIndex(truckFleet, canShortest, 3);	
						diversionRoute = getOptimalTruckDiversion(truckFleet[truckIndex], &map, s.dest);
						loadPackage(&truckFleet[truckIndex], &s);	
						testLoadSuccessful = 1;
					}
					else if (!capableTruckFound)					
						testLoadSuccessful = 0;
					else if (shortestDistance >= MAX_ROUTE)					
						testLoadSuccessful = 0;
					else					
						testLoadSuccessful = 0;
				}
			}
	}

	TEST_CLASS(UserAcceptance)
	{
	public:
		TEST_METHOD(ValidUserOperations)
		{
			initializeData();
			processUserInput("300 2 3I");
			Assert::AreEqual(0, truckIndex);
			Assert::AreEqual(1, diversionRoute.numPoints);
			processUserInput("10 0.5 3I");
			Assert::AreEqual(1, truckIndex);
			Assert::AreEqual(1, diversionRoute.numPoints);
			processUserInput("1 5 23K");
			Assert::AreEqual(2, truckIndex);
			Assert::AreEqual(3, diversionRoute.numPoints);
			processUserInput("4900 2 3B");
			Assert::AreEqual(1, truckIndex);
			Assert::AreEqual(0, diversionRoute.numPoints);
			processUserInput("10 5 2U");
			Assert::AreEqual(1, truckIndex);
			Assert::AreEqual(0, diversionRoute.numPoints);
			processUserInput("10 2 8L");
			Assert::AreEqual(0, truckIndex);
			Assert::AreEqual(2, diversionRoute.numPoints);
		}
		TEST_METHOD(InvalidUserOperations)
		{
			processUserInput("1 5 1B");
			Assert::AreEqual(0, testLoadSuccessful);
			processUserInput("1 5 23C");
			Assert::AreEqual(0, testLoadSuccessful);
			processUserInput("1 5 25Y");
			Assert::AreEqual(0, testLoadSuccessful);
			processUserInput("5001 5 2B");
			Assert::AreEqual(0, testLoadSuccessful);
			processUserInput("-1 5 2B");
			Assert::AreEqual(0, testLoadSuccessful);
			processUserInput("100 1 2B");
			Assert::AreEqual(0, testLoadSuccessful);			
		}
		TEST_METHOD(Exiting)
		{
			Assert::AreEqual(0, isExit);
			processUserInput("0 0 x");
			Assert::AreEqual(1, isExit);
		}
	};
}