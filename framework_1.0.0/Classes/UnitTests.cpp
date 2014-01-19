#include "Camera.h"
#include "BasicInterface.h"
#include "GameObject.h"
#include "UnitTests.h"
#include "Sprite.h"
#include "GameData.h"
#include "SystemDefines.h"
#include "Graphics.h"
#include "Engine.h"
#include "ImageLoader.h"
#include "Utils.h"
#include "AppDefines.h"
#include "Physics.h"
#include "ParticleSystem.h"


#if defined (ENABLE_UNITTESTING)


#if defined (ENABLE_PHYSICS_DEBUG)


CUnitTests::CUnitTests()
{
	init();
}

CUnitTests::~CUnitTests()
{
	destroy();
}

void CUnitTests::init()
{
	_time_counter = 0;
	
	_bg_color.r = 1.0f;
	_bg_color.g = 1.0f;
	_bg_color.b = 1.0f;
	_bg_color.alpha = 1.0f;
	
	_temp_color.r = 0.0f;
	_temp_color.g = 0.0f;
	_temp_color.b = 1.0f;
	_temp_color.alpha = 0.5f;
	
	_temp_color_2.r = 1.0f;
	_temp_color_2.g = 0.0f;
	_temp_color_2.b = 0.0f;
	_temp_color_2.alpha = 0.5f;
	
	Vector2 impulse;
	
	srand(time(0));
	
	_num_active = 0;
	
	_objects[0] = new POLine;
	CPhysics::initLineObject((POLine*)_objects[0], 40, 440, 280, 440);
	_objects[0]->will_cause_y_zero_out_chain = TRUE;
	_objects[0]->will_cause_x_zero_out_chain = TRUE;
	
	_objects[1] = new POLine;
	CPhysics::initLineObject((POLine*)_objects[1], 0, 0, 40, 440);
	//_objects[1]->will_cause_y_zero_out_chain = TRUE;
	//_objects[1]->will_cause_x_zero_out_chain = TRUE;
	
	_objects[2] = new POLine;
	CPhysics::initLineObject((POLine*)_objects[2], 280, 440, 320, 0);
	//_objects[2]->will_cause_y_zero_out_chain = TRUE;
	//_objects[2]->will_cause_x_zero_out_chain = TRUE;
	
	for (int i = 3; i < UNIT_TEST_MAX_OBJECTS; ++i)
	{
		int sign_x = rand() % 2;
		int sign_y = rand() % 2;
		
		_objects[i] = new POCircle;
		CPhysics::initCircleObject((POCircle*)_objects[i], 8);
		
		//_objects[i]->active = FALSE;
		
		_objects[i]->pos.x = (rand() % SCRN_W) + 1;
		_objects[i]->pos.y = (rand() % SCRN_H) + 1;
		((POCircle*)_objects[i])->radius = 8;
		_objects[i]->acc.y = GRAVITY;
		_objects[i]->acc.x = 0;
		_objects[i]->bounciness = 0.8;
		impulse.x = (rand() % 200) + 1;
		impulse.y = (rand() % 200) + 1;
		
		if (sign_x == 0)
		{
			impulse.x *= -1;
		}
		if (sign_y == 0)
		{
			impulse.y *= -1;
		}
		
		CPhysics::applyImpulse(_objects[i], impulse);
	}
	
//	initPhysicsObject(&_line);
//	_line.start.x = 40.0;
//	_line.start.y = 440.0;
//	_line.end.x = 280.0;
//	_line.end.y = 440.0;
//	_line.will_cause_y_zero_out_chain = TRUE;
//	_line.will_cause_x_zero_out_chain = TRUE;
//	
//	initPhysicsObject(&_left_line);
//	_left_line.start.x = 0.0;
//	_left_line.start.y = 0.0;
//	_left_line.end.x = 40.0;
//	_left_line.end.y = 440.0;
//	//_left_line.will_cause_y_zero_out_chain = TRUE;
//	//_left_line.will_cause_x_zero_out_chain = TRUE;
//	
//	initPhysicsObject(&_right_line);
//	_right_line.start.x = 280.0;
//	_right_line.start.y = 440.0;
//	_right_line.end.x = 320.0;
//	_right_line.end.y = 0.0;
//	//_right_line.will_cause_y_zero_out_chain = TRUE;
//	//_right_line.will_cause_x_zero_out_chain = TRUE;
	
//	_obj[0].pos.x = 100;
//	_obj[0].pos.y = 100;
//	_obj[0].radius = 100;
//	_obj[0].acc.y = GRAVITY;
//	_obj[0].acc.x = 0;
//	_obj[0].bounciness = 0.4;
//	impulse.x = 100;
//	impulse.y = 0;
//	applyImpulse(&_obj[0], impulse);
//
//	_obj[1].pos.x = 200;
//	_obj[1].pos.y = 300;
//	_obj[1].radius = 100;
//	_obj[1].acc.y = GRAVITY / 2;
//	_obj[1].acc.x = 0;
//	_obj[1].bounciness = 0.4;
//	impulse.x = 0;
//	impulse.y = 0;
//	applyImpulse(&_obj[1], impulse);
}

void CUnitTests::destroy()
{
	
}

void CUnitTests::update()
{
	//updateCollisionsAndPhysics(_objects, MAX_OBJECTS, 4.0);	
	//updateCollisionsAndPhysics(_objects, MAX_OBJECTS, 0.0);	
	
	
	for (int i = 3; i < UNIT_TEST_MAX_OBJECTS; ++i)
	{			
		for (int j = i + 1; j < UNIT_TEST_MAX_OBJECTS; ++j)
		{		
			//printf("res_t: %f \n", res_t);
			
			if (CPhysics::checkCircleCircleCollision((POCircle*)_objects[i], (POCircle*)_objects[j]))
			{
				// Execute collision reaction.
				CPhysics::circleCircleCollisionWithThreshold((POCircle*)_objects[i], (POCircle*)_objects[j], CPhysics::PHYS_AXIS_X, 1.0);
			}
		}
		
		CPhysics::circleLineCollision((POLine*)_objects[0], (POCircle*)_objects[i]);
		
		CPhysics::circleLineCollision((POLine*)_objects[1], (POCircle*)_objects[i]);
		CPhysics::circleLineCollision((POLine*)_objects[2], (POCircle*)_objects[i]);
	}
	
	for (int i = 0; i < UNIT_TEST_MAX_OBJECTS; ++i)
	{	
		CPhysics::updatePhysics(_objects[i]);
	}
}


void CUnitTests::draw()
{
	color *temp_color;
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_color, TRUE);
	
	for (int i = 3; i < UNIT_TEST_MAX_OBJECTS; ++i)
	{
		if (i % 2 == 0)
		{
			temp_color = &_temp_color;
		}
		else
		{
			temp_color = &_temp_color_2;
		}
		
		if (_objects[i]->active == TRUE)
		{
			CGraphics::drawCircle(
					   _objects[i]->pos.x,
					   _objects[i]->pos.y,
					  ((POCircle*)_objects[i])->radius, 
					   64,  
					   *temp_color);
		}
		
		//drawCircleLineCollisionVectors(&_line, &_obj[i]);
	}
	
	//drawCircleCircleCollisionVectors(&_obj[0], &_obj[1]);
	
	CGraphics::drawLine(*(POLine*)_objects[0]);
	
	CGraphics::drawLine(*(POLine*)_objects[1]);
	CGraphics::drawLine(*(POLine*)_objects[2]);
}

void CUnitTests::handleTouch(float x, float y, eTouchPhase phase)
{
	if (phase == TouchPhaseEnded)
	{
		//_obj[_num_active].active = TRUE;
		//_num_active++;
		
		Vector2 impulse;
		srand(time(0));
		for (int i = 3; i < UNIT_TEST_MAX_OBJECTS; ++i)
		{
			int sign_x = rand() % 2;
			int sign_y = rand() % 2;
			
			CPhysics::initCircleObject((POCircle*)_objects[i], 8);
			
			//_objects[i]->active = FALSE;
			
			_objects[i]->pos.x = (rand() % SCRN_W) + 1;
			_objects[i]->pos.y = (rand() % SCRN_H) + 1;
			((POCircle*)_objects[i])->radius = 8;
			_objects[i]->acc.y = GRAVITY;
			_objects[i]->acc.x = 0;
			_objects[i]->bounciness = 0.8;
			impulse.x = (rand() % 200) + 1;
			impulse.y = (rand() % 200) + 1;
			
			if (sign_x == 0)
			{
				impulse.x *= -1;
			}
			if (sign_y == 0)
			{
				impulse.y *= -1;
			}
			
			CPhysics::applyImpulse(_objects[i], impulse);
		}
	}
}



#elif defined (ENABLE_PARTICLE_DEBUG)



CUnitTests::CUnitTests()
{
	init();
}

CUnitTests::~CUnitTests()
{
	destroy();
}

void CUnitTests::init()
{
	_bg_color.r = 0.0f;
	_bg_color.g = 0.0f;
	_bg_color.b = 0.0f;
	_bg_color.a = 1.0f;
	
	engine->_image_loader->loadImagePack(image_pack_unittest, (int)(sizeof(image_pack_unittest) / sizeof(uint32)));
	
	_bonus_sprite.init();
	_bonus_sprite.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_BONUS));
	
#if ENABLE_3D
//	_particle_sys.init(1, 40);
//	_particle_sys.recenterMass(0, 240, 160, 0);
//	//_particle_sys.applyImpulse(0, 0, 0, 0);
//	_particle_sys.setMassActive(0, TRUE);
//	_particle_mode = 0;
//	_particle_sys.setMode(0, (eParticleMode)eParticleModeBurstSmall);
	
	_particle_sys.init(1, 2000);
	_particle_sys.recenterMass(0, 320, 240, 0);
	_particle_sys.setMassActive(0, TRUE);
	_particle_mode = 0;
	//_particle_sys.setMode(0, (eParticleMode)eParticleModeShooter);
	_particle_sys.setDrawCenter(0, TRUE);
	
	_bonus_sprite._x = 240;
	_bonus_sprite._y = 160;
	_bonus_sprite._z = -100;
	_bonus_sprite.resetAction();
	_camera.init();
	
	set3Dview();
#else
	_particle_sys.init(1, 500);
	_particle_sys.recenterMass(0, 340, 260);
	_particle_sys.setMassActive(0, TRUE);
	_particle_mode = 0;
	_particle_sys.setMode(0, (eParticleMode)eParticleModeSun);
	_particle_sys.setDrawCenter(0, TRUE);
	set2Dview();
#endif
	
	CPhysics::initGravWellObject(&_grav_well, 200);
	_grav_well.pos.x = 240;
	_grav_well.pos.y = 160;
	_grav_well.pos.z = 0;
	
	_particle_sys.setIsRunning(TRUE);
}

void CUnitTests::destroy()
{
	engine->_image_loader->unloadImagePack();
	_particle_sys.destroy();
}

void CUnitTests::update()
{		
	_particle_sys.update();
	
#if ENABLE_3D
	_bonus_sprite.updateAction();
	for (int i = 0; i < _particle_sys._mass[0]->num_particles; ++i)
	{
		CPhysics::updateGravWell(&_grav_well, &_particle_sys._mass[0]->particles[i]->phys);
	}
#else
	if (_particle_mode == 1)
	{
		_bonus_sprite.updateAction();
	}
#endif
}

void CUnitTests::draw()
{	
#if ENABLE_3D
	_particle_sys.draw(eParticleDrawModeBillBoard, _camera.getViewMatrix());
	//_particle_sys.draw(eParticleDrawModePoint);
#else
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_color, TRUE);
	_particle_sys.draw(eParticleDrawModeNormal);
	//_particle_sys.draw(eParticleDrawModePoint);
	if (_particle_mode == 1)
	{
		CGraphics::drawSpriteCentered(&_bonus_sprite);
	}
#endif
}


static const int particle_impulse_x = 150;
static const int particle_impulse_y = 150;
static const int particle_impulse_z = 150;

void CUnitTests::handleTouch(float x, float y, eTouchPhase phase)
{
#if ENABLE_3D
	_camera.handleTouch(x, y, phase);
	
	return;
#endif
	
	if (phase == TouchPhaseEnded)
	{
		_particle_mode++;
		if (_particle_mode > 14)
		{
			_particle_mode = 0;
		}
		
		_particle_sys.destroy();
		
		switch (_particle_mode)
		{
			case 0:
			{
#if ENABLE_3D
				_particle_sys.init(1, 100);
				_particle_sys.recenterMass(0, 240, 160, -10);
				_particle_sys.applyImpulse(0, 0, -50);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeSparkler);
				_particle_sys.setDrawCenter(0, FALSE);
				
				
				_bonus_sprite._x = 240;
				_bonus_sprite._y = 160;
				_bonus_sprite._z = -100;
				_bonus_sprite.resetAction();
				//_bonus_sprite._scale = 0.1;
				//_bonus_sprite._color = 1.0;
				//_bonus_sprite.setSizeScaleAction(1.5, 1.5, 1.5, 500);
				//_bonus_sprite.setColorPulseAction(1.0, 1.0, 1.0, 0.0, 1500);
#else
				_particle_sys.init(1, 500);
				_particle_sys.recenterMass(0, 340, 260);
				_particle_sys.setMassActive(0, TRUE);
				_particle_mode = 0;
				_particle_sys.setMode(0, (eParticleMode)eParticleModeSun);
				_particle_sys.setDrawCenter(0, TRUE);
#endif
			}
				break;
				
			case 1:
			{
				_bonus_sprite._x = 50;
				_bonus_sprite._y = 50;
				_bonus_sprite.resetAction();
				_bonus_sprite._scale = 0.1;
				_bonus_sprite._color = 1.0;
				_bonus_sprite.setSizeScaleAction(1.5, 1.5, 1.5, 500, 1);
				_bonus_sprite.setColorPulseAction(1.0, 1.0, 1.0, 0.0, 1500, 1);
				
				_particle_sys.init(1, 40);
				_particle_sys.recenterMass(0, 50, 50);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeBurstSmall);
				_particle_sys.setDrawCenter(0, FALSE);
				_particle_sys.runOnce(0);
				
				//_particle_sys.init(1, 100);
//				_particle_sys.recenterMass(0, 0, 0);
//				_particle_sys.setMassActive(0, TRUE);
//				_particle_sys.setMode(0, eParticleModeSparkler);
//				_particle_sys.applyImpulse(0, particle_impulse_x, particle_impulse_y);
//				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;
				
			case 2:
			{
				_particle_sys.init(1, 100);
				_particle_sys.recenterMass(0, 240, 160);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeFall);
				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;
				
			case 3:
			{
				_particle_sys.init(1, 100);
				_particle_sys.recenterMass(0, 0, 0);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeFall);
				_particle_sys.applyImpulse(0, particle_impulse_x, particle_impulse_y);
				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;

			case 4:
			{
				_particle_sys.init(1, 100);
				_particle_sys.recenterMass(0, 240, 160);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeCircle);
				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;
				
			case 5:
			{
				_particle_sys.init(1, 100);
				_particle_sys.recenterMass(0, 0, 0);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeCircle);
				_particle_sys.applyImpulse(0, particle_impulse_x, particle_impulse_y);
				_particle_sys.setDrawCenter(0, TRUE);
			}
				break;
				
			case 6:
			{
				_particle_sys.init(1, 200);
				_particle_sys.recenterMass(0, 240, 160);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeFire);
				_particle_sys.setDrawCenter(0, TRUE);
			}
				break;
				
			case 7:
			{
				_particle_sys.init(1, 200);
				_particle_sys.recenterMass(0, 0, 320);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeFire);
				_particle_sys.applyImpulse(0, particle_impulse_x, -particle_impulse_y);
				_particle_sys.setDrawCenter(0, TRUE);
			}
				break;
				
			case 8:
			{
				_particle_sys.init(1, 200);
				_particle_sys.recenterMass(0, 240, 160);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeFireworks);
				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;
				
			case 9:
			{
				_particle_sys.init(1, 200);
				_particle_sys.recenterMass(0, 0, 320);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeFireworks);
				_particle_sys.applyImpulse(0, particle_impulse_x, -particle_impulse_y);
				_particle_sys.setDrawCenter(0, TRUE);
			}
				break;
				
			case 10:
			{
				_particle_sys.init(1, 40);
				_particle_sys.recenterMass(0, 240, 160);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeBurst);
				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;
				
			case 11:
			{
				_particle_sys.init(1, 40);
				_particle_sys.recenterMass(0, 0, 0);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeBurst);
				_particle_sys.applyImpulse(0, particle_impulse_x, particle_impulse_y);
				_particle_sys.setDrawCenter(0, TRUE);
			}
				break;
				
			case 12:
			{
				_particle_sys.init(2, 100);
				_particle_sys.recenterMass(0, 0, 320);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeSmokeTrail);
				_particle_sys.applyImpulse(0, 100, -100);
				_particle_sys.setDrawCenter(0, FALSE);

				_particle_sys.recenterMass(1, 40, 360);
				_particle_sys.setMassActive(1, TRUE);
				_particle_sys.setMode(1, eParticleModeSmokeTrail);
				_particle_sys.applyImpulse(1, 100, -100);
				_particle_sys.setDrawCenter(1, FALSE);
			}
				break;
				
			case 13:
			{
				_particle_sys.init(1, 100);
				_particle_sys.recenterMass(0, 240, 300);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeSteam);
				_particle_sys.setDrawCenter(0, FALSE);
			}
				break;
				
			case 14:
			{
				_particle_sys.init(2, 200);
				_particle_sys.recenterMass(0, 240, 300);
				_particle_sys.setMassActive(0, TRUE);
				_particle_sys.setMode(0, eParticleModeSteam);
				_particle_sys.setDrawCenter(0, FALSE);
				
				_particle_sys.recenterMass(1, 240, 300);
				_particle_sys.setMassActive(1, TRUE);
				_particle_sys.setMode(1, eParticleModeFire);
				_particle_sys.setDrawCenter(1, TRUE);
			}
				
			default:
				break;
				
		}
		
		_particle_sys.setIsRunning(TRUE);
	}
}

void CUnitTests::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
#if ENABLE_3D
	_camera.handleMultiTouch(x1, y1, phase1, x2, y2, phase2);
	
	return;
#endif
}


#elif defined (ENABLE_SPRITE_DEBUG)

CUnitTests::CUnitTests()
{
	init();
}

CUnitTests::~CUnitTests()
{
	destroy();
}

void CUnitTests::init()
{
	_bg_color.r = 1.0f;
	_bg_color.g = 1.0f;
	_bg_color.b = 1.0f;
	_bg_color.a = 1.0f;
	
	engine->_image_loader->loadImagePack(image_pack_unittest, (int)(sizeof(image_pack_unittest) / sizeof(uint32)));
	
	_tiles.init(240, 160, 0);
	_tiles.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_TILE_00));
	_tiles.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_TILE_01));
	_tiles.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_TILE_02));
	_tiles.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_TILE_03));
	_tiles.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_TILE_04));
	_tiles.loadSpriteImage(engine->_image_loader->getImage(FILE_ID_IMAGE_TILE_05));
	
	_tiles.setAnimType(ANIM_TYPE_LOOP_FADE);
	_tiles.setFrameSpeedsMS(1000);
}

void CUnitTests::destroy()
{
	_tiles.destroy();
}

void CUnitTests::update()
{
	_tiles.updateAnim();
}

void CUnitTests::draw()
{
	CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_color, TRUE);
	
	_tiles.draw();
}

void CUnitTests::handleTouch(float x, float y, eTouchPhase phase)
{
	
}

void CUnitTests::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	
}


#endif







#else

CUnitTests::CUnitTests()
{
	
}

CUnitTests::~CUnitTests()
{
	
}

void CUnitTests::init()
{
	
}

void CUnitTests::destroy()
{
	
}

void CUnitTests::update()
{
	
}

void CUnitTests::draw()
{
	
}

void CUnitTests::handleTouch(float x, float y, eTouchPhase phase)
{
	
}

void CUnitTests::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	
}

#endif