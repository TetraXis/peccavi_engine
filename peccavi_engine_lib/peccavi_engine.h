#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// #define PE_FORCE_STABLE_TPS
// Use this define if you want stable tps, 
// engine will slow down if it cant reach desired tps. 
// If not defined engine will skip ticks.

// #define PE_ALL_MEMBERS_PUBLIC
// Use this define if you want ALL members of ALL classes to be public.
// WARNING. Newly-public members should be handled carefully, they HAVE SIDE EFFECTS.

#include <memory>

#include "peccavi_engine_literals.h"
#include "peccavi_engine_utilities.h"
#include "peccavi_engine_interfaces.h"
#include "peccavi_engine_components.h"
#include "peccavi_engine_collision.h"
#include "peccavi_engine_core.h"
#include "timer.h"

namespace pe
{
	void example()
	{
		struct my_object : object
		{
			int data = 0;

			my_object()
			{
				// You can set tick function here
				tick = [this](double delta_time)
					{
						delta_time;	// So compiler wouldn't cry
						data = -1;
					};
			}
		};

		engine* eng = new engine();
		my_object* obj = new my_object();
		component* comp = new components::life_time(10);

		// You can set tick function to class instances individually.
		// If you change it that way, make sure to capture a pointer.
		//           vvv
		obj->tick = [obj](double delta_time)
			{
				delta_time;	// So compiler wouldn't cry
				obj->data = 10;
				obj->get_owner()->stop();	// Make engine stop after this tick
			};

		obj->add_component(&comp);	// Takes ownership
		// Now comp = nullptr

		// Cast to (object**) is needed if you passing a derived class
		//              vvvvvvvvvvvvvvvv
		eng->add_object((object**)(&obj));	// Takes ownership
		// Now obj = nullptr;

		eng->start();
	}
}