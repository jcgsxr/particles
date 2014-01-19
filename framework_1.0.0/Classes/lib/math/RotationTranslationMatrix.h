#ifndef __ROTATIONTRANSLATIONMATRIX_H__
#define __ROTATIONTRANSLATIONMATRIX_H__

#include "matrix4x4.h"


class RotationQuaternion;
class Vector3;

class RotationTranslationMatrix : public Matrix4X4
{
public:
	// Default constructor.
	RotationTranslationMatrix();
	
	RotationTranslationMatrix(
		const float m11, const float m12, const float m13, const float m14, 
		const float m21, const float m22, const float m23, const float m24, 
		const float m31, const float m32, const float m33, const float m34, 
		const float m41, const float m42, const float m43, const float m44);
	
	RotationTranslationMatrix(const Matrix4X4 &im);

	RotationTranslationMatrix(const RotationQuaternion &q);

	void SetPos(const Vector3 &v);
	
	Vector3 GetRight();

	Vector3 GetUp();

	Vector3 GetAt();

	Vector3 GetPos();

	void setRotate(const RotationQuaternion &q);
};


#endif