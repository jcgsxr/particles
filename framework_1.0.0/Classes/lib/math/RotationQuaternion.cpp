#include <math.h>
#include "rotationtranslationmatrix.h"
#include "vector3.h"
#include "rotationquaternion.h"


RotationQuaternion::RotationQuaternion() : Quaternion()
{

}


RotationQuaternion::RotationQuaternion(const RotationQuaternion &src)
{
	a = src.a;
	v = Vector3(src.v);
}


RotationQuaternion::RotationQuaternion(const float ia, const Vector3 &iv)
{
	Quaternion::set(cosf(ia / 2.0f), iv * (sinf(ia / 2.0f)));
}


RotationQuaternion::RotationQuaternion(const float ia, const float ix, const float iy, const float iz)
{
	Quaternion::set( cosf(ia / 2.0f), sinf(ia / 2.0f) * ix, sinf(ia / 2.0f) * iy, sinf(ia / 2.0f) * iz );
}


RotationQuaternion::RotationQuaternion(const RotationTranslationMatrix &m)
{
	float four_xy = (m.m[1] + m.m[4]);
	float four_az = (m.m[1] - m.m[4]);
	float four_xz = (m.m[2] + m.m[8]);
	float four_ay = (m.m[8] - m.m[2]);
	float four_yz = (m.m[6] + m.m[9]);
	float four_ax = (m.m[6] - m.m[9]);

	if (four_xy != 0.0f)
	{
		float four_aa = four_ax * four_ay / four_xy;
		
		if (four_aa > 0.0f)
		{
			float two_a = sqrtf(four_aa);
			float two_x = four_ax / two_a;
			float two_y = four_ay / two_a;
			float two_z = four_az / two_a;
			
			Quaternion::set(two_a, two_x, two_y, two_z);
		}
		else
		{
			float four_zz = four_xz * four_yz / four_xy;
			
			if (four_zz > 0.0f)
			{
				float two_z = sqrtf(four_zz);
				float two_x = four_xz / two_z;
				float two_y = four_yz / two_z;
				
				Quaternion::set(0.0f, two_x, two_y, two_z);
			}
			else
			{
				float x = sqrtf(m.m[0] - m.m[10]);
				float y = sqrtf(m.m[5] - m.m[10]);
				
				Quaternion::set(0.0f, x, y, 0.0f);
			}
		}
	}
	else
	if (four_yz != 0.0f)
	{
		float four_aa = four_ay * four_az / four_yz;
		
		if (four_aa > 0.0f)
		{
			float two_a = sqrtf(four_aa);
			float two_x = four_ax / two_a;
			float two_y = four_ay / two_a;
			float two_z = four_az / two_a;
			
			Quaternion::set(two_a, two_x, two_y, two_z);
		}
		else
		{
			float four_xx = four_xy * four_xz / four_yz;
			
			if (four_xx > 0.0f)
			{
				float two_x = sqrtf(four_xx);
				float two_y = four_xy / two_x;
				float two_z = four_xz / two_x;
				
				Quaternion::set(0.0f, two_x, two_y, two_z);
			}
			else
			{
				float y = sqrtf(m.m[5] - m.m[0]);
				float z = sqrtf(m.m[10] - m.m[0]);
				
				Quaternion::set(0.0f, 0.0f, y, z);
			}
		}
	}
	else
	if (four_xz != 0.0f)
	{
		float four_aa = four_ax * four_az / four_xz;
		
		if (four_aa > 0.0f)
		{
			float two_a = sqrtf(four_aa);
			float two_x = four_ax / two_a;
			float two_y = four_ay / two_a;
			float two_z = four_az / two_a;
			
			Quaternion::set(two_a, two_x, two_y, two_z);
		}
		else
		{
			float four_yy = four_xy * four_yz / four_xz;
			
			if (four_yy > 0.0f)
			{
				float two_y = sqrtf(four_yy);
				float two_x = four_xy / two_y;
				float two_z = four_yz / two_y;
				
				Quaternion::set(0.0f, two_x, two_y, two_z);
			}
			else
			{
				float x = sqrtf(m.m[0] - m.m[5]);
				float z = sqrtf(m.m[10] - m.m[5]);
				
				Quaternion::set(0.0f, x, 0.0f, z);
			}
		}
	}
	else
	if (four_ax != 0.0f)
	{
		float theta = (float)atan2f((double)m.m[6], (double)m.m[5]) / 2.0f;
		Quaternion::set(cosf(theta), sinf(theta), 0.0f, 0.0f);
	}
	else
	if (four_ay != 0.0f)
	{
		float theta = (float)atan2f(m.m[8], m.m[10]) / 2.0f;
		Quaternion::set(cosf(theta), 0.0f, sinf(theta), 0.0f);
	}
	else
	if (four_az != 0.0f)
	{
		float theta = (float)atan2f(m.m[2], m.m[0]) / 2.0f;
		Quaternion::set(cosf(theta), 0.0f, 0.0f, sinf(theta));
	}
	else
	{
		Quaternion::set(1.0f, 0.0f, 0.0f, 0.0f);
	}
	
	normalize();
}


void RotationQuaternion::set(float ia, float ix, float iy, float iz)
{
	Quaternion::set(cosf(ia/2.0f), sinf(ia/2.0f)*ix, sinf(ia/2.0f)*iy, sinf(ia/2.0f)*iz);
}


void RotationQuaternion::set(float ia, Vector3 &iv)
{
	Quaternion::set(cosf(ia/2.0f), iv * (sinf(ia/2.0f)) );
}


Vector3 RotationQuaternion::getComponentVector3()
{
	Vector3 ov((*this).v);
	ov.normalize();
	return ov;
}


void RotationQuaternion::normalize()
{
	float vLength = v.length();
	float angle = (float)atan2f(vLength, a);
	a = cosf(angle);
	
	if (vLength != 0.0f)
	{
		v /= (vLength);		//v /= vLength;	//.normalize();
		v *= (sinf(angle));
	}
}
