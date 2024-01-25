#pragma once

#include <cmath>

namespace pe
{
	/*struct vector3_f
	{
		float x = 0;
		float y = 0;
		float z = 0;

		vector3_f(float new_x = 0, float new_y = 0, float new_z = 0);

		float len() const;
		vector3_f normalized() const;

		vector3_f operator - () const;
		vector3_f operator + (const vector3_f& other) const;
		vector3_f operator - (const vector3_f& other) const;
		vector3_f operator * (const vector3_f& other) const;
		vector3_f operator / (const vector3_f& other) const;

		vector3_f operator * (float modifier) const;
		vector3_f operator / (float modifier) const;
	};*/

	struct vector3_d
	{
		double x = 0;
		double y = 0;
		double z = 0;

		vector3_d(double new_x = 0.0, double new_y = 0.0, double new_z = 0.0);

		double len() const;
		vector3_d normalized() const;
		void normalize();
		bool compare(const vector3_d& other, double tolerancy = 0.01) const;

		vector3_d operator - () const;
		vector3_d operator + (const vector3_d& other) const;
		vector3_d operator - (const vector3_d& other) const;
		vector3_d operator * (const vector3_d& other) const;
		vector3_d operator / (const vector3_d& other) const;

		vector3_d operator * (double modifier) const;
		vector3_d operator / (double modifier) const;

		void operator += (const vector3_d& other);
		void operator -= (const vector3_d& other);
	};
}