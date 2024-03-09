#include "pch.h"
#include "peccavi_engine_core.h"
#include "peccavi_engine_collision.h"

namespace pe
{

	namespace collisions
	{


		bool collision_spheres(sphere* const sphere_1, sphere* const sphere_2)
		{
			return
				distance_squared(sphere_1->get_world_position(), sphere_2->get_world_position())
				<=
				(sphere_1->radius + sphere_2->radius) * (sphere_1->radius + sphere_2->radius);
		}

		bool collision_cuboids(cuboid* const cuboid_1, cuboid* const cuboid_2)
		{
			vec min_1, max_1, min_2, max_2;

			min_1 = cuboid_1->get_world_position() - cuboid_1->dimensions_halved;
			min_2 = cuboid_2->get_world_position() - cuboid_2->dimensions_halved;
			max_1 = cuboid_1->get_world_position() + cuboid_1->dimensions_halved;
			max_2 = cuboid_2->get_world_position() + cuboid_2->dimensions_halved;

			return
				min_1.x <= max_2.x &&
				max_1.x >= min_2.x &&
				min_1.y <= max_2.y &&
				max_1.y >= min_2.y &&
				min_1.z <= max_2.z &&
				max_1.z >= min_2.z;
		}

		bool collision_sphere_with_cuboid(sphere* const sphere_ptr, cuboid* const cuboid_ptr)
		{
			sphere_ptr;
			cuboid_ptr;
			return false;
		}

		bool primitive::is_overlapping(primitive* const prim_ptr) const
		{
			switch (type)
			{
			case c_type::sphere:
				switch (prim_ptr->type)
				{
				case c_type::sphere:
					return collision_spheres((sphere*)(this), (sphere*)(prim_ptr));
					break;
				case c_type::cuboid:
					return collision_sphere_with_cuboid((sphere*)(this), (cuboid*)(prim_ptr));
					break;
				case c_type::none:
				default:
					return false;
					break;
				}
				break;
			case c_type::cuboid:
				switch (prim_ptr->type)
				{
				case c_type::sphere:
					return collision_sphere_with_cuboid((sphere*)(prim_ptr), (cuboid*)(this));
					break;
				case c_type::cuboid:
					return collision_cuboids((cuboid*)(this), (cuboid*)(prim_ptr));
					break;
				case c_type::none:
				default:
					return false;
					break;
				}
				break;
			case c_type::none:
			default:
				return false;
				break;
			}
		}

		vec primitive::get_world_position() const
		{
			vec result = origin;
			if (owner && owner->get_owner())
			{
				result += owner->get_owner()->position;
			}
			return result;
		}

		primitive::primitive(c_type new_type) : type(new_type)
		{
		}

		sphere::sphere() : primitive(c_type::sphere)
		{
		}

		cuboid::cuboid() : primitive(c_type::cuboid)
		{
		}

		phys_object* collision_skeleton::get_owner() const
		{
			return owner;
		}

		void collision_skeleton::add_primitive(primitive** prim_ptr)
		{
			if ((*prim_ptr)->owner)
			{
				(*prim_ptr)->owner->detach_primitive(*prim_ptr);
			}
			(*prim_ptr)->owner = this;
			primitives.push_back(*prim_ptr);
			prim_ptr = nullptr;
			return;
		}

		void collision_skeleton::detach_primitive(primitive* const prim_ptr)
		{
			for (unsigned long long i = 0; i < primitives.size(); i++)
			{
				if (primitives[i] == prim_ptr)
				{
					primitives.erase(primitives.begin() + i);
					prim_ptr->owner = nullptr;
					return;
				}
			}
		}

		void collision_skeleton::destroy_primitive(primitive* const prim_ptr)
		{
			for (unsigned long long i = 0; i < primitives.size(); i++)
			{
				if (primitives[i] == prim_ptr)
				{
					primitives.erase(primitives.begin() + i);
					delete prim_ptr;
					return;
				}
			}
		}

		bool collision_skeleton::is_overlapping(collision_skeleton* col_ptr)
		{
			// TODO: Add bounding sphere check here
			for (unsigned long long i = 0; i < primitives.size(); i++)
			{
				for (unsigned long long j = 0; j < col_ptr->primitives.size(); j++)
				{
					if (primitives[i]->is_overlapping(col_ptr->primitives[j]))
					{
						return true;
					}
				}
			}
			return false;
		}

		collision_point collision_skeleton::get_collision_point(collision_skeleton* col_ptr)
		{
			collision_point result = {};
			// TODO: Add cuboid support

			for (unsigned long long i = 0; i < primitives.size(); i++)
			{
				if (primitives[i]->type == c_type::sphere)
				{
					for (unsigned long long j = 0; j < col_ptr->primitives.size(); j++)
					{
						if (primitives[j]->type == c_type::sphere && primitives[i]->is_overlapping(col_ptr->primitives[j]))
						{
							result.primitive_a = primitives[i];
							result.primitive_b = col_ptr->primitives[j];

							sphere* a = ((sphere*)(primitives[i]));
							sphere* b = ((sphere*)(col_ptr->primitives[j]));

							result.normal = (a->get_world_position() - b->get_world_position()).normalized();
							result.intersection_depth = ((a->get_world_position() + -result.normal * a->radius) - (b->get_world_position() + result.normal * b->radius)).len();
							result.position = (a->get_world_position() + -result.normal * a->radius + b->get_world_position() + result.normal * b->radius) / 2;

							return result;
						}
					}
				}
			}

			return result;
		}

	}

}
