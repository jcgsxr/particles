#ifndef __VECTOR4_H__
#define __VECTOR4_H__


class Vector2;
class Vector3;
class Matrix4X4;

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	
	// Default constructor.
	Vector4();

	// Default constructor.
	Vector4(const float ix, const float iy, const float iz, const float iw);

	Vector4(const Vector3 &v, const float iw);

	Vector4(const Vector4 &v);

	void set(const float ix, const float iy, const float iz, const float iw);

	void setZero();

	float dot3(Vector4 &v);

	float dot(Vector4 &v);

	float norm();

	float length();

	float length3();

	Vector4& normalize();

	Vector4& normalize3();

	Vector4 cross3(Vector4 &v);

	Vector4& ApplyMatrix(Matrix4X4 &m);

	Vector2 xy();

	Vector3 xyz();

	// Operator overloads for more convenient math operations.
	Vector4 &operator=(const Vector4 &v);
	Vector4 &operator+=(const Vector4 &v) { x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
	Vector4 &operator-=(const Vector4 &v) { x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }
	template <typename T> Vector4 &operator*=(const T &s) { x*=s; y*=s; z*=s; w*=s; return *this; }
	template <typename T> Vector4 &operator/=(const T &s) { x/=s; y/=s; z/=s; w*=s; return *this; }
	const Vector4 operator+(const Vector4 &v) const { return Vector4(x+v.x, y+v.y, z+v.z, w+v.w); }
	const Vector4 operator-(const Vector4 &v) const { return Vector4(x-v.x, y-v.y, z-v.z, w-v.w); }
	template <typename T> const Vector4 operator*(const T &s) const{ return Vector4(x*s, y*s, z*s, w*s);	}
	template <typename T> const Vector4 operator/(const T &s) const{ return Vector4(x/s, y/s, z/s, w/s);	}
	const bool operator == (const Vector4 &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	const bool operator != (const Vector4 &v) const { return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w); }
};

template <typename T> inline const Vector4 operator*(const T &s, const Vector4 &v){	return Vector4(v.x*s, v.y*s, v.z*s, v.w*s);	}

#endif