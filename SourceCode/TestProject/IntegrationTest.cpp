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

	TEST_CLASS(IT_01_Weight)
	{
        public:
    TEST_METHOD(AllEmpty_Trucks_BlueChosen)
    {
        initializeData();
        processUserInput("1000 5 3B"); // Weight=1000kg, 5 units volume
        Assert::AreEqual(std::string("Blue"), std::string(truckFleet[truckIndex].name));
    }

    TEST_METHOD(OverWeight_ChooseYellow)
    {
        initializeData();
        truckFleet[0].currentWeight = 2000; // Blue already overweight
        processUserInput("500 3 3B");
        Assert::AreEqual(std::string("Yellow"), std::string(truckFleet[truckIndex].name));
    }
	};

	TEST_CLASS(IT_02_Volume)
	{
        public:
    TEST_METHOD(VolumeCheck_ChooseBlue)
    {
        initializeData();
        processUserInput("200 2 3B"); // Volume ok
        Assert::AreEqual(std::string("Blue"), std::string(truckFleet[truckIndex].name));
    }

    TEST_METHOD(OverVolume_ChooseYellow)
    {
        initializeData();
        truckFleet[0].currentVolume = 9; // Near max volume for blue truck
        processUserInput("200 3 3B");
        Assert::AreEqual(std::string("Yellow"), std::string(truckFleet[truckIndex].name));
    }
	};

    TEST_CLASS(IT_03_LimitingFactor)
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

	TEST_CLASS(IT_04_ShortestDiversion)
	{
        public:
    TEST_METHOD(ShortestRoute_Blue)
    {
        initializeData();
        processUserInput("500 3 3B");
        Assert::AreEqual(std::string("Blue"), std::string(truckFleet[truckIndex].name));
    }

    TEST_METHOD(DiversionNeeded_ChooseYellow)
    {
        initializeData();
        truckFleet[0].distanceFromCustomer = 50; // Force diversion scenario (Blue far away)
        truckFleet[2].distanceFromCustomer = 20; // Yellow closer
        processUserInput("500 3 3B");
        Assert:
	};
}
}