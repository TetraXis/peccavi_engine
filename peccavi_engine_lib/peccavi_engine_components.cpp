#include "pch.h"
#include "peccavi_engine_components.h"
#include "peccavi_engine_core.h"

namespace pe
{

	namespace components
	{

		object* component::get_owner() const
		{
			return owner;
		}

		void component::set_owner(object* const object_ptr)
		{
			if (owner)
			{
				owner->detach_component(this);
			}
			object_ptr->components.push_back(this);
			owner = object_ptr;
		}

		life_time::life_time(double new_time) : time(new_time)
		{
		}

		void life_time::tick(double delta_time)
		{
			time -= delta_time;

			if (time <= 0)
			{
				get_owner()->get_owner()->destroy_object(get_owner());
			}
		}

	}
}