#pragma once
#include "peccavi_engine_utilities.h"
#include "peccavi_engine_components.h"

namespace pe
{

	namespace components
	{

		typedef vector3_d vec;

		struct collision_sphere : component
		{
			vec origin = { 0.0,0.0,0.0 };		// Offset from owner origin, m
			double radius = 1;					// m

			collision_sphere(const vec& new_origin, double new_radius);
		};

	}

}