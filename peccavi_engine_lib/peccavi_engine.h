#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <memory>

#include "peccavi_engine_literals.h"
#include "peccavi_engine_utilities.h"
#include "peccavi_engine_interfaces.h"
#include "peccavi_engine_components.h"
#include "peccavi_engine_collision.h"
#include "peccavi_engine_core.h"
#include "timer.h"

// Use this define if you want stable tps, 
// engine will slow down if it cant reach desired tps. 
// If not defined engine will skip ticks.
// #define PE_FORCE_STABLE_TPS

// Use this define if you want ALL members of ALL classes to be public.
// WARNING. Newly-public members should be handled carefully, they HAVE SIDE EFFECTS.
// #define PE_ALL_MEMBERS_PUBLIC

namespace pe
{

}