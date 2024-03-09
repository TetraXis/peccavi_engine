#include "pch.h"
#include "peccavi_engine_core.h"

namespace pe
{

	engine::engine()
	{
		physics_tick = [this](std::vector<phys_object*>& objects, double delta_time)
			{
				for (phys_object* obj : objects)
				{
					if (obj->active)
					{
						obj->velocity += gravity * delta_time;
						obj->position += obj->velocity * delta_time;
					}
				}

				for (unsigned long long i = 0; i < objects.size(); i++)
				{
					if (objects[i]->active && objects[i]->collision && objects[i]->collision->active)
					{
						for (unsigned long long j = 0; j < i; j++)
						{
							if
							(
								objects[j]->collision &&
								objects[j]->collision->active &&
								objects[i]->collision->is_overlapping(objects[j]->collision)
							)
							{
								collisions::collision_point col_point = objects[i]->collision->get_collision_point(objects[j]->collision);
								if (!col_point.primitive_a)
								{
									continue;
								}
								if (objects[j]->active)
								{
									objects[i]->position += col_point.normal * col_point.intersection_depth / 2;
									objects[j]->position += -col_point.normal * col_point.intersection_depth / 2;
								}
								else
								{
									objects[i]->position += col_point.normal * col_point.intersection_depth;
									// TODO: here
								}
							}
						}
					}
				}
			};
	}

	engine::~engine()
	{
		for (unsigned long long i = 0; i < objects.size(); i++)
		{
			delete objects[i];
		}
		for (auto const& [key, layer] : layers)
		{
			for (phys_object* obj : layer)
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

				for (auto& [key, layer] : layers)
				{
					physics_tick(layer, PE_DELTA_TIME);
				}

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
		layers[(*phys_obj_ptr)->get_layer()].push_back(*phys_obj_ptr);
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

		std::vector<phys_object*>* vec_ptr = &(layers[((phys_object*)(object_ptr))->get_layer()]);

		for (unsigned long long i = 0; i < vec_ptr->size(); i++)
		{
			if (object_ptr == (*vec_ptr)[i])
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

	const std::map<unsigned long long, std::vector<phys_object*>>& engine::get_layers() const
	{
		return layers;
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

	unsigned long long phys_object::get_layer() const
	{
		return layer;
	}

	void phys_object::set_layer(unsigned long long new_layer)
	{
		engine* owner_ptr = get_owner();
		owner_ptr->detach_object(this);
		layer = new_layer;
		phys_object* temp = this;
		owner_ptr->add_phys_object(&temp);
	}


}