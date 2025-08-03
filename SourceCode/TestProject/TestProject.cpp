#include "pch.h"
#include "CppUnitTest.h"
#include "mapping_r.h"
#include "shipment_r.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestProject
{

    TEST_CLASS(ShipmentTests)
    {
    public:

        TEST_METHOD(ParseInput_ValidInput)
        {
            int weight = 0;
            double volume = 0.0;
            struct Point p = { 0 };
            int result = parseInput(&weight, &volume, &p, "100 0.5 A2");
            Assert::AreEqual(4, result);
            Assert::AreEqual(100, weight);
            Assert::AreEqual(0.5, volume, 0.001);

            
            Assert::AreEqual('A', p.col);
            /*Assert::AreEqual(2, p.row);  */
        }

        TEST_METHOD(ParseInput_InvalidFormat)
        {
            int weight = 0;
            double volume = 0.0;
            struct Point p = { 0 };
            int result = parseInput(&weight, &volume, &p, "bad input");
            Assert::AreNotEqual(4, result);
        }

        TEST_METHOD(ValidWeight_ValidLowerBound)
        {
            Assert::IsTrue(validWeight(1));
        }

        TEST_METHOD(ValidWeight_ValidUpperBound)
        {
            Assert::IsTrue(validWeight(5000));
        }

        TEST_METHOD(ValidWeight_InvalidZero)
        {
            Assert::IsFalse(validWeight(0));
        }

        TEST_METHOD(ValidWeight_InvalidNegative)
        {
            Assert::IsFalse(validWeight(-10));
        }

        TEST_METHOD(ValidWeight_InvalidTooLarge)
        {
            Assert::IsFalse(validWeight(6000));
        }

        TEST_METHOD(ValidVolume_Valid0_5)
        {
            Assert::IsTrue(validVolume(0.5));
        }

        TEST_METHOD(ValidVolume_Valid2)
        {
            Assert::IsTrue(validVolume(2.0));
        }

        TEST_METHOD(ValidVolume_Valid5)
        {
            Assert::IsTrue(validVolume(5.0));
        }

        TEST_METHOD(ValidVolume_Invalid0_4)
        {
            Assert::IsFalse(validVolume(0.4));
        }

        TEST_METHOD(ValidVolume_Invalid3)
        {
            Assert::IsFalse(validVolume(3.0));
        }

        TEST_METHOD(ValidVolume_InvalidNegative)
        {
            Assert::IsFalse(validVolume(-1.0));
        }

        TEST_METHOD(ValidDeliverableBuilding_ValidPoint)
        {
            struct Map map = populateMap();
            struct Point p = { 'A', 0 }; 
            Assert::IsTrue(validDeliverableBuilding(&map, p));
        }

        TEST_METHOD(ValidDeliverableBuilding_InvalidPoint)
        {
            struct Map map = populateMap();
            struct Point p = { 'Z', 99 };
            Assert::IsFalse(validDeliverableBuilding(&map, p));
        }

        TEST_METHOD(CanCarryShipment_TruckHasRoom)
        {
            struct Truck trucks[1] = { 0 };
            trucks[0].currentWeight = 100;
            trucks[0].currentVolume = 1.0;

            struct Shipment s = { 50, 0.5, {'A', 0} };

            int canCarry[1] = { 0 };
            int result = canCarryShipment(trucks, canCarry, 1, s);

            Assert::IsTrue(result > 0);
            Assert::AreEqual(1, canCarry[0]);
        }

        TEST_METHOD(CanCarryShipment_TruckNoRoom)
        {
            struct Truck trucks[1] = { 0 };
            trucks[0].currentWeight = 10000;  
            trucks[0].currentVolume = 50.0;   

            struct Shipment s = { 50, 0.5, {'A', 0} };

            int canCarry[1] = { 0 };
            int result = canCarryShipment(trucks, canCarry, 1, s);

            Assert::AreEqual(0, result);
        }

        TEST_METHOD(CreateShipment_CheckFields)
        {
            struct Shipment s = createShipment(200, 5.0, { 'B', 1 });
            Assert::AreEqual(200, s.weight);
            Assert::AreEqual(5.0, s.volume, 0.001);
            Assert::AreEqual('B', s.dest.col);
            /*Assert::AreEqual(1, s.dest.row);*/
        }

        TEST_METHOD(LoadPackage_UpdatesTruck)
        {
            struct Truck truck = { 0 };
            truck.currentWeight = 0;
            truck.currentVolume = 0.0;
            truck.route = getBlueRoute();

            struct Shipment s = createShipment(1000, 2.0, { 'A', 0 });

            loadPackage(&truck, &s);

            Assert::AreEqual(1000, truck.currentWeight);
            Assert::AreEqual(2.0, truck.currentVolume, 0.001);
        }
    };

}
