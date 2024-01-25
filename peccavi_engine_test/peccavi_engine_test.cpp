#include <iostream>
#include "peccavi_engine.h"
using namespace std;

int main()
{
	pe::engine eng;
	pe::object obj1;
	obj1.components.push_back(pe::life_time(&obj1, 10));
	pe::object obj2;
	obj2.components.push_back(pe::life_time(&obj2, 20));
	eng.objects.push_back(obj1);
	eng.objects.push_back(obj2);

	cout << &obj1 << " " << obj1.components[0].owner << '\n';
	cout << &(eng.objects[0]) << " " << eng.objects[0].components[0].owner << '\n';
}