
#include "pch.h"
#include "CppUnitTest.h"
#include "mapping_r.h"
#include "shipment_r.h"

using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestProject
{

	struct Map baseMap = populateMap(); //base map with only buildings but no routes

	/*tests shortestPath function*/
	TEST_CLASS(ShortestPathTest)
	{
	public:
		struct Point a = { 5 - 1, 'D' - 'A' };
		TEST_METHOD(AnotherPoint)
		{
			struct Point b = { 9 - 1, 'F' - 'A' };
			struct Route testRoute = shortestPath(&baseMap, a, b);
			Assert::IsTrue(testRoute.numPoints > 1);
		}
		TEST_METHOD(Itself)
		{
			struct Point b = { 5 - 1, 'D' - 'A' };
			struct Route testRoute = shortestPath(&baseMap, a, b);
			Assert::IsTrue(testRoute.numPoints == 0);
		}
		TEST_METHOD(Adjacent)
		{
			struct Point b = { 6 - 1, 'C' - 'A' };
			struct Route testRoute = shortestPath(&baseMap, a, b);
			Assert::IsTrue(testRoute.numPoints == 2);
		}
		TEST_METHOD(Corner)
		{
			struct Point b = { 25 - 1, 'A' - 'A' };
			struct Route testRoute = shortestPath(&baseMap, a, b);
			Assert::IsTrue(testRoute.numPoints > 1);
		}
		TEST_METHOD(ShorterDistance)
		{
			struct Point b = { 5 - 1, 'L' - 'A' };
			struct Point c = { 8 - 1, 'J' - 'A' };
			struct Point d = { 10 - 1, 'M' - 'A' };
			struct Route testRoute1 = shortestPath(&baseMap, b, d);
			struct Route testRoute2 = shortestPath(&baseMap, c, d);
			Assert::IsTrue(testRoute2.numPoints < testRoute1.numPoints);
		}
		TEST_METHOD(SameDistance)
		{
			struct Point b = { 9 - 1, 'Q' - 'A' };
			struct Point c = { 8 - 1, 'J' - 'A' };
			struct Point d = { 10 - 1, 'M' - 'A' };
			struct Route testRoute1 = shortestPath(&baseMap, b, d);
			struct Route testRoute2 = shortestPath(&baseMap, c, d);
			Assert::IsTrue(testRoute2.numPoints == testRoute1.numPoints);
		}
	};

	TEST_CLASS(GetPossibleMovesTest)
	{
		TEST_METHOD(Origin)
		{
			Point a = { 1 - 1, 'A' - 'A' };
			struct Route testSurround = getPossibleMoves(&baseMap, a, a);
			Assert::IsTrue(testSurround.numPoints == 2);
		}
		TEST_METHOD(BlockedBuilding)
		{
			Point a = { 25 - 1, 'C' - 'A' };
			struct Route testSurround = getPossibleMoves(&baseMap, a, a);
			Assert::IsTrue(testSurround.numPoints == 0);
		}
		TEST_METHOD(CornerBuilding)
		{
			Point a = { 22 - 1, 'B' - 'A' };
			struct Route testSurround = getPossibleMoves(&baseMap, a, a);
			Assert::IsTrue(testSurround.numPoints == 2);
		}
		TEST_METHOD(LoneBuilding)
		{
			Point a = { 3 - 1, 'K' - 'A' };
			struct Route testSurround = getPossibleMoves(&baseMap, a, a);
			Assert::IsTrue(testSurround.numPoints == 4);
		}
	};
}
