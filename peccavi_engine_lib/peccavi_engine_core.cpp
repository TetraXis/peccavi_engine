#include "pch.h"
#include "peccavi_engine_core.h"

namespace pe
{

	engine::engine()
	{
	}

	engine::~engine()
	{
		for (unsigned long long i = 0; i < objects.size(); i++)
		{
			delete objects[i];
		}
		for (auto const& [key, layer] : layers)
		{
			for (phys_object* obj : layer.fixed_objects)
			{
				delete obj;
			}
			for (phys_object* obj : layer.moving_objects)
			{
				delete obj;
			}
			for (phys_object* obj : layer.forcing_objects)
			{
				delete obj;
			}
		}
	}

	void engine::start()
	{
		clock.start();
		loop();
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
					for (component* comp : obj->get_components())
					{
						comp->tick(PE_DELTA_TIME);
					}

					obj->tick(PE_DELTA_TIME);
				}

				//for (phys_object* ph_obj : phys_objects)
				//{
				//	for (component* comp : ph_obj->get_components())
				//	{
				//		comp->tick(PE_DELTA_TIME);
				//	}

				//	ph_obj->tick(PE_DELTA_TIME);
				//}

				//for (phys_object* ph_obj : phys_objects)
				//{
				//	// TODO: here
				//}

				clock.set_start();	// Restarting a new tick start time;
			}
		}
	}

	bool engine::is_running() const
	{
		return clock.is_running();
	}

	void engine::add_object(object** object_ptr)
	{
		if ((*object_ptr)->owner)
		{
			(*object_ptr)->owner->detach_object((*object_ptr));
		}
		objects.push_back(*object_ptr);
		(*object_ptr)->owner = this;
		(*object_ptr) = nullptr;
	}

	void engine::add_phys_object(phys_object** phys_obj_ptr)
	{
		if ((*phys_obj_ptr)->owner)
		{
			(*phys_obj_ptr)->owner->detach_object((*phys_obj_ptr));
		}
		switch ((*phys_obj_ptr)->get_mov_type())
		{
		case movement_type::fixed:
			layers[(*phys_obj_ptr)->get_layer()].fixed_objects.push_back(*phys_obj_ptr);
			break;
		case movement_type::movable:
			layers[(*phys_obj_ptr)->get_layer()].moving_objects.push_back(*phys_obj_ptr);
			break;
		case movement_type::forcing:
			layers[(*phys_obj_ptr)->get_layer()].forcing_objects.push_back(*phys_obj_ptr);
			break;
		default:
			return;
			break;
		}
		(*phys_obj_ptr)->owner = this;
		(*phys_obj_ptr) = nullptr;
	}

	void engine::detach_object(object* const object_ptr)
	{
		for (unsigned long long i = 0; i < objects.size(); i++)
		{
			if (objects[i] == object_ptr)
			{
				objects.erase(objects.begin() + i);
				object_ptr->owner = nullptr;
				return;
			}
		}

		std::vector<phys_object*>* vec_ptr = nullptr;
		phys_object* phys_ptr = (phys_object*)(object_ptr);

		switch (phys_ptr->get_mov_type())
		{
		case movement_type::fixed:
			vec_ptr = &(layers[phys_ptr->get_layer()].fixed_objects);
			break;
		case movement_type::movable:
			vec_ptr = &(layers[phys_ptr->get_layer()].moving_objects);
			break;
		case movement_type::forcing:
			vec_ptr = &(layers[phys_ptr->get_layer()].forcing_objects);
			break;
		default:
			return;
			break;
		}

		for (unsigned long long i = 0; i < vec_ptr->size(); i++)
		{
			if (phys_ptr == (*vec_ptr)[i])
			{
				vec_ptr->erase(vec_ptr->begin() + i);
				object_ptr->owner = nullptr;
				return;
			}
		}
	}

	void engine::destroy_object(object* const object_ptr)
	{
		detach_object(object_ptr);
		delete object_ptr;
	}

	const std::vector<object*>& engine::get_objects() const
	{
		return objects;
	}

	object::object()
	{
	}

	object::~object()
	{
		for (unsigned long long i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
	}

	engine* object::get_owner() const
	{
		return owner;
	}

	void object::set_owner(engine* const engine_ptr)
	{
		if (owner)
		{
			owner->detach_object(this);
		}
		engine_ptr->objects.push_back(this);
		owner = engine_ptr;
	}

	void object::add_component(component** component_ptr)
	{
		if ((*component_ptr)->owner)
		{
			(*component_ptr)->owner->detach_component(*component_ptr);
		}
		components.push_back(*component_ptr);
		(*component_ptr)->owner = this;
		*component_ptr = nullptr;
	}

	void object::detach_component(component* const component_ptr)
	{
		for (unsigned long long i = 0; i < components.size(); i++)
		{
			if (components[i] == component_ptr)
			{
				components.erase(components.begin() + i);
				component_ptr->owner = nullptr;
				return;
			}
		}
	}

	void object::destroy_component(component* const component_ptr)
	{
		detach_component(component_ptr);
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

	movement_type phys_object::get_mov_type() const
	{
		return mov_type;
	}

	unsigned long long phys_object::get_layer() const
	{
		return 0;
	}

}