#include "pch.h"
#include "peccavi_engine_core.h"

namespace pe
{

	engine::engine()
	{
	}

	void engine::start()
	{
		clock.start();
	}

	void engine::stop()
	{
		clock.stop();
	}

	void engine::loop()
	{
		while (clock.is_running())
		{
			if (clock.elapsed_seconds() >= tick_time)
			{
				for (object* obj : objects)
				{
					obj->tick(PE_DELTA_TIME);

					for (component* comp : obj->components)
					{
						comp->tick(PE_DELTA_TIME);
					}
				}

				for (phys_object* ph_obj : phys_objects)
				{
					ph_obj->tick(PE_DELTA_TIME);

					for (component* comp : ph_obj->components)
					{
						comp->tick(PE_DELTA_TIME);
					}
				}
			}
		}
	}

	bool engine::is_running() const
	{
		return clock.is_running();
	}

	object::object()
	{
	}

	object::object(const object& other)
	{
		for (component* comp : components)
		{
			comp->owner = this;
		}
	}

	// TODO: add zeroing, move-constructor must delete other
	/*object::object(const object&& other) noexcept
		: owner_of(&components), name(other.name), components(other.components)
	{
		for (component* comp : components)
		{
			comp->owner = this;
		}
	}*/

	phys_object::phys_object()
	{
		name = "phys_object";
	}

}