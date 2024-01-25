#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <string>
#include <vector>

#include "peccavi_engine_utilities.h"
#include "peccavi_engine_components.h"
#include "timer.h"

// Use this define if you want stable tps, 
// engine will slow down if it cant reach desired tps. 
// If not defined engine will skip ticks.

//#define PE_FORCE_STABLE_TPS

#ifdef PE_FORCE_STABLE_TPS 
#define PE_DELTA_TIME tick_time
#else 
#define PE_DELTA_TIME clock.elapsed_seconds()
#endif

namespace pe
{
	typedef vector3_d vec;

	/// <summary>
	/// Describes an object in 3D space
	/// </summary>
	/// TODO: Play with alignas(...)
	///   V
	struct object
	{
		// When adding members here, dont forget to update constructors
		std::string name = "object";		// Name, can be depricated
		vec position = { 0.0,0.0,0.0 };		// Position in 3D space, m
		//vec rotation
		vec velocity = { 0.0,0.0,0.0 };		// Velocity, m/s
		std::vector<component> components = {};

		object();
		object(const object& other);	// copy constructor
		object(const object&& other) noexcept;	// move constructor

		void tick(double delta_time) {}
	};

	/// <summary>
	/// Engine that owns everything. Ticks every object inside and does all calculations.
	/// </summary>
	struct engine
	{
		// Prefer using methods instead of directly changing variables
		double tick_time = 0.01;			// Desired time between ticks, s
		timer clock;						
		std::vector<object> objects = {};	

		engine();

		void start();
		void stop();
		void loop();
		bool is_running();

	};

}