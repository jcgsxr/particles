
#include "vector3.h"
#include "quaternion.h"
	


// Default constructor.
Quaternion::Quaternion()
{
	a = 0.0f;
	v = Vector3();
};


Quaternion::Quaternion(float ia, float ix, float iy, float iz)
{
	set(ia, ix, iy, iz);
}


Quaternion::Quaternion(const float ia, const Vector3 &iv)
{
	set(ia, iv);
}


Quaternion::Quaternion(const Quaternion &q)
{
	set(q.a, q.v);
}


void Quaternion::set(const float ia, const float ix, const float iy, const float iz)
{
	a = ia;
	v.set(ix, iy, iz);
}


void Quaternion::set(const float ia, const Vector3 &iv)
{
	a = ia;
	v = iv;
}


void Quaternion::setZero()
{
	Vector3 temp(0, 0, 0);
	set(0.0f, temp);
}


void Quaternion::setIdentity()
{
	Vector3 temp(0, 0, 0);
	set(1.0f, temp);
}


Quaternion Quaternion::conjugate()
{
	return Quaternion(a, v.getInverse());		//quaternion(a, -v);
}


float Quaternion::norm()
{
	return (a*a + v.norm());
}


Quaternion Quaternion::inverse()
{
	return conjugate().divideByFloat(norm());	// conjugate() / norm();
}


Quaternion Quaternion::divideByFloat(float s)
{
	a /= s;
	v /= s;
	return *this;
}


Quaternion Quaternion::multiplyAssignByQuaternion(Quaternion &q)
{	
	float A = (q.a * a) - (q.v.dot(v));
	
	Vector3 V = v * q.a;
	V += (q.v * a);
	V += (q.v.cross(v));
	//V.addAssignByVector3( q.v.multiplyByFloat(a) );
	//V.addAssignByVector3( q.v.cross(v) );
	
	a = A;
	v = V;
	
	return *this;
}


Quaternion Quaternion::multiplyByQuaternion(Quaternion &q)
{
	multiplyAssignByQuaternion(q);
	return *this;
}


const Quaternion &Quaternion::operator=(const Quaternion &q)
{
	set(q.a, q.v);
	return *this;
}


const Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	a += q.a;	v += q.v;
	return *this;
}


const Quaternion &Quaternion::operator-=(const Quaternion &q)
{
	a -= q.a;	v -= q.v;
	return *this;
}


const Quaternion &Quaternion::operator*=(const Quaternion &q)
{
	float A = q.a*a - q.v.dot(v);
	Vector3 V = q.a*v + a*q.v + q.v.cross(v);
	a = A;	v = V;
	return *this;
}


const Quaternion Quaternion::operator+() const
{
	return Quaternion(*this);
}


const Quaternion Quaternion::operator-() const
{
	return Quaternion(-a, -v);
}


const Quaternion Quaternion::operator+(const Quaternion &q) const
{
	return Quaternion(*this) += q;
}


const Quaternion Quaternion::operator-(const Quaternion &q) const
{
	return Quaternion(*this) -= q;
}


Quaternion Quaternion::operator*(const Quaternion &q) const
{
	return Quaternion(*this) *= q;
}