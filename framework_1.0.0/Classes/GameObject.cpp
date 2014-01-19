#include "GameObject.h"
#include "MathUtil.h"
#include "Sprite.h"
#include "Graphics.h"
#include <string.h>
#include "physics.h"
#include "physics_types.h"
#include <cmath>
#include "SystemDefines.h"
#include "Engine.h"

CGameObject::CGameObject()
{
	init();
}

CGameObject::~CGameObject()
{
	destroy();
}

void CGameObject::init(float x, float y)
{
	init();
	
	_phys.pos.x = x;
	_phys.pos.y = y;
}

void CGameObject::init()
{
	CPhysics::initPhysicsObject(&_phys);

	_alpha = 1.0;
	_num_sprites = 0;
	_should_wrap_x = FALSE;
	_should_wrap_y = FALSE;
	_is_active = TRUE;
	memset(_sprites, 0, sizeof(CSprite*) * NUM_GO_SPRITES_MAX);
}


void CGameObject::destroy()
{
	for (int i = 0; i < NUM_GO_SPRITES_MAX; ++i)
	{
		//if (_sprites[i])
		{
			_sprites[i].destroy();
			//delete _sprites[i];
			//_sprites[i] = NULL;
		}
	}
}



void CGameObject::addSprite(const CSprite& sprite)
{
	_sprites[_num_sprites] = sprite;
	
	_num_sprites++;
}

void CGameObject::loadImageIntoSprite(CImage* image)
{
	if (_num_sprites >= NUM_GO_SPRITES_MAX)
	{
		DPRINT_GAMEOBJECT("CGameObject::loadImageIntoSprite sprite limit reached");
		return;
	}
	
	//_sprites[_num_sprites] = new CSprite();
	_sprites[_num_sprites].loadSpriteImage(image);
	
	_num_sprites++;
}

void CGameObject::setAlpha(float alpha)
{
	_alpha = alpha;
	
	for (int i = 0; i < _num_sprites; ++i)
	{
		//if (_sprites[i])
		{
			_sprites[i]._color.a = alpha;
		}
	}
}

void CGameObject::update()
{
	if (!_is_active)
	{
		DPRINT_GAMEOBJECT("CGameObject::update Game object is not active, update will not be run.");
		return;
	}
	
	CPhysics::updatePhysics(&_phys);

	// Update sprite rotation values if any.
	for (int i = 0; i < _num_sprites; ++i)
	{
		_sprites[i].updateAnim();
		_sprites[i].updateAction();

		// Check for whether the rotation should be stopped.
		if (_stop_rot_on_vel_zero)
		{
			if ( ((_phys.vel.x == 0.0) && (_phys.vel.y == 0)) ||
				(_phys.x_vel_zeroed_out && _phys.y_vel_zeroed_out) )
			{
				_sprites[i]._delta_angle.zero();
				continue;
			}
		}
		
		if (fabs(_sprites[i]._delta_angle.x) > 0.0)
		{
			if (_sprites[i]._angle.x >= 360.0)
			{
				_sprites[i]._angle.x = 0.0;
			}
			else if (_sprites[i]._angle.x < 0.0)
			{
				_sprites[i]._angle.x = 360.0;
			}
			
			_sprites[i]._angle.x += _sprites[i]._delta_angle.x;
			
			DPRINT_GAMEOBJECT("CGameObject::update _sprites[i]._angle.x:%f \n", _sprites[i]._angle.x);
		}
		
		if (fabs(_sprites[i]._delta_angle.y) > 0.0)
		{
			if (_sprites[i]._angle.y >= 360.0)
			{
				_sprites[i]._angle.y = 0.0;
			}
			else if (_sprites[i]._angle.y < 0.0)
			{
				_sprites[i]._angle.y = 360.0;
			}
			
			_sprites[i]._angle.y += _sprites[i]._delta_angle.y;
			
			DPRINT_GAMEOBJECT("CGameObject::update _sprites[i]._angle.y:%f \n", _sprites[i]._angle.y);
		}
		
		if (fabs(_sprites[i]._delta_angle.z) > 0.0)
		{
			if (_sprites[i]._angle.z >= 360.0)
			{
				_sprites[i]._angle.z = 0.0;
			}
			else if (_sprites[i]._angle.z < 0.0)
			{
				_sprites[i]._angle.z = 360.0;
			}
			
			_sprites[i]._angle.z += _sprites[i]._delta_angle.z;
			
			DPRINT_GAMEOBJECT("CGameObject::update _sprites[i]._angle.z:%f \n", _sprites[i]._angle.z);
		}
	}
	
	switch (_phys.type)
	{
		case ePhysicsTypeCircle:
		{
			
			// Check for whether we need to wrap the object around when it reaches the end of the screen.
			if (_should_wrap_x)
			{
				// We need to make sure we're wrapping in the right direction by checking velocities.
				if (_phys.vel.x < 0)
				{
					if ((_phys.pos.x + ((POCircle*)&_phys)->radius) < 0)
					{
						_phys.pos.x = SCRN_W + ((POCircle*)&_phys)->radius;
					}
				}
				else if (_phys.vel.x > 0)
				{
					if (_phys.pos.x - ((POCircle*)&_phys)->radius > SCRN_W)
					{
						_phys.pos.x = -((POCircle*)&_phys)->radius;
					}
				}
			}
			
			if (_should_wrap_y)
			{
				if (_phys.vel.y < 0)
				{
					if ((_phys.pos.y + ((POCircle*)&_phys)->radius) < 0)
					{
						_phys.pos.y = SCRN_H + ((POCircle*)&_phys)->radius;
					}
				}
				else if (_phys.vel.y > 0)
				{
					if (_phys.pos.y - ((POCircle*)&_phys)->radius > SCRN_H)
					{
						_phys.pos.y = -((POCircle*)&_phys)->radius;
					}
				}
			}
		}
		break;
			
		default:
		{
			
			
			// Check for whether we need to wrap the object around when it reaches the end of the screen.
			if (_should_wrap_x)
			{
				// We need to make sure we're wrapping in the right direction by checking velocities.
				if (_phys.vel.x < 0)
				{
					if ((_phys.pos.x + _width) < 0)
					{
						_phys.pos.x = SCRN_W;
					}
				}
				else if (_phys.vel.x > 0)
				{
					if (_phys.pos.x > SCRN_W)
					{
						_phys.pos.x = -_width;
					}
				}
			}
			
			if (_should_wrap_y)
			{
				if (_phys.vel.y < 0)
				{
					if ((_phys.pos.y + _height) < 0)
					{
						_phys.pos.y = SCRN_H;
					}
				}
				else if (_phys.vel.y > 0)
				{
					if (_phys.pos.y > SCRN_H)
					{
						_phys.pos.y = -_height;
					}
				}
			}
			
		}
		break;
	}
}

void CGameObject::setDimensions(float w, float h)
{
	_width = w;
	_height = h;
}

void CGameObject::setDimAsLargestSpriteDim()
{
	for (int i = 0; i < _num_sprites; ++i)
	{
		if (_width < (_sprites[i].getWidth()))
		{
			_width = (_sprites[i].getWidth());
		}
		if (_height < (_sprites[i].getHeight()))
		{
			_height = (_sprites[i].getHeight());
		}
	}
}


void CGameObject::setPosition(float x, float y)
{
	_phys.pos.x = x;
	_phys.pos.y = y;
}


void CGameObject::setVelocity(float x, float y)
{
	_phys.vel.x = x;
	_phys.vel.y = y;
	_phys.res_vel.x = x;
	_phys.res_vel.y = y;
}


// This is only called when we want to draw the object in a custom place.
// This does not alter the actual location of the object.
void CGameObject::draw(int x, int y)
{
	if (!_is_active)
	{
		DPRINT_GAMEOBJECT("CGameObject::draw Game object is not active, it will not be rendered.");
		return;
	}
	
	for  (int i = 0; i < _num_sprites; ++i)
	{		
		switch (_phys.type)
		{
			case ePhysicsTypeCircle:
			{
				_sprites[i]._x = x - _sprites[i].getHalfWidth();
				_sprites[i]._y = y - _sprites[i].getHalfHeight();
			}
			break;
			
			default:
			{
				_sprites[i]._x = x;
				_sprites[i]._y = y;
			}
			break;
				
		}
		CGraphics::drawSprite(_sprites[i]);
	}
}


void CGameObject::draw()
{
	if (!_is_active)
	{
		DPRINT_GAMEOBJECT("CGameObject::draw Game object is not active, it will not be rendered.");
		return;
	}
	
	for  (int i = 0; i < _num_sprites; ++i)
	{
		switch (_phys.type)
		{
			case ePhysicsTypeCircle:
			{
				_sprites[i]._x = _phys.pos.x - _sprites[i].getHalfWidth();
				_sprites[i]._y = _phys.pos.y - _sprites[i].getHalfHeight();
			}
				break;
				
			default:
			{
				_sprites[i]._x = _phys.pos.x;
				_sprites[i]._y = _phys.pos.y;
			}
				break;
				
		}
		CGraphics::drawSprite(_sprites[i]);
	}
}