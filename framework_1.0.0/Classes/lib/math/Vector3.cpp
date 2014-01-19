#include <math.h>
#include "matrix4x4.h"
#include "vector2.h"
#include "vector3.h"


Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}


Vector3::Vector3(const float ix, const float iy, const float iz)
{
	x = ix;
	y = iy;
	z = iz;	
}


Vector3::Vector3(const Vector2 &vec2, const float iz)
{
	x = vec2.x;
	y = vec2.y;
	z = iz;	
}


Vector3::Vector3(const Vector3 &vec3)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;	
}


void Vector3::set(const Vector3 &vec3)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}


void Vector3::set(const float ix, const float iy, const float iz)
{
	x = ix;
	y = iy;
	z = iz;	
}


Vector3& Vector3::ApplyMatrix(Matrix4X4 &m)
{
	Vector3 tmp(*this);
	*this = m * (tmp);

	return *this;
}


float Vector3::dot(Vector3 v) const
{
	return (x*v.x + y*v.y + z*v.z);
}


float Vector3::norm()
{
	return (x*x + y*y + z*z);
}


float Vector3::length()
{
	return sqrtf(x*x + y*y + z*z);
}


Vector3& Vector3::normalize()
{
	float len = length();
	
	if (len != 0.0f)
	{
		x /= len;
		y /= len;
		z /= len;
	}
	
	return *this;
}


Vector3 Vector3::getInverse()
{
	return Vector3(-x, -y, -z);
}


void Vector3::zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;	
}


bool Vector3::IsZero()
{
	return (x == 0.0f) && (y == 0.0f) && (z == 0.0f);
}


Vector3 Vector3::cross(const Vector3 &v) const
{
	return Vector3(
		y * v.z - z * v.y, 
		z * v.x - x * v.z, 
		x * v.y - y * v.x);
}