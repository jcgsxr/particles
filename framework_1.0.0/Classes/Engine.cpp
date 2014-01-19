#include "engine.h"
#include "BasicInterface.h"
#include "SystemDefines.h"
#include "SplashScreen.h"
#include "TitleScreen.h"
#include "MainScreen.h"
#include "ImageLoader.h"
#include "MenuSystem.h"
#include "Font.h"
#include "SoundEngine.h"
#include "math.h"
#include "AppDefines.h"
#include "Utils.h"
#include "GameData.h"
#include "Graphics.h"

#if defined (ENABLE_UNITTESTING)
#include "UnitTests.h"
#endif

#if defined (ENABLE_POLYTEST)
#include "PolyTest.h"
#endif


void updatePolyCount(int count)
{
#if defined (ENABLE_FPS)
	engine->_poly_count += count;
#endif
}

CEngine _engineInstance;
CEngine* engine;
CEngine::eScreens next_screen_id;
CEngine::eScreens current_screen_id;

// Used to execute time sensitive activities.  TIME_LAST_FRAME should always be up to date.
int TIME_LAST_FRAME;
int FRAME_START_TIME;
timeval START_TIME;
timeval CURRENT_TIME;
timeval TEMP_TIME_VAL;
timeval TIMEVAL_LAST_FRAME;


void initApp()
{
	TIME_LAST_FRAME = 0;
	FRAME_START_TIME = 0;
	
	glViewport(0, 0, SCRN_W, SCRN_H);

	set2Dview();
	
	// Enable blending
	glEnable(GL_BLEND);
	
	// Set a blending function to use.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// We can safely enable the vertex array state here since everything should be drawn using a vertex array.
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// Do an initial clear screen so we don't have a flash of white (or whatever).
	// Clear the drawing buffer.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	engine = &_engineInstance;
	engine->engineInit();
}


void destroyApp(void)
{
	engine->engineDestroy();
}


void updateApp()
{
	engine->engineUpdate();
}


void CEngine::engineDestroy(void)
{
	// Safe to clear the stack since we only reach this spot if the app exits.
	clearScreenStack();	
	
	if (_image_loader)
	{
		_image_loader->unloadImagePack();
		delete _image_loader;
		_image_loader = NULL;
	}
	
	if (_menu_system)
	{
		_menu_system->destroy();
		delete _menu_system;
		_menu_system = NULL;
	}
	
#if defined (ENABLE_SOUNDENGINE)
	if (_sound_engine)
	{
		_sound_engine->destroy();
		delete _sound_engine;
		_sound_engine = NULL;
	}
#endif
	
	if (_font)
	{
		_font->destroy();
		delete _font;
		_font = NULL;
	}
}


CEngine::CEngine()
{
	_is_initialized = FALSE;
}


CEngine::~CEngine()
{
	engineDestroy();
}

void CEngine::engineInit()
{	
	_image_loader = new CImageLoader();
	_menu_system = new CMenuSystem();
	_menu_system->init();
	
#if defined (ENABLE_SOUNDENGINE)
	_sound_engine = new CSoundEngine();
	_sound_engine->init((const soundLoadInfo**)&soundLoadInfoData, FILE_ID_SOUND_MAX);
#endif
	
	_font = new CFont();
	_font->init((const fontLoadInfo**)&fontLoadInfoData, eFontMAX);
	
	_curr_screen_stack_size = -1;
	
	_clear_screen_stack = FALSE;
	_pop_screen = FALSE;
	
	// Always start at the splash screen.
	queuePushScreen(eScreenSplash);
	
	// Used for benchmark text positioning.
	int y_info_offset = 0;
#if defined (ENABLE_FPS)
	_fps_current = 0.0;
	_fps_avg = 0.0;
	_fps_frame_counter = 0.0;
	_fps_time_counter = 0;
	_fps_seconds_total = 0;
	_fps_frames_total = 0.0;

	_fps_rect.x = 0;
	_fps_rect.y = 0;
	_fps_rect.w = 150;
	_fps_rect.h = (_font->getFontCharHeight(eFontBlack8x12) * 2) + 4;
	_fps_rect.col = 1.0;
	_fps_rect.col.a = 0.5;
	y_info_offset += _fps_rect.h;
#endif
	
#if defined (ENABLE_POLY_COUNT)
	_poly_count = 0;
	_poly_count_rect.x = 0;
	_poly_count_rect.y = y_info_offset;
	_poly_count_rect.w = 150;
	_poly_count_rect.h = _font->getFontCharHeight(eFontBlack8x12) + 2;
	_poly_count_rect.col = 1.0;
	_poly_count_rect.col.a = 0.5;
#endif
	
	_is_initialized = TRUE;
}


void CEngine::engineUpdate()
{	
	// Error checking for TIME_LAST_FRAME.
	if (TIME_LAST_FRAME < 0)
	{ 
		TIME_LAST_FRAME = 0;
	}
	
	if (!_is_initialized)
	{
		return;
	}
	
	if (_curr_screen_stack_size < 0)
	{
		DPRINT_ENGINE("CEngine::engineUpdate error: _curr_screen_stack_size less than zero");
	}
	else
	{
		_screen_stack[_curr_screen_stack_size]->update();
		
		_screen_stack[_curr_screen_stack_size]->draw();
	}

	if (_menu_system)
	{
		_menu_system->draw();
	}
	
	// Pop any screen in the pop queue.
	if (_pop_screen)
	{
		popScreen();
	}
	
	// We clear the screen stack here to make sure everything has finished first before anything is freed.
	if (_clear_screen_stack)
	{
		clearScreenStack();
	}
	
	// Lastly, if there is a screen waiting to be pushed onto the stack, do it now.
	if (_next_screen != eScreenNone)
	{
		pushScreen();
	}
		
#if defined (ENABLE_FPS)
	set2Dview();
	// Update frames per second counters until one second has been reached.
	_fps_time_counter += TIME_LAST_FRAME;
	_fps_frame_counter++;
	if (_fps_time_counter >= ONE_SECOND)
	{
		_fps_current = _fps_frame_counter;
		// Now calculate the average frames per second.
		// Get total frames and seconds since app start.
		_fps_frames_total += _fps_current;
		_fps_seconds_total++;
		_fps_avg = _fps_frames_total / _fps_seconds_total;
		
		// Now reset the counters.
		_fps_time_counter = 0;
		_fps_frame_counter = 0;
	}
	char buf[16];
	memset(&buf, 0, sizeof(char) * 16);
	
	CGraphics::drawRect(_fps_rect, true);

	_font->setGapOffset(0);
	
	// Draw the current fps.
	sprintf(buf, "FPS: %4.2f", _fps_current);
	_font->drawString(eFontBlack8x12, buf, 1, 1, 1.0);
	
	// Draw the average fps.
	sprintf(buf, "FPS AVG: %4.2f", _fps_avg);
	_font->drawString(eFontBlack8x12, buf, 1, _font->getFontCharHeight(eFontBlack8x12) + 2, 1.0);
#endif
	
#if defined (ENABLE_POLY_COUNT)
	set2Dview();
	
	CGraphics::drawRect(_poly_count_rect, true);

	_font->setGapOffset(0);
	
	char polybuf[32];
	memset(&polybuf, 0, sizeof(char) * 16);
	sprintf(polybuf, "POLY COUNT: %d", _poly_count);
	_font->drawString(eFontBlack8x12, polybuf, 1, _poly_count_rect.y + 1, 1.0);
	// Reset poly count for next frame.
	_poly_count = 0;
#endif
}

void CEngine::clearScreenStack()
{
	if (engine == NULL)
	{
		return;
	}
	
	for (int i = 0; i < SCREEN_STACK_MAX; ++i)
	{
		if (engine->_screen_stack[i])
		{
			engine->_screen_stack[i]->destroy();
			delete engine->_screen_stack[i];
			engine->_screen_stack[i] = NULL;
		}
	}
	
	_clear_screen_stack = FALSE;
	engine->_curr_screen_stack_size = -1;
}

void CEngine::pushScreen()
{
	if (_curr_screen_stack_size >= SCREEN_STACK_MAX - 1)
	{
		DPRINT_ENGINE("CEngine::pushScreen failed, reached max stack size");
		return;
	}
	
	_curr_screen_stack_size++;
	_screen_stack[_curr_screen_stack_size] = createScreen(_next_screen);
	_next_screen = eScreenNone;
}

void CEngine::popScreen()
{
	if (_curr_screen_stack_size < 1)
	{
		DPRINT_ENGINE("CEngine::popScreen failed, screen stack size is < 1");
		return;
	}
	
	if (getCurrentScreen())
	{
		delete _screen_stack[_curr_screen_stack_size];
		_screen_stack[_curr_screen_stack_size] = NULL;
		_curr_screen_stack_size--;
	}
	
	_pop_screen = FALSE;
}
		
		
void CEngine::touchEventHandler(float x, float y, eTouchPhase phase, eSwipeDirection swipeDir)
{
	if (_curr_screen_stack_size >= 0)
	{
		_screen_stack[_curr_screen_stack_size]->handleTouch(x, y, phase);
	}
}


void CEngine::multiTouchEventHandler(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2, eSwipeDirection swipeDir)
{
	if (_curr_screen_stack_size >= 0)
	{
		_screen_stack[_curr_screen_stack_size]->handleMultiTouch(x1, y1, phase1, x2, y2, phase2);
	}
}


CBasicInterface* CEngine::createScreen(eScreens screenID)
{
	CBasicInterface* new_screen = NULL;
	
	switch (screenID)
	{
		case eScreenSplash:
		{
			new_screen = new CSplashScreen();
		}
			break;
			
		case eScreenTitle:
		{
			new_screen = new CTitleScreen();
		}
			break;
			
		case eScreenMain:
		{
			new_screen = new CMainScreen();
		}
			break;
			
		case eScreenExitConfirm:
		{
			
		}
			break;
			
		case eScreenPolyTest:
		{
#if defined (ENABLE_POLYTEST)
			new_screen = new CPolyTest();
#endif
		}
			break;
			
		case eScreenUnitTests:
		{
#if defined (ENABLE_UNITTESTING)
			new_screen = new CUnitTests();
#endif
		}
			break;
			
		default:
		{
			
		}
			break;
	}
	
	if (new_screen == NULL)
	{
		DPRINT_ENGINE("CEngine::createScreen failed, screen creation failed");
	}
	
	return new_screen;
}

void setScreen(CEngine::eScreens screenID)
{
	queueClearScreenStack();
	
	queuePushScreen(screenID);
}

void queuePushScreen(CEngine::eScreens screenID)
{
	engine->_next_screen = screenID;
}

void queuePopScreen()
{
	engine->_pop_screen = TRUE;
}

void queueClearScreenStack()
{
	if (engine == NULL)
	{
		return;
	}
	
	engine->_clear_screen_stack = TRUE;
}