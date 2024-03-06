#pragma once

#include <vector>
#include <functional>

#include "peccavi_engine_utilities.h"
#include "peccavi_engine_components.h"

#ifdef PE_ALL_MEMBERS_PUBLIC
#define critical public
#else
#define critical private
#endif

namespace pe
{

	typedef vector3_d vec;

	struct object;
	struct phys_object;

	/// <summary>
	/// Contains all collision related classes.
	/// </summary>
	namespace collisions
	{

		/// <summary>
		/// Collision primitive types. They are metashapes essentially.
		/// </summary>
		enum class c_type
		{
			none,
			sphere,
			cuboid
		};

		struct collision;
		struct primitive;
		struct sphere;
		struct cuboid;

		/// <summary>
		/// Collision primitive.
		/// </summary>
		struct primitive
		{
			friend struct collision;

		public:
			const c_type type = c_type::none;
			vec origin = { 0.0,0.0,0.0 };	// Shift from owner's origin, m

		critical:
			collision* owner = nullptr;

		public:
			primitive(c_type new_type = c_type::none);

			/// <summary>
			/// Check if this collision primitive is overlapping with a given one.
			/// </summary>
			/// <param name="prim_ptr"> - Ptr to other primitive</param>
			/// <returns>True if overlapping, false otherwise</returns>
			bool is_overlapping(primitive* const prim_ptr) const;

			/// <summary>
			/// Gets absolute world location.
			/// </summary>
			/// <returns>Absolute world location</returns>
			vec get_world_location() const;

		};

		struct sphere : primitive
		{
		public:
			double radius = 1.0;

		public:
			sphere();
		};

		/// <summary>
		/// For now axis aligned.
		/// </summary>
		struct cuboid : primitive
		{
		public:
			vec dimensions_halved = { 1.0,1.0,1.0 };

		public:
			cuboid();
		};

		struct collision
		{
		public:

		critical:
			phys_object* owner = nullptr;
			vec bounding_sphere_origin = { 0.0,0.0,0.0 };	// Origin of a smallest sphere. Offset from owner's origin, m
			double bounding_sphere_radius = 0;
			std::vector<primitive*> primitives = {};

		public:
			collision() {}

			phys_object* get_owner() const;

			void add_primitive(primitive** prim_ptr);

			void detach_primitive(primitive* const prim_ptr);

			void destroy_primitive(primitive* const prim_ptr);

			bool is_overlapping(collision* col_ptr);
		};

		bool collision_spheres(sphere* const sphere_1, sphere* const sphere_2);
		bool collision_cuboids(cuboid* const cuboid_1, cuboid* const cuboid_2);
		bool collision_sphere_with_cuboid(sphere* const sphere_ptr, cuboid* const cuboid_ptr);
	}

}