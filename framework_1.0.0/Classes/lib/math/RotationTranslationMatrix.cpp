#include "vector3.h"
#include "matrix4x4.h"
#include "rotationquaternion.h"
#include "rotationtranslationmatrix.h"


RotationTranslationMatrix::RotationTranslationMatrix() : Matrix4X4()
{

}


RotationTranslationMatrix::RotationTranslationMatrix(const float m11, const float m12, const float m13, const float m14, 
													 const float m21, const float m22, const float m23, const float m24, 
													 const float m31, const float m32, const float m33, const float m34, 
													 const float m41, const float m42, const float m43, const float m44) : 	
Matrix4X4(
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44)
{

}


RotationTranslationMatrix::RotationTranslationMatrix(const Matrix4X4 &im) : Matrix4X4(im)
{

}


RotationTranslationMatrix::RotationTranslationMatrix(const RotationQuaternion &q)
{
	setRotate(q);
}


void RotationTranslationMatrix::SetPos(const Vector3 &v)
{
	m[12] = v.x;
	m[13] = v.y;
	m[14] = v.z;
}


Vector3 RotationTranslationMatrix::GetRight()
{
	return Vector3(m[0], m[1], m[2]);
}


Vector3 RotationTranslationMatrix::GetUp()
{
	return Vector3(m[4], m[5], m[6]);
}


Vector3 RotationTranslationMatrix::GetAt()
{
	return Vector3(m[8], m[9], m[10]);
}


Vector3 RotationTranslationMatrix::GetPos()
{
	return Vector3(m[12], m[13], m[14]);
}


void RotationTranslationMatrix::setRotate(const RotationQuaternion &q)
{
	float xx = q.v.x*q.v.x;
	float xy = q.v.x*q.v.y;
	float xz = q.v.x*q.v.z;
	
	float yy = q.v.y*q.v.y;
	float yz = q.v.y*q.v.z;
	
	float zz = q.v.z*q.v.z;
	
	float ax = q.a*q.v.x;
	float ay = q.a*q.v.y;
	float az = q.a*q.v.z;
	
	m[0] = 1 - 2 * ( yy + zz );
	m[1] =     2 * ( xy + az );
	m[2] =     2 * ( xz - ay );
	m[3] = 0;
	
	m[4] =     2 * ( xy - az );
	m[5] = 1 - 2 * ( zz + xx );
	m[6] =     2 * ( yz + ax );
	m[7] = 0;
	
	m[8] =     2 * ( xz + ay );
	m[9] =     2 * ( yz - ax );
	m[10] = 1 - 2 * ( yy + xx );
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}
