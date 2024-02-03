#pragma once

#include <string>
#include <vector>

#include "peccavi_engine_utilities.h"
#include "peccavi_engine_literals.h"
#include "peccavi_engine_interfaces.h"
#include "peccavi_engine_components.h"
#include "peccavi_engine_collision.h"
#include "timer.h"

#ifdef PE_FORCE_STABLE_TPS 
#define PE_DELTA_TIME tick_time
#else 
#define PE_DELTA_TIME clock.elapsed_seconds()
#endif

namespace pe
{
	using namespace components;

	typedef vector3_d vec;

	/// <summary>
	/// Describes an object, that can have added components.
	/// </summary>
	/// TODO: Play with alignas(...)
	///   V
	struct object : i_owner<component>
	{
		// When adding members here, dont forget to update constructors
		i_owner<object>* owner = nullptr;
		std::string name = "object";				// Name, can be depricated
		std::vector<component*> components = {};

		object();
		object(const object& other);			// copy constructor
		object(const object&& other) noexcept = delete;	// move constructor

		virtual void tick(double delta_time) { delta_time; }

		void set_owner(component* owned_ptr) override;

		~object();
	};

	/// <summary>
	/// A physical object with mass, position, rotation and collisions...
	/// </summary>
	struct phys_object : object
	{
		double mass = 1;						// Mass, Kg
		vec position = { 0.0,0.0,0.0 };			// Position in 3D space, m
		//vec rotation = { 0.0,0.0,0.0 };
		vec velocity = { 0.0,0.0,0.0 };			// Velocity, m/s
		//std::vector<component*> collisions = {};	// All collision components

		phys_object();
	};

	/// <summary>
	/// Engine that owns everything. Ticks every object inside and does all calculations.
	/// </summary>
	struct engine : i_owner<object>, i_owner<phys_object>
	{
		// Prefer using methods instead of directly changing variables
		double tick_time = 0.01;						// Desired time between ticks, s
		timer clock;									// Clock
		std::vector<object*> objects = {};				// Simplest object. No physics for these
		std::vector<phys_object*> phys_objects = {};	// Physic objects. Full physics for these

		vec gravity = { 0.0,0.0,-9.8 };

		engine();

		virtual void start();
		virtual void stop();
		virtual void loop();
		virtual bool is_running() const;

		using i_owner<object>::add;
		using i_owner<object>::remove;
		using i_owner<object>::set_owner;
		void set_owner(object* owned_ptr) override;
		

		using i_owner<phys_object>::add;
		using i_owner<phys_object>::remove;
		using i_owner<phys_object>::set_owner;
		void set_owner(phys_object* owned_ptr) override;
	};

}