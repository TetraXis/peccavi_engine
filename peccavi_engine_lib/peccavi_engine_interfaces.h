#pragma once

#include <vector>

//TODO: think about adding i_owned

/* When deriving from this class, provide a pointer to vector of owned objects in initializer list when construting derived class.
	Also add "using i_owner<ty>::add;" (same for other functions) if you deriving from several 'i_owner' */
template <typename ty>
struct i_owner
{
	std::vector<ty*>* owned_objs_ptr = nullptr;

	i_owner(std::vector<ty*>* owned_objs_ptr)
	{
		owned_objs_ptr = owned_objs_ptr;
	}

	~i_owner() {}

	/// <summary>
	/// Adds object to 'i_owner'. Takes ownership of this pointer.
	/// </summary>
	/// <param name="owned_ptr"> - pointer to added object, ownership of the pointer is taken by 'i_owner'</param>
	void add(ty* owned_ptr)
	{
		owned_objs_ptr->push_back(owned_ptr);
		set_owner(owned_ptr);
	}

	void remove(ty* owned_ptr)
	{
		for (unsigned long long i = 0; i < owned_objs_ptr->size(); i++)
		{
			if ((*owned_objs_ptr)[i] == owned_ptr)
			{
				owned_objs_ptr->erase(owned_objs_ptr->begin() + i);
				return;
			}
		}
	}

	virtual void set_owner(ty* owned_ptr) = 0;
};