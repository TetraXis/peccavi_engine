#include "pch.h"
#include "peccavi_engine_core.h"

namespace pe
{

	engine::engine()
	{
		physics_tick = [this](std::vector<object*>& objects, double delta_time)
			{
				for (object* obj : objects)
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
								collision_point col_point = objects[i]->collision->get_collision_point(objects[j]->collision);
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
		for (auto const& [key, layer] : layers)
		{
			for (object* obj : layer)
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
				/*for (object* obj : objects)
				{
					for (component* comp : obj->get_components())
					{
						comp->tick(PE_DELTA_TIME);
					}

					obj->tick(PE_DELTA_TIME);
				}*/

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

	void engine::add_object(object** obj_ptr)
	{
		if ((*obj_ptr)->owner)
		{
			(*obj_ptr)->owner->detach_object((*obj_ptr));
		}
		layers[(*obj_ptr)->get_layer()].push_back(*obj_ptr);
		(*obj_ptr)->owner = this;
		(*obj_ptr) = nullptr;
	}

	void engine::detach_object(object* const object_ptr)
	{
		std::vector<object*>* vec_ptr = &(layers[object_ptr->get_layer()]);

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

	const std::vector<object*>& engine::get_objects(unsigned long long selected_layer) const
	{
		return layers.at(selected_layer);
	}

	const std::map<unsigned long long, std::vector<object*>>& engine::get_all_layers() const
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
		delete collision;
	}

	engine* object::get_owner() const
	{
		return owner;
	}

	//void object::set_owner(engine* const engine_ptr)
	//{
	//	if (owner)
	//	{
	//		owner->detach_object(this);
	//	}
	//	object* temp = this;
	//	engine_ptr->add_object(&temp);
	//	//owner = engine_ptr;
	//}

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

	unsigned long long object::get_layer() const
	{
		return layer;
	}

	void object::set_layer(unsigned long long new_layer)
	{
		engine* owner_ptr = get_owner();
		owner_ptr->detach_object(this);
		layer = new_layer;
		object* temp = this;
		owner_ptr->add_object(&temp);
	}

	collision_skeleton* object::get_collision_skeleton() const
	{
		return collision;
	}

	void object::set_collision_skeleton(collision_skeleton** col_skel)
	{
		if ((*col_skel)->owner)
		{
			(*col_skel)->owner->detach_collision_skeleton();
		}
		collision = *col_skel;
		(*col_skel)->owner = this;
		(*col_skel) = nullptr;
	}

	void object::detach_collision_skeleton()
	{
		collision->owner = nullptr;
		collision = nullptr;
	}

}