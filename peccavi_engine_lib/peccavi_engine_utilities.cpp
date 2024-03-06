#include "pch.h"
#include "peccavi_engine_utilities.h"

/////////////////////////////////////////////////
// vector3_f
/////////////////////////////////////////////////
//pe::vector3_f::vector3_f(float new_x = 0, float new_y = 0, float new_z = 0)
//	:x(new_x), y(new_y), z(new_z)
//{}
//
//float pe::vector3_f::len() const
//{
//	return sqrt(x * x + y * y + z * z);
//}
//
//pe::vector3_f pe::vector3_f::normalized() const
//{
//	return *this / len();
//}
//
//pe::vector3_f pe::vector3_f::operator-() const
//{
//	return { -x,-y,-z };
//}
//
//pe::vector3_f pe::vector3_f::operator+(const vector3_f& other) const
//{
//	return { x + other.x, y + other.y, z + other.z };
//}
//
//pe::vector3_f pe::vector3_f::operator-(const vector3_f& other) const
//{
//	return { x - other.x, y - other.y, z - other.z };
//}
//
//pe::vector3_f pe::vector3_f::operator*(const vector3_f& other) const
//{
//	return { x * other.x, y * other.y, z * other.z };
//}
//
//pe::vector3_f pe::vector3_f::operator/(const vector3_f& other) const
//{
//	return { x / other.x, y / other.y, z / other.z };
//}
//
//pe::vector3_f pe::vector3_f::operator*(float modifier) const
//{
//	return { x * modifier , y * modifier ,z * modifier };
//}
//
//pe::vector3_f pe::vector3_f::operator/(float modifier) const
//{
//	return { x / modifier , y / modifier ,z / modifier };
//}

/////////////////////////////////////////////////
// vector3_d
/////////////////////////////////////////////////
pe::vector3_d::vector3_d(double new_x, double new_y, double new_z)
	:x(new_x), y(new_y), z(new_z)
{}

double pe::vector3_d::len() const
{
	return sqrt(x * x + y * y + z * z);
}

pe::vector3_d pe::vector3_d::normalized() const
{
	return *this / len();
}

void pe::vector3_d::normalize()
{
	double length = len();
	x /= length;
	y /= length;
	z /= length;
}

bool pe::vector3_d::compare(const vector3_d& other, double tolerancy) const
{
	return (*this - other).len() <= tolerancy;
}

pe::vector3_d pe::vector3_d::operator-() const
{
	return { -x,-y,-z };
}

pe::vector3_d pe::vector3_d::operator+(const vector3_d& other) const
{
	return { x + other.x, y + other.y, z + other.z };
}

pe::vector3_d pe::vector3_d::operator-(const vector3_d& other) const
{
	return { x - other.x, y - other.y, z - other.z };
}

pe::vector3_d pe::vector3_d::operator*(const vector3_d& other) const
{
	return { x * other.x, y * other.y, z * other.z };
}

pe::vector3_d pe::vector3_d::operator/(const vector3_d& other) const
{
	return { x / other.x, y / other.y, z / other.z };
}

pe::vector3_d pe::vector3_d::operator*(double modifier) const
{
	return { x * modifier , y * modifier ,z * modifier };
}

pe::vector3_d pe::vector3_d::operator/(double modifier) const
{
	return { x / modifier , y / modifier ,z / modifier };
}

void pe::vector3_d::operator+=(const vector3_d& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void pe::vector3_d::operator-=(const vector3_d& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

double pe::distance(const vector3_d a, const vector3_d b)
{
	return sqrt((a.x - b.x) + (a.y - b.y) + (a.z - b.z));
}

double pe::distance_squared(const vector3_d a, const vector3_d b)
{
	return (a.x - b.x) + (a.y - b.y) + (a.z - b.z);
}
