#include <math.h>
#include "matrix4x4.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"


Vector4::Vector4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;	
}


// Default constructor.
Vector4::Vector4(const float ix, const float iy, const float iz, const float iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}


Vector4::Vector4(const Vector3 &v, const float iw)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = iw;
}


Vector4::Vector4(const Vector4 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;	
}


void Vector4::set(const float ix, const float iy, const float iz, const float iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}


void Vector4::setZero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}


float Vector4::dot3(Vector4 &v)
{
	return x*v.x + y*v.y + z*v.z;
}


float Vector4::dot(Vector4 &v)
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}


float Vector4::norm()
{
	return x*x + y*y + z*z + w*w;
}


float Vector4::length()
{
	return sqrtf(x*x + y*y + z*z + w*w);
}


float Vector4::length3()
{
	return sqrtf(x*x + y*y + z*z);
}


Vector4& Vector4::normalize()
{
	float len = length();
	
	if (len != 0)
	{
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
	
	return *this;
}


Vector4& Vector4::normalize3()
{
	float len = length3();
	
	if (len != 0)
	{
		x /= len;
		y /= len;
		z /= len;
	}	
	
	return *this;
}


Vector4 Vector4::cross3(Vector4 &v)
{
	return Vector4(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x, 0.0f);
}


Vector4& Vector4::ApplyMatrix(Matrix4X4 &m)
{
	Vector4 tmp = m * (*this);
	x = tmp.x;
	y = tmp.y;
	z = tmp.z;
	w = tmp.w;	
	
	return *this;
}


Vector2 Vector4::xy()
{
	return Vector2(x, y);
}


Vector3 Vector4::xyz()
{
	return Vector3(x, y, z);
}