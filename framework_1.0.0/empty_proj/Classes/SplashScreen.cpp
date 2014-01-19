#include <stdio.h>
#include "Sprite.h"
#include "GameData.h"
#include "Engine.h"
#include "Graphics.h"
#include "ImageLoader.h"
#include "AppDefines.h"
#include "SplashScreen.h"


#if defined (ENABLE_SPLASHSCREEN)

CSplashScreen::CSplashScreen()
{
	_splash_counter = 0;
	init();
}

CSplashScreen::~CSplashScreen()
{
	destroy();
}


void CSplashScreen::init()
{
	// Load splash images here.
	engine->_image_loader->loadImagePack(image_pack_splash, (int)(sizeof(image_pack_splash) / sizeof(uint32)));
	
	// If there exists more than one splash image, they must all be initialized here.
	_splash_images[0].init();
	_splash_images[0].loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_LLAMAFACE_SPLASH));
	_splash_pos[0].x = (SCRN_W - _splash_images[0].getWidth()) >> 1;
	_splash_pos[0].y = (SCRN_H - _splash_images[0].getHeight()) >> 1;	
	
	// Start the overlap color fully opaque.
	_bg_overlap_color.r = 0.0f;
	_bg_overlap_color.g = 0.0f;
	_bg_overlap_color.b = 0.0f;
	_bg_overlap_color.a = 1.0f;
	
	_bg_color.r = 1.0f;
	_bg_color.g = 1.0f;
	_bg_color.b = 1.0f;
	_bg_color.a = 1.0f;
	
	_timer = 0;
	
	_splash_state = eSplashStateStart;
	_splash_counter = 0;
	
	set2Dview();
}

void CSplashScreen::destroy()
{
	engine->_image_loader->unloadImagePack();
}

void CSplashScreen::update()
{
	_timer += TIME_LAST_FRAME;
	
	switch (_splash_state)
	{			
		case eSplashStateStart:
		{
			// Blank splash screen. 
			if (_timer >= SPLASH_FADE_START)
			{
				_timer = 0;
				_splash_state = eSplashStateFadeIn;
			}
			
			break;
		}
		case eSplashStateFadeIn:
		{
			// We need to scale the time left to a 0 to 1 value.
			_bg_overlap_color.a = ((float)(SPLASH_FADE_IN - _timer)) / SPLASH_FADE_IN;
			
			if (_timer >= SPLASH_FADE_IN)
			{
				_timer = 0;
				_bg_overlap_color.a = 0;
				_splash_state = eSplashStateFadeFull;
			}
			
			break;
		}

		case eSplashStateFadeFull:
		{
			if (_timer >= SPLASH_FADE_FULL)
			{
				_timer = 0;
				_splash_state = eSplashStateFadeOut;
			}
			
			break;
		}
		case eSplashStateFadeOut:
		{
			_bg_overlap_color.a = 1.0 - (((float)(SPLASH_FADE_OUT - _timer)) / SPLASH_FADE_OUT);
			
			if (_timer >= SPLASH_FADE_OUT)
			{
				_timer = 0;
				_splash_state = eSplashStateFadeOutDone;
				_bg_overlap_color.a = 1.0f;
			}
			
			break;
		}
		case eSplashStateFadeOutDone:
		{
			if (_timer >= SPLASH_FADE_END)
			{
				_timer = 0;
				_splash_counter++;
				
				// Determine whether we need to run more splash screens. Go to the end state if not.
				if (_splash_counter > SPLASH_COUNT - 1)
				{
					// Make sure we don't go out of bounds if the image array is being accessed elsewhere with the _splash_counter variable.
					_splash_counter = SPLASH_COUNT - 1;
					_splash_state = eSplashStateEnd;
				}
				else
				{
					_splash_state = eSplashStateStart;
				}
			}
			
			break;
		}
		case eSplashStateEnd:
		{
			// No point in keeping the splash screen around, just clear the stack to be safe.
			setScreen(CEngine::eScreenTitle);
			
			break;
		}
	}
}

void CSplashScreen::draw()
{
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_color, TRUE);

	CGraphics::drawSprite(&_splash_images[_splash_counter], _splash_pos[_splash_counter].x, _splash_pos[_splash_counter].y);
	
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_overlap_color, TRUE);
}

void CSplashScreen::handleTouch(float x, float y, eTouchPhase phase)
{
	
}

void CSplashScreen::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	
}

#else


void CSplashScreen::init()
{

}

void CSplashScreen::destroy()
{

}

void CSplashScreen::update()
{

}

void CSplashScreen::draw()
{

}

void CSplashScreen::handleTouch(float x, float y, eTouchPhase phase)
{
	
}

void CSplashScreen::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	
}

#endif