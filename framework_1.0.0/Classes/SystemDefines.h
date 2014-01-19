#ifndef __SYSTEMDEFINES_H__
#define __SYSTEMDEFINES_H__

#include <stdio.h>
#include "AppDefines.h"


#define DPRINT_ENGINE(...)		//printf(__VA_ARGS__)
#define DPRINT_GAMEOBJECT(...)	//printf(__VA_ARGS__)
#define DPRINT_SPRITE(...)		//printf(__VA_ARGS__)
#define DPRINT_PHYSICS(...)		//printf(__VA_ARGS__)
#define DPRINT_TITLE(...)		//printf(__VA_ARGS__)
#define DPRINT_TOUCH(...)		//printf(__VA_ARGS__)
#define DPRINT_GRAPHICS(...)	//printf(__VA_ARGS__)
#define DPRINT_PARTICLESYS(...)	//printf(__VA_ARGS__)
#define DPRINT_MENUSYS(...)		printf(__VA_ARGS__)
#define DPRINT_IMAGE(...)		printf(__VA_ARGS__)
#define DPRINT_SOUND(...)		printf(__VA_ARGS__)
#define DPRINT_CAMERA(...)		printf(__VA_ARGS__)
#define DPRINT_LIST(...)		printf(__VA_ARGS__)
#define DPRINT_REFERENCE(...)	printf(__VA_ARGS__)
#define DPRINT_FLOATBUFFER(...)	printf(__VA_ARGS__)
#define DPRINT_HASHTABLE(...)	printf(__VA_ARGS__)
#define DPRINT_STRINGI(...)		printf(__VA_ARGS__)
#define DPRINT_STRINGD(...)		printf(__VA_ARGS__)

//#define ENABLE_PNGLOAD

#define FRAME_INTERVAL	(1.0/60.0)

// These are not #defined because it makes it harder to look at these values during debug.
#if defined (LANDSCAPE_MODE)
extern int SCRN_W;
extern int SCRN_H;
extern int SCRN_D;
extern int HALF_SCRN_W;
extern int HALF_SCRN_H;
extern int HALF_SCRN_D;
#define ORTHO_LEFT_3D	(-1.5)
#define ORTHO_RIGHT_3D	(1.5)
#define ORTHO_TOP_3D	(1.0)
#define ORTHO_BOTTOM_3D	(-1.0)
#define ORTHO_NEAR_3D	(-1.0)
#else
extern int SCRN_W;
extern int SCRN_H;
extern int SCRN_D;
extern int HALF_SCRN_W;
extern int HALF_SCRN_H;
extern int HALF_SCRN_D;
#define ORTHO_LEFT_3D	(-1.0)
#define ORTHO_RIGHT_3D	(1.0)
#define ORTHO_TOP_3D	(1.5)
#define ORTHO_BOTTOM_3D	(-1.5)
#define ORTHO_NEAR_3D	(-1.0)
#endif



// These are only used in 3d mode.
#define ORTHO_FAR_3D	((float)SCRN_D)
#define PERSPECTIVE_FOVY	(90.0)
#define PERSPECTIVE_ASPECT	((float)SCRN_W / (float)SCRN_H)
#define PERSPECTIVE_NEAR_CLIP	(0.1)
#define PERSPECTIVE_FAR_CLIP	((float)SCRN_D)

#define SAFE_DEL(a)	if(a){delete a; a = NULL;}
#define SAFE_DEL_ARRAY(a) if(a){delete[] a; a = NULL;}

#endif