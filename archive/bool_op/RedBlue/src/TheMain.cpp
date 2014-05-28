#include <iostream>
#include "Bundle.h"
#include "RedBlueIntersection.h"
//#include "Test_RB.cpp"
//#include "PointQueue.h"

using namespace std;



//typedef Point_2::Std_point StandardPoint;
/*void testBundleTree();
void testSearchHighest();
void TestBundleListSearch();
void Test_split_a_red_bundle();
void testProcess();
*/

int a, b;
void change(int *ptr)
{
	ptr = &b;
}
int main()
{

	RedBlueIntersection alg;
	a = 2;
	b = 3;
	int *ptr = &a;
	change(ptr);
	cout << *ptr << endl;
	alg.algorithm();
	alg.printIntersections();

	alg.readPolygon("E://polygon1.txt", true);



	//testProcess();
	return 0;
}




