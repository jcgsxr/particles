#include <string.h>
#include <math.h>
#include "MathUtil.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"


Matrix4X4::Matrix4X4()
{
	// Start with an identity matrix by default.
	identity();
}


Matrix4X4::Matrix4X4(	
					 const float _a11, const float _a12, const float _a13, const float _a14,
					 const float _a21, const float _a22, const float _a23, const float _a24,
					 const float _a31, const float _a32, const float _a33, const float _a34,
					 const float _a41, const float _a42, const float _a43, const float _a44)
{
	m[0]  = _a11;	m[4]  = _a12;	m[8]  = _a13;	m[12]  = _a14;
	m[1]  = _a21;	m[5]  = _a22;	m[9]  = _a23;	m[13]  = _a24;
	m[2]  = _a31;	m[6]  = _a32;	m[10] = _a33;	m[14] = _a34;
	m[3] = _a41;	m[7] = _a42;	m[11] = _a43;	m[15] = _a44;

	
}


Matrix4X4::Matrix4X4(const Matrix4X4 &src)
{
	memcpy(&m, &src, sizeof(float) * 16);	
}


void Matrix4X4::set(
					const float _a11, const float _a12, const float _a13, const float _a14,
					const float _a21, const float _a22, const float _a23, const float _a24,
					const float _a31, const float _a32, const float _a33, const float _a34,
					const float _a41, const float _a42, const float _a43, const float _a44)
{
	m[0]  = _a11;	m[4]  = _a12;	m[8]  = _a13;	m[12] = _a14;
	m[1]  = _a21;	m[5]  = _a22;	m[9]  = _a23;	m[13] = _a24;
	m[2]  = _a31;	m[6]  = _a32;	m[10] = _a33;	m[14] = _a34;
	m[3] = _a41;	m[7] = _a42;	m[11] = _a43;	m[15] = _a44;
}


void Matrix4X4::set(const int *fixedPointMat)
{
	m[0] = XPTOFP(fixedPointMat[0]);
	m[1] = XPTOFP(fixedPointMat[1]);
	m[2] = XPTOFP(fixedPointMat[2]);
	m[3] = XPTOFP(fixedPointMat[3]);
	m[4] = XPTOFP(fixedPointMat[4]);
	m[5] = XPTOFP(fixedPointMat[5]);
	m[6] = XPTOFP(fixedPointMat[6]);
	m[7] = XPTOFP(fixedPointMat[7]);
	m[8] = XPTOFP(fixedPointMat[8]);
	m[9] = XPTOFP(fixedPointMat[9]);
	m[10] = XPTOFP(fixedPointMat[10]);
	m[11] = XPTOFP(fixedPointMat[11]);
	m[12] = XPTOFP(fixedPointMat[12]);
	m[13] = XPTOFP(fixedPointMat[13]);
	m[14] = XPTOFP(fixedPointMat[14]);
	m[15] = XPTOFP(fixedPointMat[15]);
//	m[16] = XPTOFP(fixedPointMat[16]);
}


void Matrix4X4::set(const Matrix4X4 &src)
{
	memcpy(&m, &src, sizeof(float) * 16);	
}


void Matrix4X4::RotationXYZ(float x, float y, float z)
{
	float cx = (float)cosf(x);
	float sx = (float)sinf(x);
	float cy = (float)cosf(y);
	float sy = (float)sinf(y);
	float cz = (float)cosf(z);
	float sz = (float)sinf(z);
	
	m[0] = cy*cz;				m[4] =-cy*sz;				m[8] = sy;		m[12] = 0.0f;
	m[1] =-cx*sz + sx*sy*cz;	m[5] = cx*cz - sx*sy*sz;	m[9] =-sx*cy;	m[13] = 0.0f;
	m[2] = sx*sz - cx*sy*cz;	m[6] = sx*cz + cx*sy*sz;	m[10] = cx*cy;	m[14] = 0.0f;
	m[3] = 0.0f;				m[7] = 0.0f;				m[11] = 0.0f;	m[15] = 1.0f;
}


void Matrix4X4::identity()
{
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 1.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;	
}


Matrix4X4 Matrix4X4::transposed()
{
	return Matrix4X4(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]);
}


Matrix4X4& Matrix4X4::transpose()
{
	set(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
	
	return *this;
}


float Matrix4X4::trace()
{
	return m[0] + m[5] + m[10] + m[15];
}


float Matrix4X4::determinant()
{
	return	 m[0]*(m[5]*(m[10]*m[15] - m[14]*m[11]) + m[9]*(m[6]*m[15] - m[14]*m[7]) + m[13]*(m[6]*m[11] - m[10]*m[7]))
			-m[4]*(m[1]*(m[10]*m[15] - m[14]*m[11]) + m[9]*(m[2]*m[15] - m[14]*m[3]) + m[13]*(m[2]*m[11] - m[10]*m[3]))
			+m[8]*(m[1]*(m[6]*m[15] - m[14]*m[7]) + m[5]*(m[2]*m[15] - m[14]*m[3]) + m[13]*(m[6]*m[3] - m[2]*m[7]))
			-m[12]*(m[1]*(m[6]*m[11] - m[10]*m[7]) + m[5]*(m[2]*m[11] - m[10]*m[3]) + m[9]*(m[2]*m[7] - m[6]*m[3]));
}


void Matrix4X4::setColumn(const int n, const Vector4 v)
{
	m[0+4*n] = v.x;
	m[1+4*n] = v.y;
	m[2+4*n] = v.z;
	m[3+4*n] = v.w;
}


Vector4 Matrix4X4::getColumn(int n)
{
	return Vector4
		(
			m[0+4*n],
			m[1+4*n],
			m[2+4*n],
			m[3+4*n]
		); 
}


void Matrix4X4::setRow(int n, const Vector4 &v)
{
	m[n+0] = v.x;	m[n+4] = v.y;	m[n+8] = v.z;	m[n+12] = v.w;
}


Vector4 Matrix4X4::getRow(int n)
{
	return Vector4(m[n+0], m[n+4], m[n+8], m[n+12]);
}


void Matrix4X4::ApplyMatrix(const Matrix4X4 &im)
{
	Matrix4X4 tmp;
	
	tmp.setColumn(0, im * getColumn(0) );
	tmp.setColumn(1, im * getColumn(1) );
	tmp.setColumn(2, im * getColumn(2) );
	tmp.setColumn(3, im * getColumn(3) );
	
	set(tmp);
}


const Vector3 Matrix4X4::operator* (const Vector3 &v) const
{
	return Vector3
		(
			m[0] * v.x + m[4] * v.y + m[8] * v.z,
			m[1] * v.x + m[5] * v.y + m[9] * v.z,
			m[2] * v.x + m[6] * v.y + m[10] * v.z
		);
}


const Vector4 Matrix4X4::operator* (const Vector4 &v) const
{
	return Vector4
		(
			m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
			m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
			m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
			m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
		);
}