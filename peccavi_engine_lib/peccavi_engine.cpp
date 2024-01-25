#include "pch.h"
#include "peccavi_engine.h"

namespace pe
{

	engine::engine()
	{
	}

	void engine::start()
	{
		clock.start();
	}

	void engine::stop()
	{
		clock.stop();
	}

	void engine::loop()
	{
		while (clock.is_running())
		{
			if (clock.elapsed_seconds() >= tick_time)
			{
				for (object& obj : objects)
				{
					obj.tick(PE_DELTA_TIME);

					for (component& comp : obj.components)
					{
						comp.tick(PE_DELTA_TIME);
					}
				}
			}
		}
	}

	bool engine::is_running()
	{
		return clock.is_running();
	}

	object::object()
	{
	}

	object::object(const object& other)
		: name(other.name), position(other.position), velocity(other.velocity), components(other.components)
	{
		for (component& comp : components)
		{
			comp.owner = this;
		}
	}

	object::object(const object&& other) noexcept
		: name(other.name), position(other.position), velocity(other.velocity), components(other.components)
	{
		for (component& comp : components)
		{
			comp.owner = this;
		}
	}
	
}