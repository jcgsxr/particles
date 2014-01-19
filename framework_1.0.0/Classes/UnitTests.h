#ifndef __UNITTESTS_H__
#define __UNITTESTS_H__

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "GameObject.h"
#include "physics_types.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "AppDefines.h"

static const int UNIT_TEST_MAX_OBJECTS = 100;

class CBasicInterface;
class CParticleSystem;
class CCamera;


class CUnitTests : public CBasicInterface
{
public:
	CGameObject _gameObject;
	
	CUnitTests();
	~CUnitTests();
	
	void init(void);
	void destroy(void);
	void draw(void);
	void update(void);
	
	void handleTouch(float x, float y, eTouchPhase phase);
	void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2);
private:
	
	
	// Visible bg color.
	color _bg_color;	
	
	
#if defined(ENABLE_PHYSICS_DEBUG)
	int _num_active;
	color _temp_color;
	color _temp_color_2;	
	physicsObject *_objects[UNIT_TEST_MAX_OBJECTS];
//	POCircle _obj[UNIT_TEST_MAX_OBJECTS];
//	POLine _line;
//	POLine _left_line;
//	POLine _right_line;
	int _time_counter;
#elif defined(ENABLE_PARTICLE_DEBUG)

	CSprite _bonus_sprite;
	int _particle_mode;
	CParticleSystem _particle_sys;
	POGravWell _grav_well;
	CCamera _camera;
	
#elif defined(ENABLE_SPRITE_DEBUG)
	
	CSprite _tiles;
	
#endif
};

#endif