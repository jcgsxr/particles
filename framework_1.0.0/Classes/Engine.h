#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <OpenGLES/ES1/gl.h>
#include <sys/time.h>
#include "types.h"

#define SCREEN_STACK_MAX	8

// Convenience macros for easier access to sub-systems.
#define GET_FONT		engine->_font
#define GET_IMGLOADER	engine->_image_loader
#define GET_MENUSYS		engine->_menu_system
#define GET_SOUND		engine->_sound_engine


class CBasicInterface;
class CImageLoader;
class CMenuSystem;
class CFont;
class CPhysics;
class CSoundEngine;


/*! \class CEngine
 * \brief The Application Engine class.
 *
 * This class contains the game state machine and manages game flow and state transition.
 * A stack is utilized to store the game screens. This allows for the ability to hold multiple game screens in memory.
 */
class CEngine
{
public:
	/*! \enum eScreens
	 * All the different possible screen types.
	 */
	typedef enum _eScreens
	{
		eScreenNone = -1,	/*!< This screen indicates that there is no screen waiting to be pushed onto the screen stack. */
		eScreenSplash = 0,	/*!< The general splash screen. */
		eScreenTitle,		/*!< The application title screen. */
		eScreenMain,		/*!< The main state of the app. The core chunk of game logic should be run within this state. */
		eScreenExitConfirm,	/*!< The screen that contains an exit confirmation. */
		eScreenUnitTests,	/*!< Used for general purpose testing. */
		eScreenPolyTest,	/*!< Used for physics and rendering testing. */
		eScreenMax,			/*!< The total number of unique screens. */
	} eScreens;
	
	/*! \fn CEngine()
	 *  \brief The CEngine class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CEngine();
	
	/*! \fn ~CEngine()
	 *  \brief The CEngine class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CEngine();
	
	/*! \fn engineInit(void)
	 *  \brief The CEngine class initializer function.
	 *  
	 * All engine systems are initialized here.
	 *	\param n/a
	 *  \return n/a
	 */
	void engineInit(void);

	/*! \fn engineDestroy(void)
	 *  \brief The CEngine class destructor function.
	 *  
	 * All engine systems are freed here.
	 *	\param n/a
	 *  \return n/a
	 */
	void engineDestroy(void);
	
	/*! \fn engineUpdate(void)
	 *  \brief The application update function.
	 *  
	 * State handling and transitions are managed within update.
	 *	\param n/a
	 *  \return n/a
	 */
	void engineUpdate(void);
	
	
	/*! \fn createScreen(eScreens screenID)
	 *  \brief Creates a new screen.
	 *  
	 * After queuePushScreen is called, createScreen is executed upon the next frame update.
	 *	\param screenID The screen type that is to be created.
	 *  \return A pointer to the newly created screen.
	 */
	CBasicInterface* createScreen(eScreens screenID);
	
	/*! \fn touchEventHandler(float x, float y, eTouchPhase phase, eSwipeDirection swipeDir)
	 *  \brief Touch handler for the current active screen.
	 *  
	 * All touch events and data pass through this handler. The data is then sent to the current active screen to handle.
	 *	\param x The x screen location that the touch occured.
	 *	\param y The y screen location that the touch occured.
	 *	\param phase The type of touch that has occured.
	 *	\param swipeDir The direction of a swipe.
	 *  \return n/a
	 */
	void touchEventHandler(float x, float y, eTouchPhase phase, eSwipeDirection swipeDir);
	
	/*! \fn multiTouchEventHandler(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2, eSwipeDirection swipeDir)
	 *  \brief Touch handler for the current active screen.
	 *  
	 * All touch events and data pass through this handler. The data is then sent to the current active screen to handle.
	 *	\param x1 The x screen location that the first touch occured.
	 *	\param y1 The y screen location that the first touch occured.
 	 *	\param phase1 The type of touch that has occured for the first touch.
	 *	\param x2 The x screen location that the second touch occured.
	 *	\param y2 The y screen location that the second touch occured.
 	 *	\param phase2 The type of touch that has occured for the second touch.
	 *	\param swipeDir The direction of a swipe.
	 *  \return n/a
	 */
	void multiTouchEventHandler(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2, eSwipeDirection swipeDir);
	
	/*! \fn clearScreenStack(void)
	 *  \brief Destroys all screens in the screen stack.
	 *  
	 * This is called automatically after queueClearScreenStack is called and should never be called manually.
	 * A new screen MUST be pushed onto the stack after this call unless the app is exiting.
	 *	\param n/a
	 *  \return n/a
	 */
	void clearScreenStack(void);
	
	/*! \fn pushScreen(void)
	 *  \brief Creates and pushes a screen onto the screen stack.
	 *  
	 * This is called automatically after queuePushScreen is called and should never be called manually.
	 *	\param n/a
	 *  \return n/a
	 */
	void pushScreen(void);
	
	/*! \fn popScreen(void)
	 *  \brief Pops the top screen from the screen stack and destroys it.
	 *  
	 * This is called automatically after queuePopScreen is called and should never be called manually.
	 *	\param n/a
	 *  \return n/a
	 */
	void popScreen(void);
	
	/*! \fn getCurrentScreen(void)
	 *  \brief Returns the current active screen.
	 *	\param n/a
	 *  \return The current active screen.
	 */
	inline CBasicInterface* getCurrentScreen(void) {return _screen_stack[_curr_screen_stack_size];}


	CBasicInterface* _screen_stack[SCREEN_STACK_MAX];	/*!< The screen stack. The screen can hold up to SCREEN_STACK_MAX screens. */
	int _curr_screen_stack_size;						/*!< Keeps track of the current active screen and is updated within the push and pop functions. */
	CImageLoader* _image_loader;						/*!< Instance of the image loader that is used to help organize image loading. */
	CMenuSystem* _menu_system;							/*!< Instance of the menu system. */
	CFont* _font;										/*!< Instance of the font class, used to draw custom fonts to the screen. */
	CSoundEngine* _sound_engine;						/*!< Instance of the sound sytem class, used to play sounds. */
	BOOL _clear_screen_stack;							/*!< Indicates that upon the next update completetion, the screen stack must be cleared. */
	eScreens _next_screen;								/*!< Holds the id to the next screen to be pushed onto the stack. */
	BOOL _pop_screen;									/*!< Indicates the upon the next update completetion, the active screen will be popped. */
	BOOL _is_initialized;								/*!< Indicates that #init() has finished. */
	

	// These are only used when ENABLE_FPS is defined.
	float _fps_current;		// The number of frames that have occured the previous one second.
	float _fps_avg;			// The average frames per second since app start.
	float _fps_frame_counter;	// Used to keep track of how many frames have occured in one second.
	int _fps_time_counter;	// Used to keep track of every occurance of one second.
	float _fps_frames_total;	// The total number of frames that have occured since app start.
	int _fps_seconds_total;	// The total number of seconds that have occured since app start.
	colorRect _fps_rect;
	
	// These are only used when ENABLE_POLY_COUNT is defined.
	int _poly_count;
	colorRect _poly_count_rect;
};


/*! \fn initApp(void)
 *  \brief The application initializer function.
 *  
 * Rendering and viewport initialization occurs here.
 *	\param n/a
 *  \return n/a
 */
extern void initApp(void);

/*! \fn destroyApp(void)
 *  \brief The application destructor function.
 *  
 * This is the globally accessible engine destructor function.
 *	\param n/a
 *  \return n/a
 */
extern void destroyApp(void);

/*! \fn updateApp(void)
 *  \brief The application update function.
 *  
 * The highest level application update. 
 *	\param n/a
 *  \return n/a
 */
extern void updateApp(void);

/*! \fn setScreen(eScreens screenID)
 *  \brief Sets a new screen.
 *  
 * Effectively clears the screen stack and executes a transition to the given screen.
 *	\param screenID The new screen to transition to.
 *  \return n/a
 */
extern void setScreen(CEngine::eScreens screenID);

/*! \fn queuePushScreen(eScreens screenID)
 *  \brief Queues a screen to be pushed onto the screen stack.
 *  
 * Prepares the next update to push a new screen onto the stack.
 *	\param screenID The new screen to push onto the stack.
 *  \return n/a
 */
extern void queuePushScreen(CEngine::eScreens screenID);

/*! \fn queuePopScreen(void)
 *  \brief Queues a screen to be popped from the screen stack.
 *  
 * Prepares the next update to pop the active screen.
 *	\param n/a
 *  \return n/a
 */
extern void queuePopScreen(void);

/*! \fn queueClearScreenStack(void)
 *  \brief Queues a screen to be popped from the screen stack.
 *  
 * Prepares the screen stack to be cleared. This is called by other screens once they are finished.
 *	\param n/a
 *  \return n/a
 */
extern void queueClearScreenStack(void);

/*! \fn updatePolyCount(int count)
 *  \brief Used for benchmarking polygon count.
 *  
 * Updates the poly count for when benchmarking is enabled. This should be invoked where ever a gl rendering call is made.
 *	\param count The number of polygons to add to the count. This value is displayed and reset after each frame.
 *  \return n/a
 */
extern void updatePolyCount(int count);


/**
 * \defgroup Globals Global variables.
 */
/*@{*/
/** This value is updated every frame and contains the duration of time in milliseconds that the previous frame took. */
extern int TIME_LAST_FRAME;	
/** Contains the current time of the current frame in milliseconds. */
extern int FRAME_START_TIME;
/** Used to help calculate TIME_LAST_FRAME. This is defined as a global in the CEngine class in order to make the framework more easily integratable. */
extern timeval CURRENT_TIME;
/** Used to help calculate TIME_LAST_FRAME. This is defined as a global in the CEngine class in order to make the framework more easily integratable. */
extern timeval TEMP_TIME_VAL;
/** Used to help calculate TIME_LAST_FRAME. This is defined as a global in the CEngine class in order to make the framework more easily integratable. */
extern timeval TIMEVAL_LAST_FRAME;
/** Instance of the application engine. */
extern CEngine _engineInstance;
/** Pointer to the engine instance. Globally accessible since various systems need access to the engine. */
extern CEngine* engine;
/*@}*/


#endif
