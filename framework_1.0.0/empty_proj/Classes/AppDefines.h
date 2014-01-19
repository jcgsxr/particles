/*
 *  AppDefines.h
 *  framework
 *
 *  Created by Johnny Chan on 11/9/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */

#ifndef __APPDEFINES_H__
#define __APPDEFINES_H__

// These are app specific defines.
// We decide which systems to enable here.

//#define DEBUG

//#define ENABLE_FPS

//#define ENABLE_PHYSICS_DEBUG
//#define ENABLE_PARTICLE_DEBUG
//#define ENABLE_SPRITE_DEBUG

//#define ENABLE_MENU_SYSTEM
#define ENABLE_IMAGELOADER_SYSTEM
// App delegate must handle this if it is disabled or the app will crash since _window is a touchsystem item.
//#define ENABLE_TOUCH_SYSTEM

#define ENABLE_MAINSCREEN
#define ENABLE_TITLESCREEN
#define ENABLE_SPLASHSCREEN
//#define ENABLE_UNITTESTING
//#define ENABLE_POLYTEST
#define ENABLE_SOUNDENGINE

//#define ENABLE_PARTICLESYSTEM
//#define ENABLE_OPENFEINT

//#define LANDSCAPE_MODE

#define ENABLE_3D	0
#define ENABLE_TRIANGLE_QUADS	1


#define DPRINT_TILESYSTEM(...)	//printf(__VA_ARGS__)
#define DPRINT_MAIN(...)		//printf(__VA_ARGS__)


static const char* SAVE_FILENAME = "save.dat";

#endif