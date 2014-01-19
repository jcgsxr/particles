#include "GameData.h"
#include "BasicInterface.h"
#include "Font.h"
#include "TitleScreen.h"
#include "Graphics.h"
#include "Image.h"
#include "ImageLoader.h"
#include "Engine.h"
#include "AppDefines.h"
#include "types.h"
#include "SystemDefines.h"
#include "SoundEngine.h"



CTitleScreen::CTitleScreen()
{
	setState(eTitleStateInit);
}

CTitleScreen::~CTitleScreen()
{
	destroy();
}

void CTitleScreen::init()
{    
	_bg_color.r = 1.0f;
	_bg_color.g = 1.0f;
	_bg_color.b = 1.0f;
	_bg_color.a = 1.0f;

	_fade_color.r = 0.0;
	_fade_color.g = 0.0;
	_fade_color.b = 0.0;
	_fade_color.a = 1.0;
	
	_fade_timer = 0;
	_time_counter = 0;
	_time_to_wait = 0;
	_allow_input = FALSE;
	_next_state = eTitleStateNone;
}

void CTitleScreen::destroy()
{

}

void CTitleScreen::update()
{	
	switch (getState()) 
	{
		case eTitleStateInit:
		{
			init();
			setState(eTitleStateLoad);
		}
			break;
			
		case eTitleStateLoad:
		{
			// Draw a loading bar here.
			
			
			// When done, fade in the title and allow input.
			setState(eTitleStateFadeIn);
			_allow_input = TRUE;
		}
			break;
		
		case eTitleStateFadeIn:
		{
			_fade_timer += TIME_LAST_FRAME;
			// We need to scale the time left to a 0 to 1 value.
			_fade_color.a = ((float)(SCREEN_TRANSITION_FADE_TIME - _fade_timer)) / SCREEN_TRANSITION_FADE_TIME;
			
			if (_fade_timer >= SCREEN_TRANSITION_FADE_TIME)
			{
				_fade_color.a = 0.0;
				
				// Go to the idle state and wait for user input to proceed.
				setState(eTitleStateIdle);
			}
		}
			break;
			
		case eTitleStateIdle:
		{

		}
			break;
			
		case eTitleStateEnd:
		{
			// Usually, we go to the main game screen at this point.
			//setScreen(CEngine::eScreenMain);
		}
			break;
			
		case eTitleStateTimer:
		{
			_time_counter += TIME_LAST_FRAME;
			
			if (_time_counter >= _time_to_wait)
			{
				setState(getNextState());
			}
		
		}
			break;
			
		default:
			break;
	}
}


void CTitleScreen::draw()
{	
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_color, TRUE);
	
	// Title screen draws go here.
	
	
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _fade_color, TRUE);
}


void CTitleScreen::handleTouch(float x, float y, eTouchPhase phase)
{
	// Usually, touch is only allowed after data has been loaded and the title screen has finished fading in.

	if (_allow_input)
	{
		// Handle user input here.
		
	}
}

void CTitleScreen::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	
}
