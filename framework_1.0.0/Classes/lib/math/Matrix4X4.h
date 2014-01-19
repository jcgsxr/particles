#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__


class Vector3;
class Vector4;

class Matrix4X4
{
public:
	float m[16];
	
	// Default constructor.
	Matrix4X4();
	
	Matrix4X4(	
		const float _a11, const float _a12, const float _a13, const float _a14,
		const float _a21, const float _a22, const float _a23, const float _a24,
		const float _a31, const float _a32, const float _a33, const float _a34,
		const float _a41, const float _a42, const float _a43, const float _a44);	
	
	Matrix4X4(const Matrix4X4 &src);

	void set(
		const float _a11, const float _a12, const float _a13, const float _a14,
		const float _a21, const float _a22, const float _a23, const float _a24,
		const float _a31, const float _a32, const float _a33, const float _a34,
		const float _a41, const float _a42, const float _a43, const float _a44);
		
	void set(const Matrix4X4 &src);

	void set(const int *fixedPointMat);	
	
	void RotationXYZ(float x, float y, float z);
	
	void identity();
	
	Matrix4X4 transposed();
	
	Matrix4X4& transpose();	
	
	float trace();
	
	float determinant();
	
	void setColumn(const int n, const Vector4 v);
	
	Vector4 getColumn(int n);	
	
	void setRow(int n, const Vector4 &v);
	
	Vector4 getRow(int n);
	
	void ApplyMatrix(const Matrix4X4 &im);
	
	const Vector3 operator* (const Vector3 &v) const;
	const Vector4 operator* (const Vector4 &v) const;
};

#endif