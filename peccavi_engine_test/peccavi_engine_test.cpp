#include <iostream>
#include <vector>
#include "peccavi_engine.h"
using namespace std;
using namespace pe;
using namespace components;

struct ass : phys_object
{
	ass()
	{
		tick = [this](double delta_time)
			{
				cout << delta_time << " I am alive!\n";
			};
	}

	~ass()
	{
		cout << "Dead!\n";
	}
};

int main()
{
	engine a;
	a.tick_time = 0.5;
	object* as = new ass();
	component* cmp = new life_time(5);
	as->add_component(&cmp);
	a.add_object(&as);
	a.start();
}