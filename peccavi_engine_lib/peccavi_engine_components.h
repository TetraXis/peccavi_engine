#pragma once
#include <functional>

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

		public:
			std::function<void(double)> tick = [this](double delta_time) {delta_time;}; // Called every tick

		critical:
			object* owner = nullptr;	// Owning object


		public:
			/// <summary>
			/// Gets an owning object for this component.
			/// </summary>
			/// <returns>Owning object</returns>
			object* get_owner() const;

			/// <summary>
			/// Sets an owning object for this component. Moves ownership of 'this' to the object.
			/// Don't call it from a "std::vector" or any other smart container. Use "object::add_component()" instead.
			/// </summary>
			/// <param name="object_ptr"> - Pointer to object</param>
			void set_owner(object* const object_ptr);
		};

		struct life_time : component
		{
		public:
			double time = 1;	// Time that object has to live, s

		public:
			life_time(double new_time = 1);
		};
	}
}