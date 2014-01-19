#ifndef __QUATERNION_H__
#define __QUATERNION_H__


class Vector3;

class Quaternion
{
public:

	float a;
	Vector3 v;
		
	
	// Default constructor.
	Quaternion();	
	
	Quaternion(const float ia, const float ix, const float iy, const float iz);
	
	Quaternion(const float ia, const Vector3 &iv);
	
	Quaternion(const Quaternion &q);

	void set(const float ia, const float ix, const float iy, const float iz);

	void set(const float ia, const Vector3 &iv);

	void setZero();
	
	void setIdentity();
	
	Quaternion conjugate();

	float norm();

	Quaternion inverse();

	Quaternion divideByFloat(float s);
	
	Quaternion multiplyAssignByQuaternion(Quaternion &q);

	Quaternion multiplyByQuaternion(Quaternion &q);

	// Operator overloads for more convenient math operations.
	const Quaternion	&operator=(const Quaternion &q);
	const Quaternion	&operator+=(const Quaternion &q);
	const Quaternion	&operator-=(const Quaternion &q);
	const Quaternion	&operator*=(const Quaternion &q);
	const Quaternion	operator+() const;
	const Quaternion	operator-() const;
	const Quaternion	operator+(const Quaternion &q) const;
	const Quaternion	operator-(const Quaternion &q) const;
	Quaternion	operator*(const Quaternion &q) const;
	const Quaternion	operator*=(const float s){	a *= s; v *= s; return *this; }
	const Quaternion	operator/=(const float s){	a /= s; v /= s; return *this; }
	const Quaternion	operator*(const float s) const{	return Quaternion(*this) *= s;	}
	const Quaternion	operator/(const float s) const{	return Quaternion(*this) /= s;	}
};
inline const Quaternion operator*(const float s, const Quaternion &q){	return q*s;	}


#endif
