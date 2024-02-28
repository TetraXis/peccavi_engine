#pragma once

#ifdef PE_ALL_MEMBERS_PUBLIC
#define critical public
#else
#define critical private
#endif

#include "peccavi_engine_interfaces.h"

namespace pe
{
	struct object;

	namespace components
	{

		/// <summary>
		/// A simple component, which is called every tick. For example `life_time_component` or `projectile_movement_component`.
		/// </summary>
		struct component
		{
			friend struct object;

		critical:
			object* owner = nullptr;
			//owner_of<component>* owner = nullptr;	// Owner of this component, advised not to change it manualy

		public:
			virtual void tick(double delta_time) { delta_time; }

			object* get_owner() const;
			void attach_to(object* const owner_object);
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
					//get_owner()->get_owning_engine()
				}
			}
		};
	}
}