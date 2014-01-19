#ifndef __ROTATIONQUATERNION_H__
#define __ROTATIONQUATERNION_H__


#include "quaternion.h"

class Vector3;
class RotationTranslationMatrix;

class RotationQuaternion : public Quaternion
{
public:
	// Default constructor.
	RotationQuaternion();
	
	RotationQuaternion(const RotationQuaternion &src);

	RotationQuaternion(const float ia, const Vector3 &iv);

	RotationQuaternion(const float ia, const float ix, const float iy, const float iz);

	RotationQuaternion(const RotationTranslationMatrix &m);
	
	void set(float ia, float ix, float iy, float iz);
	
	void set(float ia, Vector3 &iv);
	
	Vector3 getComponentVector3();
		
	void normalize();
};


#endif