#include <iostream>
#include <vector>
#include "peccavi_engine.h"
using namespace std;

struct my_object : pe::object
{
	my_object()
	{
		name = "my_object\n";
	}

	void balls()
	{
		cout << "BALLS!\n";
	}
};

int main()
{
	pe::engine eng;
	pe::object obj;

	//eng.add(new pe::phys_object());

	//eng.add<pe::object>(new pe::object());
}