#ifndef __VECTOR3_H__
#define __VECTOR3_H__


class Vector2;
class Matrix4X4;

class Vector3
{
public:
	float x;
	float y;
	float z;
	
	// Default constructor.
	Vector3();
	
	Vector3(const float ix, const float iy, const float iz);

	Vector3(const Vector2 &vec2, const float iz);

	Vector3(const Vector3 &vec3);

	void set(const Vector3 &vec3);

	void set(const float ix, const float iy, const float iz);

	Vector3& ApplyMatrix(Matrix4X4 &m);

	float dot(Vector3 v) const;

	float norm();

	float length();

	Vector3& normalize();

	Vector3 getInverse();

	void zero();

	bool IsZero();
	
	Vector3 cross(const Vector3 &v) const;

	// Operator overloads for more convenient math operations.
	const Vector3 operator+() const { return *this; }
	const Vector3 operator-() const { return Vector3(-x, -y, -z); }
	Vector3& operator=(const Vector3 &v) { x = v.x; y = v.y; z = v.z; return *this; }
	Vector3& operator+=(const Vector3 &v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	Vector3& operator-=(const Vector3 &v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
	template <typename T> Vector3 &operator*=(const T &s) { x*=s; y*=s; z*=s; return *this; }
	template <typename T> Vector3 &operator/=(const T &s) { x/=s; y/=s; z/=s; return *this; }
	const Vector3 operator+(const Vector3 &v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
	const Vector3 operator-(const Vector3 &v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
	template <typename T> const Vector3 operator*(const T &s) const{ return Vector3(x*s, y*s, z*s);	}
	template <typename T> const Vector3 operator/(const T &s) const{ return Vector3(x/s, y/s, z/s);	}
	const bool operator == (const Vector3 &v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	const bool operator != (const Vector3 &v) const { return (x != v.x) || (y != v.y) || (z != v.z); }
	//Vector3& operator=(int i) { x = (float)i; y = (float)i; z = (float)i; return *this; }
	//Vector3& operator=(float f) { x = f; y = f; z = f; return *this; }
};
template <typename T> inline const Vector3 operator*(const T &s, const Vector3 &v){	return Vector3(v.x*s, v.y*s, v.z*s);	}

#endif