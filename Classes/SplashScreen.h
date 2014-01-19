#ifndef __SPLASHSCREEN_H__
#define __SPLASHSCREEN_H__

#import <OpenGLES/ES1/gl.h>
#include "BasicInterface.h"
#include "types.h"
#include "SystemDefines.h"
#include "Sprite.h"

#if defined (DEBUG)
const uint32 SPLASH_FADE_START = 1;
const uint32 SPLASH_FADE_IN = 1;
const uint32 SPLASH_FADE_FULL = 1;
const uint32 SPLASH_FADE_OUT = 1;
const uint32 SPLASH_FADE_END = 1;
#else
const uint32 SPLASH_FADE_START = 200;
const uint32 SPLASH_FADE_IN = 300;
const uint32 SPLASH_FADE_FULL = 1000;
const uint32 SPLASH_FADE_OUT = 300;
const uint32 SPLASH_FADE_END = 200;
#endif

static const int SPLASH_COUNT = 1;	// Change this value for when more than one splash screen is required.
const uint32 SPLASH_DURATION = (SPLASH_FADE_START + SPLASH_FADE_IN + SPLASH_FADE_FULL + SPLASH_FADE_OUT + SPLASH_FADE_END);

typedef enum _eSplashStates
{
	eSplashStateStart = 0,
	eSplashStateFadeIn,
	eSplashStateFadeFull,
	eSplashStateFadeOut,
	eSplashStateFadeOutDone,
	eSplashStateEnd,
} eSplashStates;


class CSplashScreen : public CBasicInterface
{
public:
	int _splash_counter;
	
	CSprite _splash_images[SPLASH_COUNT];
	point _splash_pos[SPLASH_COUNT];
	int _splash_image_x;
	int _splash_image_y;
	
	int _timer;
	int _splash_state;
	
	// The color that will fade the splash screen in.
	color _bg_overlap_color;
	
	// Visible bg color.
	color _bg_color;
	
	CSplashScreen();
	~CSplashScreen();
	
	void init(void);
	void destroy(void);
	void update(void);
	void draw(void);
	void handleTouch(float x, float y, eTouchPhase phase);
	void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2);
};

#endif