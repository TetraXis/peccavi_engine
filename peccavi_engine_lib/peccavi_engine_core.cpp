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

					for (component* comp : obj->get_components())
					{
						comp->tick(PE_DELTA_TIME);
					}
				}

				for (phys_object* ph_obj : phys_objects)
				{
					ph_obj->tick(PE_DELTA_TIME);

					for (component* comp : ph_obj->get_components())
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

	void engine::add_object(object** object_ptr)
	{
		objects.push_back(*object_ptr);
		(*object_ptr)->owner = this;
		(*object_ptr) = nullptr;
	}

	void engine::add_phys_object(phys_object** phys_object_ptr)
	{
		phys_objects.push_back(*phys_object_ptr);
		(*phys_object_ptr)->owner = this;
		(*phys_object_ptr) = nullptr;
	}

	void engine::destroy_object(object* const object_ptr)
	{
		for (unsigned long long i = 0; i < objects.size(); i++)
		{
			if (objects[i] == object_ptr)
			{
				objects.erase(objects.begin() + i);
				return;
			}
		}
		for (unsigned long long i = 0; i < phys_objects.size(); i++)
		{
			if (phys_objects[i] == object_ptr)
			{
				phys_objects.erase(phys_objects.begin() + i);
				return;
			}
		}
	}

	object::object()
	{
	}

	/*object::object(const object& other)
	{
		for (component* comp : components)
		{
			comp->owner = this;
		}
	}*/

	/*object* object::get_owner() const
	{
		return owner;
	}*/

	engine* object::get_owner() const
	{
		return owner;
	}

	void object::set_owner(engine* const engine_ptr)
	{
		if (owner)
		{
			owner->destroy_object(this);
		}
		engine_ptr->objects.push_back(this);
		owner = engine_ptr;
	}

	void object::add_component(component** component_ptr)
	{
		(*component_ptr)->attach_to(this);
		*component_ptr = nullptr;
	}

	void object::remove_component(component* const component_ptr)
	{
		component_ptr->attach_to(nullptr);
		delete component_ptr;
	}

	const std::vector<component*>& object::get_components() const
	{
		return components;
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

	void phys_object::set_owner(engine* const engine_ptr)
	{
		if (owner)
		{
			owner->destroy_object(this);
		}
		engine_ptr->phys_objects.push_back(this);
		owner = engine_ptr;
	}

}