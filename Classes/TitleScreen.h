#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "types.h"
#include "SystemDefines.h"

static const int SCREEN_TRANSITION_FADE_TIME = 1000;

class CImage;
class CSprite;

class CTitleScreen : public CBasicInterface
{
public:	
	typedef enum _eTitleState
	{
		eTitleStateNone = 0,
		eTitleStateInit,
		eTitleStateLoad,		// Displays a loading bar.
		eTitleStateFadeIn,		// Fade in sequence.
		eTitleStateIdle,		// Idle title screen state, where input is expected from the player.
		eTitleStateFadeOut,		// Fade out sequence.
		eTitleStateEnd,			// Transitions to the next screen.
		eTitleStateTimer,		// General timer state.
	} eTitleState;
	
	CTitleScreen();
	~CTitleScreen();
	
	void init(void);
	void destroy(void);
	void draw(void);
	void update(void);
	void handleTouch(float x, float y, eTouchPhase phase);
	void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2);
	
	inline void allowInput(BOOL allow) {_allow_input = allow;}
	inline BOOL getAllowInput(void) {return _allow_input;}
	
	inline eTitleState getState(void) {return _state;}
	inline void setState(eTitleState state) {_state = state;}
	
	inline eTitleState getNextState(void) {return _next_state;}
	inline void setNextState(eTitleState state) {_next_state = state;}
	
private:
	// Visible bg color.
	color _bg_color;
	
	int _fade_timer;
	color _fade_color;
	
	// General timer used for the timer state.
	int _time_counter;
	int _time_to_wait;
	
	// Title screen state containers.
	eTitleState _state;
	eTitleState _next_state;
	
	BOOL _allow_input;
};

#endif