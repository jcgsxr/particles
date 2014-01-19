#include <math.h>
#include "vector2.h"


Vector2::Vector2()
{
	x = 0;
	y = 0;
}


Vector2::Vector2(const float ix, const float iy)
{
	x = ix;
	y = iy;
}


// Copy constructor.
Vector2::Vector2(const Vector2 &v)
{
	x = v.x;
	y = v.y;
}


void Vector2::zero()
{
	x = y = 0;
}


void Vector2::set(const float ix, const float iy)
{
	x = ix;
	y = iy;
}


void Vector2::set(const Vector2 &v)
{
	x = v.x;
	y = v.y;	
}


// Obtains the dot product between the current vector and the parameter vector.
float Vector2::dot(Vector2 vb) const
{
	return (x*vb.x + y*vb.y);
}


float Vector2::cross(const Vector2 &v) const
{
	return x*v.y - y*v.x;
}


// Obtains the normal for the current vector - which is the dot product of itself
float Vector2::norm()
{
	return (x*x + y*y);
}


// Obtains the length of the current vector.
float Vector2::length()
{
	return sqrtf(x*x + y*y);
}


// Normalizes the current vector.
Vector2& Vector2::normalize()
{
	float len = length();
	
	// Basically, don't bother normalizing a zero length vector (since it doesn't matter)
	if (len != 0.0f)
	{
		x /= len;
		y /= len;
	}
	
	return *this;
}


Vector2 Vector2::getInverse()
{
	return Vector2(-x, -y);
}


bool Vector2::IsZero()
{
	return ((x == 0.0f) && (y == 0.0f));
}