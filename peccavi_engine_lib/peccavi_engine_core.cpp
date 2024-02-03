#include "pch.h"
#include "peccavi_engine_core.h"

namespace pe
{

	engine::engine()
		: i_owner<object>(&objects), i_owner<phys_object>(&phys_objects)
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

	void engine::set_owner(object* owned_ptr)
	{
		owned_ptr->owner = this;
	}

	void engine::set_owner(phys_object* owned_ptr)
	{
		owned_ptr->owner = this;
	}

	object::object() : i_owner(&components)
	{
	}

	object::object(const object& other)
		: i_owner(&components), name(other.name), components(other.components)
	{
		for (component* comp : components)
		{
			comp->owner = this;
		}
	}

	void object::set_owner(component* owned_ptr)
	{
		owned_ptr->owner = this;
	}

	object::~object()
	{
		if (owner)
		{
			owner->remove(this);
		}
	}

	// TODO: add zeroing, move-constructor must delete other
	/*object::object(const object&& other) noexcept
		: i_owner(&components), name(other.name), components(other.components)
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