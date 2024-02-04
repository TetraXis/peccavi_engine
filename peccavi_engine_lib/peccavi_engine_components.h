#pragma once

#include "peccavi_engine_interfaces.h"

namespace pe
{
	struct object;

	namespace components
	{

		/// <summary>
		/// A simple component, which is called every tick. For example `life_time_component` or `projectile_movement_component`.
		/// </summary>
		struct component : is_owned<component>
		{
			//owner_of<component>* owner = nullptr;	// Owner of this component, advised not to change it manualy

			virtual void tick(double delta_time) { delta_time; }
		};

		struct life_time : component
		{
			double time = 1;	// Time that object has to live, s

			life_time(double new_time = 1) : time(new_time)
			{}

			void tick(double delta_time) override
			{
				time -= delta_time;

				if (time <= 0)
				{
					//owner->~owner_of<component>();
					owner->~owner_of();
				}
			}
		};
	}
}