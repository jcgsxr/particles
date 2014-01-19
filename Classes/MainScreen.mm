/*
 *  MainScreen.cpp
 *  framework
 *
 *  Created by Johnny Chan on 10/22/09.
 *  Copyright 2009 Llamaface. All rights reserved.
 *
 */

#import "BasicInterface.h"
#import "Font.h"
#import "Engine.h"
#import "MainScreen.h"
#import "Graphics.h"
#import "Image.h"
#import "ImageLoader.h"
#import "AppDefines.h"
#import "SystemDefines.h"
#import "SoundEngine.h"
#import "GameData.h"
#import "physics.h"
#import "particlesAppDelegate.h"
#import "SettingsController.h"
#import "SettingsViewController.h"

// Particle mode data.
const particleData mode_data[eParticleDispMode_MAX] =
{
	{ 	eParticleDispMode_burst, "Burst", 1 },
	{	eParticleDispMode_3Dburst, "3D Burst", 1 },
	{	eParticleDispMode_sparkle, "Sparkle", 1 },
	{	eParticleDispMode_fireworks, "Fireworks", 1 },
	{	eParticleDispMode_water, "Water", 1 },
	{	eParticleDispMode_fire, "Fire", 1 },
	{	eParticleDispMode_firesmoke, "Fire and Smoke", 2 },
	{	eParticleDispMode_flames, "Flame Bursts", 10 },
	{	eParticleDispMode_sun, "Fireball", 1 },
	{	eParticleDispMode_smoketrail, "Smoke Trail", 2 },
	{	eParticleDispMode_radar, "Circle", 1 },
	{	eParticleDispMode_gravwell, "Orbit", 1 },
	{	eParticleDispMode_bigbang, "Explosion", 1 },
	{	eParticleDispMode_laser, "Strands", 1 },
	{	eParticleDispMode_snow, "Snow", 1 },
};

// This is used for the particle image label when switching particle images.
const particleImageData image_data[imageMAX] = 
{
	{ 	FILE_ID_IMAGE_PARTICLE, "White Circle", "particle_16x16.png",},
	{	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARK, "Yellow Spark", "particle_spark_yellow_16x16.png",},
	{	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARK_BIG, "Yellow Spark Big", "particle_spark_yellow_32x32.png",},
	{	FILE_ID_IMAGE_PARTICLE_ORANGE_SPARK_BIG, "Orange Circle", "particle_spark_orange_32x32.png",},
	{	FILE_ID_IMAGE_PARTICLE_CLOUD, "Cloud", "particle_cloud_32x32.png",},
	{	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARKLE, "Yellow Gleam", "particle_sparkle_yellow_32x32.png",},
	{	FILE_ID_IMAGE_PARTICLE_BLUE_SPARKLE, "Blue Gleam", "particle_sparkle_blue_32x32.png",},
	{	FILE_ID_IMAGE_PARTICLE_DROPLET, "Water", "particle_droplet_16x16.png",},
	{	FILE_ID_IMAGE_PARTICLE_SMALL, "Small Circle", "particle_round_8x8.png",},
};


const char* CMainScreen::getImageName(int imageID)
{
	for (int i = 0; i < imageMAX; ++i)
	{
		if (image_data[i].image_id == imageID)
		{
			return image_data[i].image_name;
		}
	}
	
	return "NULL";
}

const int CMainScreen::getImageId(int index)
{
	return image_data[index].image_id;
}

const char* CMainScreen::getImageFileName(int imageID)
{
	for (int i = 0; i < imageMAX; ++i)
	{
		if (image_data[i].image_id == imageID)
		{
			return image_data[i].image_filename;
		}
	}
	
	return "NULL";
}


CMainScreen::CMainScreen()
{
	init();
}

CMainScreen::~CMainScreen()
{
	destroy();
}

void CMainScreen::init()
{	
	_bg_color.r = 0.0f;
	_bg_color.g = 0.0f;
	_bg_color.b = 0.0f;
	_bg_color.a = 1.0f;

	memset(&_particle_val_text_buf, 0, sizeof(char) * TEXT_BUF_SIZE);
	_touch_value_timer = -1;
	_touch_value_counter = 0;
	_touch_value_mult = 1;
	
	// Text always starts as fully visible.
	_text_alpha = 1.0f;
	_text_fade_counter = TEXT_FADE_TIME;
	
	GET_IMGLOADER->loadImagePack(image_pack_main, (int)(sizeof(image_pack_main) / sizeof(uint32)));
	
	_particle_sys.setIsRunning(TRUE);

	// The gravity well is used exclusively for galaxy particle mode. It attracts particles towards like much like a black hole would.
	// We will place ti to the upper right of where the particles are being emitted.
	for (int i = 0; i < NUM_GRAV_WELLS; ++i)
	{
		CPhysics::initGravWellObject(&_grav_wells[i], 20);
	}
	_grav_wells[0].pos.x = 240;
	_grav_wells[0].pos.y = 160;
	_grav_wells[0].pos.z = 0;
	_grav_wells[1].pos.x = 100;
	_grav_wells[1].pos.y = 100;
	_grav_wells[1].pos.z = 0;
	
	// Set up touch arrow positions.
	initRect(_left_arrow_rect, 0, MODE_SELECTOR_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);	
	initRect(_right_arrow_rect, 320 - GET_FONT->getStringWidth(eFontCalibriBold_24x24, ">"), MODE_SELECTOR_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);

	// Set up the properties arrows.
	initRect(_left_prop_rect, 0, PROP_SELECTOR_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	initRect(_right_prop_rect, 320 - GET_FONT->getStringWidth(eFontCalibriBold_24x24, ">"), PROP_SELECTOR_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	
	// Set up increase/decrease property value touch boxes.
	initRect(_inc_rect, VALUE_BOX_X, VALUE_BOX_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	initRect(_dec_rect, VALUE_BOX_X, VALUE_BOX_Y + 50, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	
	// Init the rewind button properties.
	initRect(_rewind_rect, REWIND_BUTTON_X, REWIND_BUTTON_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	_rewind_sprite.init();
	_rewind_sprite.loadSpriteImage(GET_IMGLOADER->getImage(FILE_ID_IMAGE_REWIND));
	_rewind_sprite._x = _rewind_rect.x + (abs(_rewind_rect.w - _rewind_sprite.getWidth()) / 2);
	_rewind_sprite._y = _rewind_rect.y + (abs(_rewind_rect.h - _rewind_sprite.getHeight()) / 2);

	// Init the restart button properties.
	initRect(_restart_rect, RESTART_BUTTON_X, RESTART_BUTTON_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	_restart_sprite.init();
	_restart_sprite.loadSpriteImage(GET_IMGLOADER->getImage(FILE_ID_IMAGE_RESTART));
	_restart_sprite._x = _restart_rect.x + (abs(_restart_rect.w - _restart_sprite.getWidth()) / 2);
	_restart_sprite._y = _restart_rect.y + (abs(_restart_rect.h - _restart_sprite.getHeight()) / 2);
	
	// Init the menu rect properties.
	initRect(_menu_rect, MENU_BUTTON_X, MENU_BUTTON_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	_menu_sprite.init();
	_menu_sprite.loadSpriteImage(GET_IMGLOADER->getImage(FILE_ID_IMAGE_MENU));
	_menu_sprite._x = _menu_rect.x + (abs(_menu_rect.w - _menu_sprite.getWidth()) / 2);
	_menu_sprite._y = _menu_rect.y + (abs(_menu_rect.h - _menu_sprite.getHeight()) / 2);
	
	// Init the pause button properties.
	initRect(_pause_rect, PAUSE_BUTTON_X, PAUSE_BUTTON_Y, TOUCH_AREA_SIZE, TOUCH_AREA_SIZE);
	_pause_sprite.init();
	_pause_sprite.loadSpriteImage(GET_IMGLOADER->getImage(FILE_ID_IMAGE_PAUSE));
	_pause_sprite._x = _pause_rect.x + (abs(_pause_rect.w - _pause_sprite.getWidth()) / 2);
	_pause_sprite._y = _pause_rect.y + (abs(_pause_rect.h - _pause_sprite.getHeight()) / 2);
	
	// The resume button uses some of the same properties as the pause button, since they swap.
	_resume_sprite.init();
	_resume_sprite.loadSpriteImage(GET_IMGLOADER->getImage(FILE_ID_IMAGE_RESUME));
	_resume_sprite._x = _pause_rect.x + (abs(_pause_rect.w - _resume_sprite.getWidth()) / 2);
	_resume_sprite._y = _pause_rect.y + (abs(_pause_rect.h - _resume_sprite.getHeight()) / 2);
	
	// glview rect starts out as the entire screen and is compacted as the modal view comes into visiblity.
	initRect(_glview_rect, 0, 0, SCRN_W, SCRN_H);
	_glview_to_modal_time = 0;
	_modal_to_glview_time = 0;
	
	// Set the initial particle property selected.
	_prop_id = PROP_DRAW_MODE;
	
	// There are initially no active masses.
	_num_active_masses = 0;
	
	// Set an initial mode.
	//setMode(eParticleDispMode_burst);
	//setMode(eParticleDispMode_bigbang);
	setMode(eParticleDispMode_gravwell);
	
	_resume_time = 0;
	_is_running = TRUE;
}

void CMainScreen::destroy()
{
	GET_IMGLOADER->unloadImagePack();
	_particle_sys.destroy();
	_rewind_sprite.destroy();
	_restart_sprite.destroy();
	_menu_sprite.destroy();
}

void CMainScreen::setMode(int mode, bool reset_props)
{
	// Set a wait time.
	_wait_time = WAIT_TIME;
	
	// Unpause when setting a new mode.
	resume();
	
	_particle_sys.destroy();
	
	switch (mode) 
	{
		case eParticleDispMode_burst:
		{
			_camera.reset();
			
			_particle_sys.init(1, 800);
			_particle_sys.recenterMass(0, 160, 240);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeBurst]);
			_particle_sys.setDrawCenter(0, FALSE);
		}
			break;
				
		case eParticleDispMode_3Dburst:
		{
			_camera.init();
			
			_particle_sys.init(1, 800);
			_particle_sys.recenterMass(0, 160, 240, -5);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeBurst]);
			_particle_sys.setDrawCenter(0, FALSE);
			_particle_sys.setEnable3D(0, TRUE);
			
		}
			break;
			
		case eParticleDispMode_sparkle:
		{
			_camera.init();
			
			_particle_sys.init(1, 500);
			_particle_sys.recenterMass(0, 160, 240, -10);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeScreenSparkler]);
			_particle_sys.setDrawCenter(0, FALSE);			
		}
			break;
			
		case eParticleDispMode_fireworks:
		{
			_particle_sys.init(1, 200);
			_particle_sys.recenterMass(0, 240, 160);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeFireworks]);
			_particle_sys.setDrawCenter(0, FALSE);
		}
			break;
			
		case eParticleDispMode_water:
		{
			_particle_sys.init(1, 300);
			_particle_sys.recenterMass(0, 160, 160);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeFall]);
			_particle_sys.setDrawCenter(0, FALSE);
		}
			break;
			
		case eParticleDispMode_fire:
		{
			_camera.reset();
			
			_particle_sys.init(1, 200);
			_particle_sys.recenterMass(0, 160, 240);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeFire]);
			_particle_sys.setDrawCenter(0, TRUE);
		}
			break;
			
		case eParticleDispMode_firesmoke:
		{
			_camera.reset();
			
			_particle_sys.init(2, 200);
			_particle_sys.recenterMass(0, 160, 300);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeSteam]);
			_particle_sys.setDrawCenter(0, FALSE);
			
			_particle_sys.recenterMass(1, 160, 300);
			_particle_sys.setMassActive(1, TRUE);
			_particle_sys.setMode(1, particle_mode_props[eParticleModeFire]);
			_particle_sys.setDrawCenter(1, TRUE);
		}
			break;
			
		case eParticleDispMode_flames:
		{
			_camera.reset();
			
			_particle_sys.init(10, 50);
			
			int xPos = 60;
			for (int i = 0; i < 10; ++i)
			{
				_particle_sys.recenterMass(i, xPos, 300);
				_particle_sys.setMassActive(i, TRUE);
				_particle_sys.setMode(i, particle_mode_props[eParticleModeFire]);
				_particle_sys.setDrawCenter(i, TRUE);
				
				xPos += 20;
			}
		}
			break;
			
		case eParticleDispMode_sun:
		{
			_camera.reset();
			
			_particle_sys.init(1, 500);
			_particle_sys.recenterMass(0, 160, 240);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeSun]);
			_particle_sys.setDrawCenter(0, TRUE);
		}
			break;
			
		case eParticleDispMode_smoketrail:
		{
			_camera.reset();
	
			// Init 2 masses of particles, resulting in 2 smoke trails.
			_particle_sys.init(2, 100);
			// Center at bottom left corner of the screen.
			_particle_sys.recenterMass(0, 0, 480);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeSmokeTrail]);
			// The trail will travel right and up so we place a positive x impulse and a negative y impulse.
			_particle_sys.applyImpulse(0, 12, -25);
			_particle_sys.setDrawCenter(0, FALSE);
			// Don't release more than one set of particles since the trail moves off screen.
			_particle_sys.setLoopCount(0, 1);
			
			_particle_sys.recenterMass(1, 40, 480);
			_particle_sys.setMassActive(1, TRUE);
			_particle_sys.setMode(1, particle_mode_props[eParticleModeSmokeTrail]);
			_particle_sys.applyImpulse(1, 12, -25);
			_particle_sys.setDrawCenter(1, FALSE);
			// Don't release more than one set of particles since the trail moves off screen.
			_particle_sys.setLoopCount(1, 1);
		}
			break;
			
		case eParticleDispMode_radar:	
		{
			_camera.reset();

			_particle_sys.init(1, 200);
			_particle_sys.recenterMass(0, 160, 240);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeCircle]);
			_particle_sys.setDrawCenter(0, FALSE);
		}
			break;
			
		case eParticleDispMode_gravwell:
		{
			_camera.init();
			
			_particle_sys.init(1, 200);
			_particle_sys.recenterMass(0, 160, 240, 0);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeShooter]);
			_particle_sys.setDrawCenter(0, TRUE);
			_particle_sys.setEnable3D(0, TRUE);
			_particle_sys.setStrandLength(0, 5);
		}
			break;
			
		case eParticleDispMode_bigbang:
		{
			_camera.init();
			
			_particle_sys.init(1, 2000);
			_particle_sys.recenterMass(0, 160, 240, 0);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeBigBang]);
			_particle_sys.setDrawCenter(0, TRUE);
			_particle_sys.setEnable3D(0, TRUE);
		}
			break;
			
		case eParticleDispMode_laser:
		{
			_camera.init();
			
			_particle_sys.init(1, 3);
			_particle_sys.recenterMass(0, 160, 240, 0);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeShooter]);
			_particle_sys.setDrawCenter(0, FALSE);
			_particle_sys.setEnable3D(0, TRUE);
			_particle_sys.setStrandLength(0, 2000);
		}
			break;
			
		case eParticleDispMode_snow:
		{
			_camera.reset();
			
			_particle_sys.init(1, 2000);
			_particle_sys.recenterMass(0, 400, -50);
			_particle_sys.setMassActive(0, TRUE);
			_particle_sys.setMode(0, particle_mode_props[eParticleModeSnow]);
			_particle_sys.setDrawCenter(0, FALSE);
			_particle_sys.setEnable3D(0, FALSE);
		}
			
		default:
			break;
	}
	
	_particle_mode.mode = mode_data[mode].mode;
	_particle_mode.name = mode_data[mode].name;
	
//	// Check for whether we want to preserve mode values.
//	if (reset_props)
//	{
//		_particle_sys.setDrawMode(mode_data[mode].draw_mode);
//	}
}

void CMainScreen::update()
{
	// Camera is always updated no matter what.
	_camera.update();
	
	// If the glview time is set, then we need to change the glview rect size.
	if (_glview_to_modal_time > 0)
	{
		_glview_to_modal_time -= TIME_LAST_FRAME;
		
		float glheightdelta = SCRN_H - (((float)(GL_TO_MODAL_TIME - _glview_to_modal_time) / GL_TO_MODAL_TIME) * GL_TO_MODAL_Y_OFFSET);
		_glview_rect.h = glheightdelta;
		_glview_rect.y = SCRN_H - glheightdelta;
		
		// Set viewport size.
		glViewport(_glview_rect.x, _glview_rect.y, _glview_rect.w, _glview_rect.h);
		
		// Ensure that we end up at the correct final size.
		if (_glview_to_modal_time <= 0)
		{
			_glview_rect.h = SCRN_H - GL_TO_MODAL_Y_OFFSET;
			_glview_rect.y = GL_TO_MODAL_Y_OFFSET;
			glViewport(_glview_rect.x, _glview_rect.y, _glview_rect.w, _glview_rect.h);
		}
	}
	
	if (_modal_to_glview_time > 0)
	{
		_modal_to_glview_time -= TIME_LAST_FRAME;
		
		float glheightdelta = SCRN_H - (((float)_modal_to_glview_time / GL_TO_MODAL_TIME) * GL_TO_MODAL_Y_OFFSET);
		_glview_rect.h = glheightdelta;
		_glview_rect.y = SCRN_H - glheightdelta;
		
		// Set viewport size.
		glViewport(_glview_rect.x, _glview_rect.y, _glview_rect.w, _glview_rect.h);
		
		// Ensure that we end up at the correct final siz.e
		if (_modal_to_glview_time <= 0)
		{
			_glview_rect.h = SCRN_H;
			_glview_rect.y = 0;
			glViewport(_glview_rect.x, _glview_rect.y, _glview_rect.w, _glview_rect.h);
		}
	}
	
	if (!_is_running)
	{
		return;
	}
	
	// Wait time is used as a delay right after particles are loaded nad before particles are updated.
	if (_wait_time > 0)
	{
		_wait_time -= TIME_LAST_FRAME;
		return;
	}
	
	// Check for the resume delay after a pause has ended.
	if (_resume_time > 0)
	{
		_resume_time -= TIME_LAST_FRAME;
		return;
	}
	
	if ((_particle_mode.mode == eParticleDispMode_gravwell) || (_particle_mode.mode == eParticleDispMode_laser))
	{
		for (int i = 0; i < _particle_sys.getNumParticles(0); ++i)
		{
			for (int j = 0; j < NUM_GRAV_WELLS; ++j)
			{
				CPhysics::updateGravWell(&_grav_wells[j], &_particle_sys.getMass(0).particles[i].phys, _particle_sys.getMass(0).center.props.frame_skip);
			}
		}
	}
	
	_particle_sys.update();
	
	// Update touch timers if needed.
	if (_touch_value_timer >= 0)
	{
		// While the touch is held, don't allow text fading.
		_text_fade_counter = 0;
		
		// Update touch time counter.
		_touch_value_counter += TIME_LAST_FRAME;
		
		// Roll the values faster as the button is held longer.
		if (_touch_value_counter >= _touch_value_timer)
		{
			_touch_value_counter = 0;
			
			// Each time the timer has been reached, the amount of hold time is decreased, so that the values will being to roll by faster.
			_touch_value_timer -= VALUE_TOUCH_HOLD_DELTA;
			if (_touch_value_timer <= VALUE_TOUCH_MIN_TIME)
			{
				_touch_value_mult++;
				_touch_value_timer = VALUE_TOUCH_MIN_TIME;
			}
			
			for (int i = 0; i < mode_data[_particle_mode.mode].num_masses; ++i)
			{				
				setPropertyInc(i, _prop_id, _touch_value_dir);
			}
		}		
	}
	
	// Update text fade timer.
	_text_fade_counter += TIME_LAST_FRAME;
	if (_text_fade_counter > TEXT_FADE_TIME)
	{
		_text_alpha -= (float)TIME_LAST_FRAME / ONE_SECOND;
		
		if (_text_alpha < 0)
		{
			_text_alpha = 0.0f;
		}
	}
}

void CMainScreen::draw()
{
	if (_particle_sys.is3DEnabled(0))
	{
		set3Dview();
		//_particle_sys.draw(eParticleDrawModeBillBoard, _camera.getViewMatrix());
		_particle_sys.draw(_camera.getViewMatrix());
	}
	else 
	{
		set2Dview();
		_camera.reset();
		CGraphics::drawRect(0, 0, SCRN_W, SCRN_H, _bg_color, TRUE);
		_particle_sys.draw();
	}
	
	// We need to draw the text in 2d mode so set that here.
	_camera.reset();
	set2Dview();
	
	// Draw left and right arrows.
	GET_FONT->drawString(eFontCalibriBold_24x24, "<", _left_arrow_rect.x, _left_arrow_rect.y, _text_alpha);
	GET_FONT->drawString(eFontCalibriBold_24x24, ">", _right_arrow_rect.x, _right_arrow_rect.y, _text_alpha);
	
	// Draw the name of the particle mode.
	GET_FONT->setGapOffset(-10);
	GET_FONT->drawString(
						 eFontCalibriBold_24x24, 
						 _particle_mode.name, 
						 (SCRN_W - GET_FONT->getStringWidth(eFontCalibriBold_24x24, _particle_mode.name)) / 2,
						 MODE_SELECTOR_Y,
						 _text_alpha);
	
	
#if defined (ENABLE_OGL_MENU)
	// Draw the particle properties selector buttons and text.
	GET_FONT->drawString(eFontCalibriBold_24x24, "<", _left_prop_rect.x, _left_prop_rect.y, _text_alpha);
	GET_FONT->drawString(eFontCalibriBold_24x24, ">", _right_prop_rect.x, _right_prop_rect.y, _text_alpha);
	GET_FONT->setGapOffset(-10);
	GET_FONT->drawString(
						 eFontCalibriBold_24x24,
						 _particle_sys.getPropName(_prop_id),
						 (SCRN_W - GET_FONT->getStringWidth(eFontCalibriBold_24x24, _particle_sys.getPropName(_prop_id))) / 2,
						 PROP_SELECTOR_Y,
						 _text_alpha);
	
	// Draw the inc and dec buttons.
	GET_FONT->drawString(eFontCalibriBold_24x24, "+", _inc_rect.x, _inc_rect.y, _text_alpha);
	GET_FONT->drawString(eFontCalibriBold_24x24, "-", _dec_rect.x, _dec_rect.y, _text_alpha);
	
	// Draw the current value of the selected property.
	GET_FONT->drawString(
						 eFontCalibriBold_24x24, 
						 getPropertyValueStr(0, _prop_id), 
						 (SCRN_W - GET_FONT->getStringWidth(eFontCalibriBold_24x24, getPropertyValueStr(0, _prop_id))) / 2,
						 _inc_rect.y + ((_inc_rect.y - GET_FONT->getFontCharHeight(eFontCalibriBold_24x24)) / 2),
						 _text_alpha);
	
#endif
	
	if (_particle_mode.mode == eParticleDispMode_bigbang)
	{
		_rewind_sprite.setAlpha(_text_alpha);
		// Draw rewind button.
		CGraphics::drawSprite(_rewind_sprite);
	}

	_restart_sprite.setAlpha(_text_alpha);
	// Draw restart button.
	CGraphics::drawSprite(_restart_sprite);
	
	_menu_sprite.setAlpha(_text_alpha);
	// Draw menu button.
	CGraphics::drawSprite(_menu_sprite);
	
	// Depending on whether we are paused or not, draw the pause or resume icons.
	if (_is_running)
	{
		_pause_sprite.setAlpha(_text_alpha);
		// Draw pause button.
		CGraphics::drawSprite(_pause_sprite);
	}
	else
	{
		_resume_sprite.setAlpha(_text_alpha);
		// Draw resume button.
		CGraphics::drawSprite(_resume_sprite);
	}

}

void CMainScreen::handleTouch(float x, float y, eTouchPhase phase)
{	
	//GET_SOUND->playSound(FILE_ID_SOUND_BONK2);
	
	_text_fade_counter = 0;
	
	// Galaxy particle mode lets the user control the view location, so we need to pass the touch info along to the camera in this case.
	//if (_particle_mode.mode == eParticleDispMode_gravwell)
	if (_particle_sys.is3DEnabled(0))
	{
		_camera.handleTouch(x, y, phase);
	}
	
	if ((phase == TouchPhaseBegan) || (phase == TouchPhaseMoved))
	{
		// Any touch action will make the text appear immediately.
		_text_alpha = 1.0;
	}
	
	if (phase == TouchPhaseBegan)
	{		
#if defined (ENABLE_OGL_MENU)
		
		// Check for whether the user pressed the arrows to change particle values
		if (isPointInRect(x, y, _inc_rect))
		{
			// Reset timer value when a touch is first recieved.
			_touch_value_timer = VALUE_TOUCH_HOLD_TIME;
			_touch_value_dir = PROP_DIR_INC;
			for (int i = 0; i < mode_data[_particle_mode.mode].num_masses; ++i)
			{
				setPropertyInc(i, _prop_id, _touch_value_dir);
			}
		}
		
		if (isPointInRect(x, y, _dec_rect))
		{
			// Reset timer value when a touch is first recieved.
			_touch_value_timer = VALUE_TOUCH_HOLD_TIME;
			_touch_value_dir = PROP_DIR_DEC;
			for (int i = 0; i < mode_data[_particle_mode.mode].num_masses; ++i)
			{
				setPropertyInc(i, _prop_id, _touch_value_dir);
			}
		}
#endif	
		
		// Handle rewind button touch.
		if (isPointInRect(x, y, _rewind_rect) && (_particle_mode.mode == eParticleDispMode_bigbang))
		{
			// Automatically unpause if we try to rewind.
			if (!_is_running)
			{
				resume();
			}
			
			for (int i = 0; i < mode_data[_particle_mode.mode].num_masses; ++i)
			{
				_particle_sys.setPhysicsState(i, PHYSICS_MOVEMENT_REWIND);
			}
		}

		// Handle pause button touch.
		if (isPointInRect(x, y, _pause_rect))
		{
			if (_is_running)
			{
				pause();
			}
			else 
			{
				resume();
			}
		}
		
		// Handle restart button touch.
		if (isPointInRect(x, y, _restart_rect))
		{
			// Reset the mode for all masses.			
			for (int i = 0; i < mode_data[_particle_mode.mode].num_masses; ++i)
			{
				_particle_sys.reset(i);
			}
			
			// Set a wait time.
			_wait_time = WAIT_TIME;
		}
		
		// Handle menu button touch.
		if (isPointInRect(x, y, _menu_rect))
		{			
			// Pop up the modal view.
			showModalView();
		}
		
		// We need to account for when mutiple updates occur between each touch action.
		// This happens when the phone chugs, so the values are changed more than once per touch.
		// This is especially important when we are changing particle images or boolean values.
		if ((_prop_id == PROP_IMAGE_ID) || (_prop_id == PROP_GLOWS) || (_prop_id == PROP_3D_MODE))
		{
			// This will effectively prevent touch-hold value rolling.
			_touch_value_timer = -1;
			_touch_value_counter = 0;
			_touch_value_mult = 1;
		}
	}
	
	if (phase == TouchPhaseEnded)
	{
		// Check for whether the user pressed the arrows to change particle modes.
		if (isPointInRect(x, y, _left_arrow_rect))
		{
			_particle_mode.mode--;
			if (_particle_mode.mode < 0)
			{
				_particle_mode.mode = eParticleDispMode_MAX - 1;
			}
			setMode(_particle_mode.mode);
		}
		
		if (isPointInRect(x, y, _right_arrow_rect))
		{
			_particle_mode.mode++;
			if (_particle_mode.mode >= eParticleDispMode_MAX)
			{
				_particle_mode.mode = 0;
			}
			setMode(_particle_mode.mode);
		}
		
#if defined (ENABLE_OGL_MENU)	
		if (isPointInRect(x, y, _left_prop_rect))
		{
			_prop_id--;
			
			if (_prop_id < 0)
			{
				_prop_id = PROP_MAX - 1;
			}
		}
		
		if (isPointInRect(x, y, _right_prop_rect))
		{
			_prop_id++;
			
			if (_prop_id >= PROP_MAX)
			{
				_prop_id = 0;
			}
		}
#endif
		
		// Reset touch hold time values.
		_touch_value_timer = -1;
		_touch_value_counter = 0;
		_touch_value_mult = 1;
		
		// Reset physics movement states.
		for (int i = 0; i < mode_data[_particle_mode.mode].num_masses; ++i)
		{
			_particle_sys.setPhysicsState(i, PHYSICS_MOVEMENT_FORWARD);
		}
	}
}


void CMainScreen::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	// Galaxy particle mode lets the user zoom in and out using multitouch pinch.
	//if (_particle_mode.mode == eParticleDispMode_gravwell)
	if (_particle_sys.is3DEnabled(0))
	{
		_camera.handleMultiTouch(x1, y1, phase1, x2, y2, phase2);
	}
}


void CMainScreen::showModalView()
{
	SettingsController *sCtrl = [[particlesAppDelegate getInstance] settingsCtrl];
	[[[sCtrl.navController viewControllers] objectAtIndex:0] reload];
	//[sCtrl presentModalViewController:sCtrl.navController animated:YES];
	
	SettingsViewController *svCtrl = [[sCtrl.navController viewControllers] objectAtIndex:0];
	[sCtrl showModal: svCtrl.view];
	
	// Set timer for changing the viewport size.
	_glview_to_modal_time = GL_TO_MODAL_TIME;
	
	_glview_rect.x = 0;
	_glview_rect.y = 0;
	_glview_rect.w = SCRN_W;
	_glview_rect.h = SCRN_H;
	
	// Make all ogl text dissappear.
	_text_alpha = 0.0f;
}


void CMainScreen::dismissModalView()
{
	// Set timer for changing the viewport back to original fullscreen size.
	_modal_to_glview_time = MODAL_TO_GL_TIME;
}


void CMainScreen::pause()
{
	_particle_sys.pause();
	_is_running = FALSE;
}



void CMainScreen::resume(bool shouldWait)
{
	if (shouldWait)
	{
		// Set a short delay when coming back from a pause.
		_resume_time = RESUME_WAIT_TIME;
	}
	_particle_sys.resume();
	_is_running = TRUE;
}


const char* CMainScreen::getPropertyValueStr(const int massID, int propID)
{	
	// Clear text buffer.
	memset(&_particle_val_text_buf, 0, sizeof(char) * TEXT_BUF_SIZE);
	
	// Get the particle property value.
	propVal value = _particle_sys.getPropertyValue(massID, propID);
	
	// Determine the particle property type.
	switch (CParticleSystem::getPropDataType(propID))
	{
		case PROP_DATA_TYPE_INT:
		{			
			if (value.intVal == __INF)
			{
				sprintf(_particle_val_text_buf, "INF");
			}
			else
			{
				
				sprintf(_particle_val_text_buf, "%d", value.intVal);
			}
		}
			break;
			
		case PROP_DATA_TYPE_FLOAT:
		{
			sprintf(_particle_val_text_buf, "%4.2f", value.floatVal);			
		}
			break;
			
		case PROP_DATA_TYPE_BOOL:
		{
			if (value.boolVal)
			{
				sprintf(_particle_val_text_buf, "TRUE");
			}
			else 
			{
				sprintf(_particle_val_text_buf, "FALSE");
			}
		}
			break;
			
		case PROP_DATA_TYPE_STR:
		{
			sprintf(_particle_val_text_buf, "%s", value.strVal);			
		}
			break;
			
		case PROP_DATA_TYPE_GENERIC:
		{
			// Generic type doesn't contain any property value.
		}
			break;
	}

	return _particle_val_text_buf;
}


void CMainScreen::setPropertyInc(const int massID, const int propID, const int direction)
{
	propVal value = _particle_sys.getPropertyValue(massID, propID);
	
	// Special handling for image id.
	if (propID == PROP_IMAGE_ID)
	{
		// Changes the particle image.
		
		// Search for the next image id in the id list.
		for (int i = 0; i < imageMAX; ++i)
		{
			if (value.intVal == image_data[i].image_id)
			{
				// Make sure we don't go out of bounds.
				if (value.intVal < 0)
				{
					if (i == 0)
					{
						value.intVal = image_data[imageMAX - 1].image_id;
					}
					else
					{
						value.intVal = image_data[i - 1].image_id;
					}
				}
				else
				{
					if (i >= imageMAX - 1)
					{
						value.intVal = image_data[0].image_id;
					}
					else
					{
						value.intVal = image_data[i + 1].image_id;
					}
				}
				break;
			}
		}
		
		_particle_sys.setPropertyValue(massID, propID, value);

		// Done with the special case, just return.
		return;
	}
	
	switch(CParticleSystem::getPropDataType(propID))
	{
		case PROP_DATA_TYPE_INT:
		{
			if (direction == PROP_DIR_INC)
			{
				if (value.intVal == __INF)
				{
					value.intVal = 0;
				}
				else
				{
					value.intVal += ((int)_particle_sys.getPropIncrementValue(propID) * _touch_value_mult);
				}
			}
			else if (direction == PROP_DIR_DEC)
			{
				value.intVal -= ((int)_particle_sys.getPropIncrementValue(propID) * _touch_value_mult);
			
				if (value.intVal < 0)
				{
					value.intVal = __INF;
				}
			}
		}
			break;
			
		case PROP_DATA_TYPE_BOOL:
		{
			value.boolVal = !value.boolVal;
		}
			break;
			
		case PROP_DATA_TYPE_FLOAT:
		{
			if (direction == PROP_DIR_INC)
			{
				value.floatVal += (_particle_sys.getPropIncrementValue(propID) * _touch_value_mult);
			}
			else if (direction == PROP_DIR_DEC)
			{
				value.floatVal -= (_particle_sys.getPropIncrementValue(propID) * _touch_value_mult);
			}
		}
			break;
			
		case PROP_DATA_TYPE_STR:
		{
			// This specifies the draw mode since that is the only mode that uses the string data type currently.
			if (direction == PROP_DIR_INC)
			{
				value.intVal += ((int)_particle_sys.getPropIncrementValue(propID) * _touch_value_mult);
			}
			else if (direction == PROP_DIR_DEC)
			{
				value.intVal -= ((int)_particle_sys.getPropIncrementValue(propID) * _touch_value_mult);
			}
		}
			break;
			
		case PROP_DATA_TYPE_GENERIC:
		{
			// This is unspecified.
		}
			break;
	}
	
	_particle_sys.setPropertyValue(massID, propID, value);
}

