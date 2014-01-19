#ifndef __VECTOR2_H__
#define __VECTOR2_H__


class Vector2
{
public:
	float x;
	float y;

	// Default constructor.
	Vector2();

	Vector2(const float ix, const float iy);
	
	// Copy constructor.
	Vector2(const Vector2 &v);

	void zero();
	
	void set(const float ix, const float iy);

	void set(const Vector2 &v);
	
	// Obtains the dot product between the current vector and the parameter vector.
	float dot(Vector2 vb) const;
	
	float cross(const Vector2 &v) const;
	
	// Obtains the normal for the current vector - which is the dot product of itself
	float norm();

	// Obtains the length of the current vector.
	float length();

	// Normalizes the current vector.
	Vector2& normalize();

	Vector2 getInverse();

	bool IsZero();

	// Operator overloads for more convenient math operations.
	Vector2 &operator=(const Vector2 &v) { x=v.x; y=v.y; return *this; }
	Vector2 &operator+=(const Vector2 &v) { x+=v.x; y+=v.y; return *this; }
	Vector2 &operator-=(const Vector2 &v) { x-=v.x; y-=v.y; return *this; }
	template <typename T> Vector2 operator*=(const T &s){ x*=s; y*=s; return *this; }
	template <typename T> Vector2 operator/=(const T &s){ x/=s; y/=s; return *this; }
	const Vector2 operator+(const Vector2 &v) const { return Vector2(x+v.x, y+v.y); }
	const Vector2 operator-(const Vector2 &v) const { return Vector2(x-v.x, y-v.y); }
	template <typename T> const Vector2 operator*(const T &s) const{ return Vector2(x*s, y*s);	}
	template <typename T> const Vector2 operator/(const T &s) const{ return Vector2(x/s, y/s);	}
	const bool operator == (const Vector2 &v) const { return (x == v.x) && (y == v.y); }
	const bool operator != (const Vector2 &v) const { return (x != v.x) || (y != v.y); }

	Vector2 &operator*=(const class RotationTranslationMatrix &m);	//for GLMatrix
};
template <typename T> inline const Vector2 operator*(const T &s, const Vector2 &v){	return Vector2(v.x*s, v.y*s);	}


#endif
