#include <iostream>
#include <vector>
#include "peccavi_engine.h"
using namespace std;
using namespace pe;
using namespace components;

int main()
{
	component* ptr = new component();
	object my_obj;
	//my_obj.attach_component(&ptr);
	cout << ptr;
}