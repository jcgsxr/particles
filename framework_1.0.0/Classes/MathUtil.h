/*
 *  Math.h
 *  framework
 *
 *  Created by Johnny Chan on 10/11/09.
 *  Copyright 2009 Llamaface. All rights reserved.
 *
 */

#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__

//#include "types.h"


#define FIXED_SHIFT_ONE	(16)
#define FIXED_SHIFT_ONE_HALF	(15)

#define FIXED_ONE		(1 << 16)
#define FIXED_ONE_HALF	(1 << 15)
#define FPTOXP(a)		(a * 65536.0f)
#define XPTOFP(a)		((float)a / 65536.0f)

#define FLOAT_QUARTER_PI (0.78539816f)
#define FLOAT_HALF_PI    (1.57079632f)
#define FLOAT_PI         (3.14159265f)
#define FLOAT_2_PI       (6.28318530f)

#define FIXED_QUARTER_PI (0x0C90F)
#define FIXED_HALF_PI    (0x1921F)
#define FIXED_PI         (0x3243F)
#define FIXED_2_PI       (0x6487E)

#define WORLD_TO_SCREEN(x)	(x >> FIXED_SHIFT_ONE)
#define SCREEN_TO_WORLD(x)	(x << FIXED_SHIFT_ONE)

//#define M_PI	(3.14159265358979323846264338327950288) // PI
#define DEGREES_TO_RADIANS(x)	((x * M_PI) / 180.0)
#define RADIANS_TO_DEGREES(x)	((x * 180) / M_PI)


/* Computes the inner product between vectors v and q */ 

#define mathsInnerProduct(v,q) \
((v)[0] * (q)[0] + \
(v)[1] * (q)[1] + \
(v)[2] * (q)[2])	


/* a = b x c */

#define mathsCrossProduct(a,b,c) \
(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];


/* vector a = b - c, where b and c represent points*/

#define mathsVector(a,b,c) \
(a)[0] = (b)[0] - (c)[0];	\
(a)[1] = (b)[1] - (c)[1];	\
(a)[2] = (b)[2] - (c)[2];



#endif