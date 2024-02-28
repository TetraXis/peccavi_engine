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
	struct phys_object;

	/// <summary>
	/// Describes an object. Can have added components. Base for "phys_object".
	/// </summary>
	struct object 
	{
		friend struct engine;
		friend struct phys_object;
		friend struct component;

	public:
		std::string name = "object";	// Name, can be deprecated

	critical:
		engine* owner = nullptr;					// Owning engine
		std::vector<component*> components = {};	// Attached components


	public:
		object();
		//object(const object& other);			// copy constructor
		object(const object&& other) noexcept = delete;	// move constructor

		/// <summary>
		/// Executes every tick by an owning engine.
		/// </summary>
		/// <param name="delta_time"> - Time that passed since last tick</param>
		virtual void tick(double delta_time) { delta_time; }

		/// <summary>
		/// Gets an owning engine for this object.
		/// </summary>
		/// <returns>Owning engine</returns>
		engine* get_owner() const;

		/// <summary>
		/// Sets an owning engine for this object. Moves ownership of 'this' to the engine.
		/// Don't call it from a "std::vector" or any other smart container. Use "engine::add_object()" instead.
		/// </summary>
		/// <param name="engine_ptr"> - Pointer to engine</param>
		virtual void set_owner(engine* const engine_ptr);

		/// <summary>
		/// Attaches given component to this object. 
		/// Takes ownership of given pointer and erases it.
		/// </summary>
		/// <param name="component_ptr"> - Pointer to a component</param>
		void add_component(component** component_ptr);

		/// <summary>
		/// Detaches given component from this object. 
		/// Does NOT free memory from a pointer.
		/// </summary>
		/// <param name="component_ptr">Ptr to component</param>
		void detach_component(component* const component_ptr);

		/// <summary>
		/// Destroys given component from this object. 
		/// DOES free memory from a pointer.
		/// </summary>
		/// <param name="component_ptr">Ptr to component</param>
		void destroy_component(component* const component_ptr);
		
		/// <summary>
		/// Gets all attached components to this object.
		/// </summary>
		/// <returns>All attached components</returns>
		const std::vector<component*>& get_components() const;
	};

	/// <summary>
	/// A physical object with mass, position, rotation and collisions.
	/// Base class : "object"
	/// </summary>
	struct phys_object : object
	{
		friend struct object;
		friend struct component;

		double mass = 1;						// Mass, Kg
		vec position = { 0.0,0.0,0.0 };			// Position in 3D space, m
		//vec rotation = { 0.0,0.0,0.0 };
		vec velocity = { 0.0,0.0,0.0 };			// Velocity, m/s
		//std::vector<component*> collisions = {};	// All collision components

		phys_object();

		/// <summary>
		/// Sets an owning engine for this phys_object. Moves ownership of 'this' to the engine.
		/// Don't call it from a "std::vector" or any other smart container. Use "engine::add_object()" instead.
		/// </summary>
		/// <param name="engine_ptr"> - Pointer to engine</param>
		virtual void set_owner(engine* const engine_ptr) override;
	};

	/// <summary>
	/// Engine that owns everything. Ticks every object inside and does all calculations.
	/// </summary>
	struct engine
	{
		friend struct object;
		friend struct phys_object;
		friend struct component;

	public:
		double tick_time = 0.01;						// Desired time between ticks, s
		vec gravity = { 0.0,0.0,-9.8 };

	critical:
		timer clock;									// Clock
		std::vector<object*> objects = {};				// Simplest objects. No physics for these
		std::vector<phys_object*> phys_objects = {};	// Physic objects. Full physics for these


	public:
		engine();

		/// <summary>
		/// Starts the engine.
		/// </summary>
		void start();

		/// <summary>
		/// Stops the engine.
		/// </summary>
		void stop();

		/// <summary>
		/// Main execution loop. This loop start with "start()" and stops with "stop()".
		/// </summary>
		void loop();

		/// <summary>
		/// Checks if the engine is running or not.
		/// </summary>
		/// <returns>TRUE if engine is running, FALSE otherwise</returns>
		bool is_running() const;

		/// <summary>
		/// Adds given object to this engine. You might meant to use "add_phys_object()."
		/// Takes ownership of given pointer and erases it.
		/// </summary>
		/// <param name="object_ptr"> - Pointer to an object</param>
		void add_object(object** object_ptr);

		/// <summary>
		/// Adds given phys_object to this engine. 
		/// Takes ownership of given pointer and erases it.
		/// </summary>
		/// <param name="phys_object_ptr"> - Pointer to an object</param>
		void add_phys_object(phys_object** phys_object_ptr);

		/// <summary>
		/// Detaches a given object.
		/// Does NOT free memory from a pointer.
		/// </summary>
		/// <param name="object_ptr"> - Pointer to an object</param>
		void detach_object(object* const object_ptr);

		/// <summary>
		/// Destroys a given object.
		/// DOES free memory from a pointer.
		/// </summary>
		/// <param name="object_ptr"> - Pointer to an object</param>
		void destroy_object(object* const object_ptr);

	};

}