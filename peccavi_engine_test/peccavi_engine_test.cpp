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

struct slave
{

};

struct master : i_owner<slave>
{
	vector<slave*> vec;

	master() : i_owner<slave>(&vec)
	{}

	void set_owner(slave* owned_ptr)
	{

	}
};

int main()
{


	//eng.add<pe::object>(new pe::object());
}