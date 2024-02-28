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

#ifdef PE_ALL_MEMBERS_PUBLIC
#define critical public
#else
#define critical private
#endif

namespace pe
{
	using namespace components;

	typedef vector3_d vec;

	struct engine;

	/// <summary>
	/// Describes an object, that can have added components.
	/// </summary>
	/// TODO: Play with alignas(...)
	///   V
	struct object 
	{
		friend struct component;
		friend struct engine;

	public:
		std::string name = "object";				// Name, can be deprecated

	critical:
		// When adding members here, don't forget to update constructors
		//object* owner = nullptr;
		engine* owning_engine = nullptr;
		std::vector<component*> components = {};


	public:
		object();
		//object(const object& other);			// copy constructor
		object(const object&& other) noexcept = delete;	// move constructor

		virtual void tick(double delta_time) { delta_time; }

		//object* get_owner() const;
		engine* get_owning_engine() const;

		void add_component(component** component_ptr);

		/// <summary>
		/// Detaches given component from this object. DOES free pointer.
		/// </summary>
		/// <param name="component_ptr">Ptr to component</param>
		void remove_component(component* const component_ptr);
		
		const std::vector<component*>& get_components() const;
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
	struct engine
	{
	public:
		double tick_time = 0.01;						// Desired time between ticks, s
		vec gravity = { 0.0,0.0,-9.8 };

	critical:
		timer clock;									// Clock
		std::vector<object*> objects = {};				// Simplest object. No physics for these
		std::vector<phys_object*> phys_objects = {};	// Physic objects. Full physics for these


	public:
		engine();

		void start();
		void stop();
		void loop();
		bool is_running() const;

		void add_object(object** object_ptr);
		void add_phys_object(phys_object** object_ptr);
		void destroy_object(object* const object_ptr);
	};

}