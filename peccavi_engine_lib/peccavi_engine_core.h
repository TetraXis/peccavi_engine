#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>

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
	using namespace collisions;

	typedef vector3_d vec;

	struct engine;

	/// <summary>
	/// Describes an object in 3D space. It is possible to add components to it.
	/// </summary>
	struct object 
	{
		friend struct engine;
		friend struct component;

	public:
		std::string name = "object";												// Name, can be deprecated
		std::function<void(double)> tick = [this](double delta_time) {delta_time;};	// Called every tick by owning engine
		bool active = true;						// Should physics apply or not
		double mass = 1.0;						// Mass, Kg
		vec position = { 0.0,0.0,0.0 };			// Position in 3D space, m
		//vec rotation = { 0.0,0.0,0.0 };
		vec velocity = { 0.0,0.0,0.0 };			// Velocity, m/s

	critical:
		engine* owner = nullptr;					// Owning engine
		std::vector<component*> components = {};	// Attached components
		collision_skeleton* collision = nullptr;
		unsigned long long layer = 0;


	public:
		object();
		virtual ~object();
		//object(const object& other);			// copy constructor
		object(const object&& other) noexcept = delete;	// move constructor

		/// <summary>
		/// Gets an owning engine for this object.
		/// </summary>
		/// <returns>Owning engine</returns>
		engine* get_owner() const;

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

		unsigned long long get_layer() const;

		void set_layer(unsigned long long new_layer);

		collision_skeleton* get_collision_skeleton() const;

		void set_collision_skeleton(collision_skeleton** col_skel);

		void detach_collision_skeleton();
	};

	/// <summary>
	/// Engine that owns everything. Ticks every object inside and does all calculations.
	/// </summary>
	struct engine
	{
		friend struct object;
		friend struct component;

	public:
		double tick_time = 0.01;						// Desired time between ticks, s
		vec gravity = { 0.0,0.0,-9.8 };
		std::function<void(std::vector<object*>&, double)> physics_tick;

	critical:
		timer clock;									// Clock
		std::map<unsigned long long, std::vector<object*>> layers = {};


	public:
		engine();
		virtual ~engine();

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

		/// <summary>
		/// Gets all simple objects from this engine.
		/// </summary>
		/// <returns>Array of owned simple objects</returns>
		const std::vector<object*>& get_objects(unsigned long long selected_layer = 0) const;

		const std::map<unsigned long long, std::vector<object*>>& get_all_layers() const;
	};

}