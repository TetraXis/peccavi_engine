#include "pch.h"
#include "peccavi_engine_components.h"
#include "peccavi_engine_core.h"

namespace pe
{

	namespace components
	{

		object* component::get_owner() const
		{
			return owner;
		}

		void component::attach_to(object* const owner_object)
		{
			if (owner)
			{
				for (unsigned long long i = 0; i < owner->components.size(); i++)
				{
					if (owner->components[i] == this)
					{
						owner->components.erase(owner->components.begin() + i);
					}
				}
			}
			owner_object->components.push_back(this);
			owner = owner_object;
		}

	}
}