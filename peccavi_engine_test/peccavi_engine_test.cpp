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

using namespace pe;

struct slave : is_owned<slave>
{

};

struct master : owner_of<slave>
{
	vector<slave*> slaves;

	master() : owner_of<slave>(&slaves) {}
};


int main()
{
	master M;
	slave S;

	M.add(&S);

	//eng.add(new pe::phys_object());

	//eng.add<pe::object>(new pe::object());
}