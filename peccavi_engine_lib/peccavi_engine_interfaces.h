#pragma once

#include <vector>

//TODO: think about adding is_owned

namespace pe
{
	template <typename ty>
	struct owner_of;

	/// <summary>
	/// Template parameter should be the SAME as class that you are deriving from owner_of.
	/// </summary>
	/// <typeparam name="ty"> - Should be the SAME as class that you are deriving from owner_of</typeparam>
	template <typename ty>
	struct is_owned
	{
		owner_of<ty>* owner = nullptr;

		~is_owned();
	};

	/* When deriving from this class, provide a pointer to vector of owned objects in initializer list when construting derived class.
		Also add "using owner_of<ty>::add;" (same for other functions) if you deriving from several 'owner_of' */
	template <typename ty>
	struct owner_of
	{
		std::vector<ty*>* owned_objs_ptr = nullptr;

		owner_of(std::vector<ty*>* new_owned_objs_ptr)
		{
			owned_objs_ptr = new_owned_objs_ptr;
		}

		~owner_of() {}

		/// <summary>
		/// Adds object to 'owner_of'. Takes ownership of this pointer.
		/// </summary>
		/// <param name="owned_ptr"> - pointer to added object, ownership of the pointer is taken by 'owner_of'</param>
		void add(is_owned<ty>* owned_ptr)
		{
			owned_objs_ptr->push_back((ty*)owned_ptr);
			owned_ptr.owner = this;
		}

		void remove(is_owned<ty>* owned_ptr)
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
	};

	template<typename ty>
	inline is_owned<ty>::~is_owned()
	{
		if (owner)
		{
			owner->remove(this);
		}
	}

}