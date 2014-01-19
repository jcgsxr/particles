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

// These are benchmarking defines. Both will print information on the screen.
#define ENABLE_FPS
#define ENABLE_POLY_COUNT

// Enable this to rendering every physics frame, enabling all frames to be visible, but time-innaccurate.
#define ENABLE_PHYSICS_FRAMES_ALL

// These enable different testing in unittesting.
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

// General tests.
//#define ENABLE_UNITTESTING

// Used to test 3d polygon rendering.
//#define ENABLE_POLYTEST

// If the app doesn't play sound, then this can be disabled.
//#define ENABLE_SOUNDENGINE

#define ENABLE_PARTICLESYSTEM

//#define LANDSCAPE_MODE

#define ENABLE_3D	1
#define ENABLE_TRIANGLE_QUADS	1
//#define GL_ATTENUATION_NOT_SUPPORTED

#define DPRINT_TILESYSTEM(...)	//printf(__VA_ARGS__)
#define DPRINT_MAIN(...)		//printf(__VA_ARGS__)

// Enables the opengl menu and disables the IB modal menu. Use this if working on a different platform than iphone.
//#define ENABLE_OGL_MENU

//static const char* SAVE_FILENAME = "save.dat";

#endif