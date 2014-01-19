/*
 *  MainScreen.h
 *  framework
 *
 *  Created by Johnny Chan on 10/22/09.
 *  Copyright 2009 Llamaface. All rights reserved.
 *
 */

#ifndef __MAINSCREEN_H__
#define __MAINSCREEN_H__

#include "ParticleSystem.h"
#include "Camera.h"
#include "physics_types.h"
#include "BasicInterface.h"

// The y position of the particle properties selectors and buttons.
static const int PROP_SELECTOR_Y = 30;

// The y position of the particle mode selectors and buttons.
static const int MODE_SELECTOR_Y = 450;

// The touch positions of the increase/decrease buttons.
static const int VALUE_BOX_X = 0;
static const int VALUE_BOX_Y = 70;

// The touchable area of the buttons.
static const int TOUCH_AREA_SIZE = 30;

static const int TEXT_BUF_SIZE = 64;

// The amount of time in MS that the next hold time is decreased, in order increase value change speed.
static const int VALUE_TOUCH_HOLD_TIME = 250;
// The minumum amount of time that the value changes.
static const int VALUE_TOUCH_MIN_TIME = 250;
// The interval at which the hold time is decreased each time.
static const int VALUE_TOUCH_HOLD_DELTA = 20;

// Time in MS that the text begins to fade.
static const int TEXT_FADE_TIME = 2000;

// Interval at which the text fades.
static const int TEXT_FADE_SPEED = 25;

// The short pause time between switching modes.
static const int WAIT_TIME = 2000;

// The short pause when the particle system is resumed.
static const int RESUME_WAIT_TIME = 1000;

static const int NUM_GRAV_WELLS = 2;

// The height amount to adjust the glview when opening the modal view.
static const int GL_TO_MODAL_Y_OFFSET = 44 + 190;

// The amount of time it takes to change the size of the glview.
static const int GL_TO_MODAL_TIME = 500;
static const int MODAL_TO_GL_TIME = 500;


// Button positions.
static const int MENU_BUTTON_X = 280;
static const int MENU_BUTTON_Y = 250;
static const int RESTART_BUTTON_X = 280;
static const int RESTART_BUTTON_Y = 300;
static const int PAUSE_BUTTON_X = 280;
static const int PAUSE_BUTTON_Y = 350;
static const int REWIND_BUTTON_X = 280;
static const int REWIND_BUTTON_Y = 400;



// The different preset particle modes.
enum 
{
	eParticleDispMode_burst = 0,
	eParticleDispMode_3Dburst,
	eParticleDispMode_sparkle,
	eParticleDispMode_fireworks,
	eParticleDispMode_water,
	eParticleDispMode_fire,
	eParticleDispMode_firesmoke,
	eParticleDispMode_flames,
	eParticleDispMode_sun,
	eParticleDispMode_smoketrail,
	eParticleDispMode_radar,	
	eParticleDispMode_gravwell,
	eParticleDispMode_bigbang,
	eParticleDispMode_laser,
	eParticleDispMode_snow,
	eParticleDispMode_MAX
};

enum
{
	PROP_DIR_INC = 0,	// Used as an id to indicate value increase for adjusting particle property values.
	PROP_DIR_DEC,		// Used as an id to indicate value decrease for adjusting particle property values.
	PROP_DIR_NONE,
};

typedef struct particleData
{
	int mode;						// The id of the current particle mode.
	const char* name;				// The name displayed on the screen to indicate the current particle mode.
	int num_masses;					// Used for adjusting particle properties.
} particleData;


// It is important to keep this value up to date with the number of available particle images so that cycling through them work properly.
static const int imageMAX = 9;
// The image data contains the file image id of the particle and a string name.
typedef struct particleImageData
{
	int image_id;
	const char* image_name;
	const char* image_filename;
} particleImageData;



class CImage;
class CSprite;

class CMainScreen : public CBasicInterface
{	
public:	
	CMainScreen();
	~CMainScreen();
	
	void init(void);
	void destroy(void);
	void draw(void);
	void update(void);
	void handleTouch(float x, float y, eTouchPhase phase);
	void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2);
	
	void setMode(int mode, bool reset_props = TRUE);
	
	// Sets the particle property in the given mass. The direction indicates whether we are incrementing or decrementing the current value.
	void setPropertyInc(const int massID, const int propID, const int direction);

	// Set the particle property to the given value. The value can either be an int, float, or boolean.
	void setPropertyValue(const int massID, const int propID, const propVal value);
		
	// Returns a string of the actual current value of the selected particle property.
	const char* getPropertyValueStr(const int massID, int propID);
	
	// Gets a particle image name/description from the given image id.
	const char* getImageName(int imageID);
	
	// Gets the filename of the particle image.
	const char* getImageFileName(int imageID);
	
	// Gets the particle image id given an index into the data array.
	const int getImageId(int index);

	// Pops up the modal view.
	void showModalView();
	
	// Dismisses the modal view.
	void dismissModalView();
	
	inline CParticleSystem* getParticleSys(void) { return &_particle_sys; }
	
	// Pause the particle system.
	void pause(void);
	
	// Resume particle system. A short delay is placed on the resume to ensure that the modal view is finised animating.
	void resume(bool shouldWait = FALSE);
	
private:
	// Visible bg color.
	color _bg_color;
	
	// The touch rects for the arrows to change particle modes.
	rect _left_arrow_rect;
	rect _right_arrow_rect;

	// Touch rects for cycling through the particle properties.
	rect _left_prop_rect;
	rect _right_prop_rect;
	
	// The touch rects to increase/decrease particle values.
	rect _inc_rect;
	rect _dec_rect;

	// Rect-button that resets the current mode.
	rect _restart_rect;
	
	// Rewinds the physics, only works in certain modes.
	rect _rewind_rect;
	
	// The menu button that pops up the modal view.
	rect _menu_rect;
	
	// Pauses the particle system.
	rect _pause_rect;
	
	// Button sprites.
	CSprite _rewind_sprite;
	CSprite _restart_sprite;
	CSprite _menu_sprite;
	CSprite _pause_sprite;
	CSprite _resume_sprite;
	
	// Contains info on the current active particle mode.
	particleData _particle_mode;
	
	CParticleSystem _particle_sys;
	POGravWell _grav_wells[NUM_GRAV_WELLS];
	CCamera _camera;
	
	// This timer is used when the user holds the inc/dec buttons to change the value. As the button is held, the values change faster.
	int _touch_value_timer;
	int _touch_value_counter;
	
	// Indicates which button is being pressed for when the inc/dec values are rolling.
	int _touch_value_dir;
	
	// A multiplier value to make the value roll faster.
	int _touch_value_mult;
	
	// The current particle property selected, to change.
	int _prop_id;
	
	// Holds the string of the current value of the selected property.
	char _particle_val_text_buf[TEXT_BUF_SIZE];
	
	// Used to fade the text in and out.
	float _text_alpha;
	int _text_fade_counter;
	
	// Force a short pause between modes to prevent chugging.
	int _wait_time;
	
	int _resume_time;
	
	BOOL _is_running;
	
	// This rect represents the glviewport rect and is resized when the modal view pops up.
	rect _glview_rect;
	// The timer counter for when the glview rect changes size when the modal view pops up.
	int _glview_to_modal_time;
	// The timer counter for when the glview rect changes size when the modal view is dismissed.
	int _modal_to_glview_time;
	
	// The number of active masses.
	int _num_active_masses;
};


#endif