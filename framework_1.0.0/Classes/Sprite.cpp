#include "GameData.h"
#include "types.h"
#include "Sprite.h"
#include "Image.h"
#include "SystemDefines.h"
#include "Graphics.h"
#include "Engine.h"
#include "ImageLoader.h"
#include <string.h>
#include <stdlib.h>

CSprite::CSprite()
{
	init();
}

CSprite::~CSprite()
{
	destroy();
}

void CSprite::init()
{	
	memset(_images, 0, sizeof(CImage*) * SPRITE_ANIM_FRAMES_MAX);
	memset(&_did_allocate_image_mem, 0, sizeof(BOOL) * SPRITE_ANIM_FRAMES_MAX);
	memset(&_anim_texture_names, 0, sizeof(GLuint) * SPRITE_ANIM_FRAMES_MAX);
	_x = 0;
	_y = 0;
	_z = 0;
	_num_anim_frames = 0;
	_curr_anim_frame = 0;
	memset(&_anim_frame_speeds_ms, 0, sizeof(int) * SPRITE_ANIM_FRAMES_MAX);
	_anim_frame_time = 0;
	_anim_dir = ANIM_DIR_POS;
	_anim_alpha = 0;
	_color = 1.0f;
	_scale = Vector3(1.0f, 1.0f, 1.0f);
	_angle = Vector3(0.0f, 0.0f, 0.0f);
	_orig_scale = Vector3(0.0f, 0.0f, 0.0f);
	_dest_scale = Vector3(0.0f, 0.0f, 0.0f);
	_delta_scale = Vector3(0.0f, 0.0f, 0.0f);
	_orig_color = 0;
	_dest_color = 0;
	_delta_color = 0;
	_orig_angle = Vector3(0.0f, 0.0f, 0.0f);
	_dest_angle = Vector3(0.0f, 0.0f, 0.0f);
	_delta_angle = Vector3(0.0f, 0.0f, 0.0f);
	_largest_width = 0;
	_largest_height = 0;
	_largest_depth = 0;	
	_largest_half_width = 0;
	_largest_half_height = 0;
	_largest_half_depth = 0;
	_is_visible = TRUE;
	_is_animated = FALSE;
	_action_scale_time = 0;
	_action_scale_time_counter = 0;
	_action_scale_num_pulses = 0;
	_action_color_time = 0;
	_action_color_time_counter = 0;
	_action_color_num_pulses = 0;
	_action_rotate_time = 0;
	_action_rotate_time_counter = 0;
	_action_rotate_num_rots = 0;
	_action_blink_on_time = 0;
	_action_blink_on_time_rand = 0;
	_action_blink_on_time_rand_val = 0;
	_action_blink_on_time_counter = 0;
	_action_blink_off_time = 0;
	_action_blink_off_time_rand = 0;
	_action_blink_off_time_rand_val = 0;
	_action_blink_off_time_counter = 0;
	_num_blinks = 0;
	_action = 0;
	_action_state = 0;
	_rot_dir = eSpriteRotNone;
	_anim_type = ANIM_TYPE_NONE;	
}


void CSprite::init(int x, int y, int z)
{   
	init();
	_x = x;
	_y = y;
	_z = z;
}

void CSprite::init(int x, int y, int w, int h)
{   
	init();
	_x = x;
	_y = y;
	_z = 0;
	_largest_width = w;
	_largest_height = h;
//	_width = w;
//	_height = h;
}

void CSprite::bindSpriteImage(uint32 imageID)
{
	loadSpriteImage(GET_IMGLOADER->getImage(imageID));
}


void CSprite::loadSpriteImage(int imageFileID)
{
	CImage *image = new CImage();
	image->load(imageFileNames[imageFileID]);
	
	// Make sure to set this so that we know that memory was allocated to load this image.
	// Otherwise, we know that we got our image from image loader which handles it's own free-ing.
	_did_allocate_image_mem[_num_anim_frames] = TRUE;
	
	loadSpriteImage(image);
}

void CSprite::loadSpriteImage(char* fileName)
{
	CImage *image = new CImage();
	image->load(fileName);
	
	// Make sure to set this so that we know that memory was allocated to load this image.
	// Otherwise, we know that we got our image from image loader which handles it's own free-ing.
	_did_allocate_image_mem[_num_anim_frames] = TRUE;
	
	loadSpriteImage(image);
}


void CSprite::loadSpriteImage(CImage* image)
{
	if (_num_anim_frames >= SPRITE_ANIM_FRAMES_MAX)
	{
		DPRINT_SPRITE("CSprite::loadSpriteFrameImage animation frame limit reached");
		return;
	}
	
	_images[_num_anim_frames] = image;
	_largest_width = image->getWidth();
	_largest_height = image->getHeight();
	_largest_depth = 0;
	_largest_half_width = image->getWidth() / 2;
	_largest_half_height = image->getHeight() / 2;
	_largest_half_depth = 0;
	
	// Bind the image to a texture name for drawing the texture.
	if (_images[_num_anim_frames])
	{
		glGenTextures (1, &_anim_texture_names[_num_anim_frames]);
		glBindTexture (GL_TEXTURE_2D, _anim_texture_names[_num_anim_frames]);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtering
		
		glTexImage2D (GL_TEXTURE_2D, 
					  0, 
					  image->getGLFormat(),
					  image->getWidth(),
					  image->getHeight(),
					  0, 
					  image->getGLFormat(), 
					  GL_UNSIGNED_BYTE,
					  image->getImageData());
	}
	
	// Increment the animation frames count.
	_num_anim_frames++;
}


int CSprite::getWidth() const
{
	return _images[_curr_anim_frame]->getWidth();
}

int CSprite::getHeight() const
{
	return _images[_curr_anim_frame]->getHeight();
}

int CSprite::getDepth() const
{
	return 0;
}

int CSprite::getHalfWidth() const
{
	return _images[_curr_anim_frame]->getWidth() >> 1;
}

int CSprite::getHalfHeight() const
{
	return _images[_curr_anim_frame]->getHeight() >> 1;
}

int CSprite::getHalfDepth() const
{
	return 0;
}


void CSprite::destroy()
{
	for (int i = 0; i < SPRITE_ANIM_FRAMES_MAX; ++i)
	{
		// Delete the texture before attempting to delete the image itself.
		glDeleteTextures(1, &_anim_texture_names[i]);
		
		if ((_images[i]) && (_did_allocate_image_mem[i]))
		{
			_images[i]->unload();
			delete _images[i];
			_images[i] = NULL;
		}	
	}
}


void CSprite::setCurrAnimFrame(int frame)
{ 
	if (frame > _num_anim_frames) 
	{
		_curr_anim_frame = _num_anim_frames;
	}
	else
	{
		_curr_anim_frame = frame;
	}
}


void CSprite::drawCentered()
{
	draw(_x - getHalfWidth(), _y - getHalfHeight());
}


void CSprite::drawCentered(float x, float y)
{
	draw(x - getHalfWidth(), y - getHalfHeight());
}


void CSprite::draw()
{	
	draw(_x, _y);
}


void CSprite::draw(float x, float y)
{	
	if (_is_visible)
	{
		switch (_anim_type)
		{
		case ANIM_TYPE_ONCE_FADE:
			{
				// Handle fading animations based on direction.
				if (_anim_dir == ANIM_DIR_POS)
				{
					if (_curr_anim_frame < _num_anim_frames - 1)
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_curr_anim_frame + 1], x, y, _anim_texture_names[_curr_anim_frame + 1], _anim_alpha);
					}
					else
					{
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame]);
					}
				}
				else
				{
					if (_curr_anim_frame >= 1)
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_curr_anim_frame - 1], x, y, _anim_texture_names[_curr_anim_frame - 1], _anim_alpha);
					}
					else
					{
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame]);
					}
				}
			}
			break;

		case ANIM_TYPE_LOOP_FADE:
			{
				// Handle fading animations based on direction.
				if (_anim_dir == ANIM_DIR_POS)
				{
					if (_curr_anim_frame >= _num_anim_frames - 1)
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[0], x, y, _anim_texture_names[0], _anim_alpha);
					}
					else
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_curr_anim_frame + 1], x, y, _anim_texture_names[_curr_anim_frame + 1], _anim_alpha);
					}
				}
				else
				{
					if (_curr_anim_frame <= 0)
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_num_anim_frames - 1], x, y, _anim_texture_names[_num_anim_frames - 1], _anim_alpha);
					}
					else
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_curr_anim_frame - 1], x, y, _anim_texture_names[_curr_anim_frame - 1], _anim_alpha);
					}
				}
			}
			break;

		case ANIM_TYPE_ENDTOEND_FADE:
			{
				// Handle fading animations based on direction.
				if (_anim_dir == ANIM_DIR_POS)
				{
					if (_curr_anim_frame < _num_anim_frames - 1)
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_curr_anim_frame + 1], x, y, _anim_texture_names[_curr_anim_frame + 1], _anim_alpha);
					}
					else
					{
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame]);
					}
				}
				else
				{
					if (_curr_anim_frame >= 1)
					{
						// Curent frame fades out.
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame], 1.0 - _anim_alpha);
						// Next frame fades in.
						CGraphics::drawImage(_images[_curr_anim_frame - 1], x, y, _anim_texture_names[_curr_anim_frame - 1], _anim_alpha);
					}
					else
					{
						CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame]);
					}
				}
			}
			break;

		default:
			{
				// No special fading animation, just render normally.
				CGraphics::drawImage(_images[_curr_anim_frame], x, y, _anim_texture_names[_curr_anim_frame]);
			}
			break;
		}
	}
}

void CSprite::setSizeScaleAction(float destScaleX, float destScaleY, float destScaleZ, int time, int numPulses)
{
	_orig_scale = _scale;
	
	_dest_scale.x = destScaleX;
	_dest_scale.y = destScaleY;
	_dest_scale.z = destScaleZ;
	
	_action_scale_time = time;
	_action_scale_time_counter = 0;
	
	_action |= eSpriteActScale;
	_action_scale_num_pulses = numPulses;
	
	// Figure out how much we need to change the scale during each speed tick.
	_delta_scale.x = GLfloat((destScaleX - _scale.x) / time);
	_delta_scale.y = GLfloat((destScaleY - _scale.y) / time);
	_delta_scale.z = GLfloat((destScaleZ - _scale.z) / time);
	
	// This is set every time to state 1 so that in the case of a loop action, we can change back and forth between states 1 and 2.
	_action_state |= (int)eSpriteActStatePulse1;
}

void CSprite::setSizeScaleAction(Vector3 destScale, int time, int numPulses)
{
	setSizeScaleAction(destScale.x, destScale.y, destScale.z, time);
}



void CSprite::setColorPulseAction(float r, float g, float b, float a, int time, int numPulses)
{
	_orig_color = _color;
	
	_dest_color.r = r;
	_dest_color.g = g;
	_dest_color.b = b;
	_dest_color.a = a;
	
	_action_color_time = time;
	_action_color_time_counter = 0;
	
	_action |= eSpriteActColorPulse;
	_action_color_num_pulses = numPulses;
	
	// Figure out how much we need to change the color during each speed tick.
	_delta_color.r = (r - _color.r) / time;
	_delta_color.g = (g - _color.g) / time;
	_delta_color.b = (b - _color.b) / time;
	_delta_color.a = (a - _color.a) / time;
	
	// This is set every time to state 1 so that in the case of a loop action, we can change back and forth between states 1 and 2.
	_action_state |= (int)eSpriteActStateColPulse1;
}

void CSprite::setColorPulseAction(color destColor, int time, int numPulses)
{
	setColorPulseAction(destColor.r, destColor.g, destColor.b, destColor.a, time, numPulses);
}



void CSprite::setRotateAction(Vector3 destAngle, eSpriteRotateDir direction, int time, int numRots)
{
	_orig_angle = _angle;
	_dest_angle = destAngle;
	
	_action_rotate_time = time;
	_action_rotate_time_counter = 0;
	
	_action |= eSpriteActRotate;
	_action_rotate_num_rots = numRots;
	
	// Figure out how much we need to change the angle during each speed tick.
	_delta_angle = (_dest_angle - _angle) / time;
	
	_rot_dir = direction;
	
	// This is set every time to state 1 so that in the case of a loop action, we can change back and forth between states 1 and 2.
	_action_state |= (int)eSpriteActStateRotate1;
}


void CSprite::setBlinkAction(int onTime, int randOnTime, int offTime, int randOffTime, int numBlinks)
{
	_action_blink_on_time = onTime;
	_action_blink_on_time_counter = 0;
	_action_blink_on_time_rand = 0;
	_action_blink_on_time_rand_val = 0;
	// Apply any randomness. Subsequent randomness is applied in updateAction().
	if (randOnTime > 0)
	{
		_action_blink_on_time_rand_val = randOnTime;
		_action_blink_on_time_rand = (rand() % _action_blink_on_time_rand_val);
	}
	
	_action_blink_off_time = offTime;
	_action_blink_off_time_counter = 0;
	_action_blink_off_time_rand = 0;
	_action_blink_off_time_rand_val = 0;
	// Apply any randomness. Subsequent randomness is applied in updateAction().
	if (randOffTime > 0)
	{
		_action_blink_off_time_rand_val = randOffTime;
		_action_blink_off_time_rand = (rand() % _action_blink_off_time_rand_val);
	}
	
	_num_blinks = numBlinks;
	
	_action |= eSpriteActBlink;
	_action_state |= (int)eSpriteActStateBlink1;
}


void CSprite::updateAction()
{	
	if (_action & eSpriteActScale)
	{
		if ((_action_scale_num_pulses <= 0) && (_action_scale_num_pulses != __INF))
		{
			// JC: This doesn't work since the destination size might not be correct depending on the number of pulses.
			// Set the scale to destination scale to account for any error.
			//_scale = _dest_scale;
			
			// Return to the first action state.
			_action_scale_time_counter = 0;
			_action_state &= ~(eSpriteActStatePulse1 | eSpriteActStatePulse2);
			_action &= ~eSpriteActScale;
		}
		else
		{		
			_action_scale_time_counter += TIME_LAST_FRAME;
			
			if (_action_state & eSpriteActStatePulse1)
			{
				_scale += (_delta_scale * TIME_LAST_FRAME);
				
				if (_action_scale_time_counter >= _action_scale_time)
				{
					// Set the scale to destination scale to account for any error.
					//_scale = _dest_scale;
					// Go to the next action state.
					_action_scale_time_counter = 0;
					_action_state &= ~eSpriteActStatePulse1;
					_action_state |= eSpriteActStatePulse2;
					
					if (_action_scale_num_pulses != __INF)
					{
						_action_scale_num_pulses--;
					}
				}
			}
			else if (_action_state & eSpriteActStatePulse2)
			{
				_scale -= (_delta_scale * TIME_LAST_FRAME);
				
				if (_action_scale_time_counter >= _action_scale_time)
				{
					// Set the scale to destination scale to account for any error.
					//_scale = _orig_scale;
					// Return to the first action state.
					_action_scale_time_counter = 0;
					_action_state &= ~eSpriteActStatePulse2;
					_action_state |= eSpriteActStatePulse1;
					
					if (_action_scale_num_pulses != __INF)
					{
						_action_scale_num_pulses--;
					}
				}
			}
		}
	}
	
	if (_action & eSpriteActColorPulse)
	{
		if ((_action_color_num_pulses <= 0) && (_action_color_num_pulses != __INF))
		{
			// JC: This doesn't work since the destination color might not be correct depending on the number of pulses.
			// Set the color to destination color to account for any error.
			//_color = _dest_color;
			
			// Reset action properties.
			_action_color_time_counter = 0;
			_action_color_time = 0;
			_action &= ~eSpriteActColorPulse;
			_action_state &= ~(eSpriteActStateColPulse1 | eSpriteActStateColPulse2);
		}
		else
		{
			_action_color_time_counter += TIME_LAST_FRAME;
			
			if (_action_state & eSpriteActStateColPulse1)
			{
				_color += (_delta_color * TIME_LAST_FRAME);
				
				if (_action_color_time_counter >= _action_color_time)
				{
					// Set the color to destination color to account for any error.
					//_color = _dest_color;
					// Go to the next action state.
					_action_color_time_counter = 0;
					_action_state &= ~eSpriteActStateColPulse1;
					_action_state |= eSpriteActStateColPulse2;
					
					if (_action_color_num_pulses != __INF)
					{
						_action_color_num_pulses--;
					}
				}
			}
			else if (_action_state & eSpriteActStateColPulse2)
			{
				_color -= (_delta_color * TIME_LAST_FRAME);
				
				if (_action_color_time_counter >= _action_color_time)
				{
					// Set the color to destination color to account for any error.
					//_color = _orig_color;
					// Return to the first action state.
					_action_color_time_counter = 0;
					_action_state &= ~eSpriteActStateColPulse2;
					_action_state |= eSpriteActStateColPulse1;
					
					if (_action_color_num_pulses != __INF)
					{
						_action_color_num_pulses--;
					}
				}
			}
		}
	}
	
	if (_action & eSpriteActRotate)
	{
		// Check for whether the rotation count has been reached.
		if ((_action_rotate_num_rots <= 0) && (_action_rotate_num_rots != __INF))
		{
			// JC: This doesn't work since the destination angle might not be correct depending on the number of pulses.
			// Set the rotation angle to the destination angle to account for any error.
			//_angle = _dest_angle;
			
			// Reset action properties.
			_action_rotate_time_counter = 0;
			_action_rotate_time = 0;
			_action &= ~eSpriteActRotate;
			_action_state &= ~(eSpriteActStateRotate1 | eSpriteActStateRotate2);
		}
		else
		{
			_action_rotate_time_counter += TIME_LAST_FRAME;
			
			if (_action_state & eSpriteActStateRotate1)
			{
				if (_rot_dir == eSpriteRotClock)
				{
					_angle += (_delta_angle * TIME_LAST_FRAME);
				}
				else if (_rot_dir == eSpriteRotCounterClock)
				{
					_angle -= (_delta_angle * TIME_LAST_FRAME);
				}
				
				if (_action_rotate_time_counter >= _action_rotate_time)
				{
					// Reverse rotation directions.
					if (_rot_dir == eSpriteRotClock)
					{
						_rot_dir = eSpriteRotCounterClock;
					}
					else if (_rot_dir == eSpriteRotCounterClock)
					{
						_rot_dir = eSpriteRotClock;
					}
					
					// Set the rotation angle to the destination angle to account for any error.
					//_angle = _dest_angle;
					// Reset action properties.
					_action_rotate_time_counter = 0;
					_action_state &= ~eSpriteActStateRotate1;
					_action_state |= eSpriteActStateRotate2;
					
					if (_action_rotate_num_rots != __INF)
					{
						_action_rotate_num_rots--;
					}
				}
			}
			else if (_action_state & eSpriteActStateRotate2)
			{
				if (_rot_dir == eSpriteRotClock)
				{
					_angle -= (_delta_angle * TIME_LAST_FRAME);
				}
				else if (_rot_dir == eSpriteRotCounterClock)
				{
					_angle += (_delta_angle * TIME_LAST_FRAME);
				}
				
				if (_action_rotate_time_counter >= _action_rotate_time)
				{
					// Reverse rotation directions.
					if (_rot_dir == eSpriteRotClock)
					{
						_rot_dir = eSpriteRotCounterClock;
					}
					else if (_rot_dir == eSpriteRotCounterClock)
					{
						_rot_dir = eSpriteRotClock;
					}
					
					// Set the rotation angle to the destination angle to account for any error.
					//_angle = _orig_angle;
					// Reset action properties.
					_action_rotate_time_counter = 0;
					_action_state &= ~eSpriteActStateRotate2;
					_action_state |= eSpriteActStateRotate1;
					
					if (_action_rotate_num_rots != __INF)
					{
						_action_rotate_num_rots--;
					}
				}
			}
		}
	}
	
	if (_action & eSpriteActBlink)
	{
		if ((_num_blinks <= 0) && (_num_blinks != __INF))
		{
			// Always end with the sprite being visible.
			_is_visible = TRUE;
			_action_state &= ~(eSpriteActStateBlink1 | eSpriteActStateBlink2);
			_action &= ~eSpriteActBlink;
		}
		else
		{
			if (_action_state & eSpriteActStateBlink1)
			{
				_action_blink_on_time_counter += TIME_LAST_FRAME;
				// Once we have reached the end of this action state, we will go to the next, which will make the sprite invisible.
				if (_action_blink_on_time_counter >= (_action_blink_on_time + _action_blink_on_time_rand))
				{
					// Check if there is a randomness time to be added.
					if (_action_blink_off_time_rand_val > 0)
					{
						_action_blink_off_time_rand = (rand() % _action_blink_off_time_rand_val);
					}
					// Set the next action state.
					_action_state &= ~eSpriteActStateBlink1;
					_action_state |= eSpriteActStateBlink2;
					// Set visibility to off here.
					_is_visible = FALSE;
					// Decrement the blink counter if it is not infinity.
					if (_num_blinks != __INF)
					{
						_num_blinks--;
					}
				}
			}
			else if (_action_state & eSpriteActStateBlink2)
			{
				_action_blink_off_time_counter += TIME_LAST_FRAME;
				// Once we have reached the end of this action state, we will go to the next, which will make the sprite invisible.
				if (_action_blink_off_time_counter >= (_action_blink_off_time + _action_blink_off_time_rand))
				{
					// Check if there is a randomness time to be added.
					if (_action_blink_on_time_rand_val > 0)
					{
						_action_blink_on_time_rand = (rand() % _action_blink_on_time_rand_val);
					}
					// Set the next action state.
					_action_state &= ~eSpriteActStateBlink2;
					_action_state |= eSpriteActStateBlink1;
					// Set visibility to on here.
					_is_visible = TRUE;
					// Decrement the blink counter if it is not infinity.
					if (_num_blinks != __INF)
					{
						_num_blinks--;
					}
				}
			}
		}
		
	}
	
	if (_action & eSpriteActNone)
	{
		_action = (eSpriteAct)0;
	}
}


void CSprite::updateAnim()
{
	switch(_anim_type)
	{
		case ANIM_TYPE_ONCE:
		case ANIM_TYPE_ONCE_FADE:
		{
			_anim_frame_time += TIME_LAST_FRAME;
			if (_anim_frame_time >= _anim_frame_speeds_ms[_curr_anim_frame])
			{
				_anim_frame_time = 0;
				
				// Depending on direction, we will either increment the current animation frame or decrement it.
				if (_anim_dir == ANIM_DIR_POS)
				{
					_curr_anim_frame++;
				}
				else if (_anim_dir == ANIM_DIR_NEG)
				{
					_curr_anim_frame--;
				}
				
				// End the animation.
				if (_curr_anim_frame < 0)
				{
					_curr_anim_frame = 0;
					_anim_type = ANIM_TYPE_NONE;
				}
				if (_curr_anim_frame >= _num_anim_frames)
				{
					_curr_anim_frame = _num_anim_frames - 1;
					_anim_type = ANIM_TYPE_NONE;
				}

				if (_anim_type == ANIM_TYPE_ONCE_FADE)
				{
					// Reset alpha for the next frame.
					_anim_alpha = 0.0;
				}
			}
			else
			{
				if (_anim_type == ANIM_TYPE_ONCE_FADE)
				{
					// Calculate the alpha blend based on frame time.
					_anim_alpha = (float)_anim_frame_time / _anim_frame_speeds_ms[_curr_anim_frame];
				}
			}

		}
			break;

		case ANIM_TYPE_ENDTOEND:
		case ANIM_TYPE_ENDTOEND_FADE:
		{
			_anim_frame_time += TIME_LAST_FRAME;
			if (_anim_frame_time >= _anim_frame_speeds_ms[_curr_anim_frame])
			{
				_anim_frame_time = 0;
				
				// Depending on direction, we will either increment the current animation frame or decrement it.
				if (_anim_dir == ANIM_DIR_POS)
				{
					_curr_anim_frame++;
				}
				else if (_anim_dir == ANIM_DIR_NEG)
				{
					_curr_anim_frame--;
				}
				
				// Switch anim directions to positive when we reach the first frame.
				if (_curr_anim_frame < 0)
				{
					// We will set the frame to 1 here to prevent the first frame from drawing twice in a row.
					_curr_anim_frame = 1;
					_anim_dir = ANIM_DIR_POS;
				}
				
				// Switch animation directions to negative when we reach the last frame.
				if (_curr_anim_frame >= _num_anim_frames)
				{
					_curr_anim_frame = _num_anim_frames - 1;
					_anim_dir = ANIM_DIR_NEG;
				}
					
				if (_anim_type == ANIM_TYPE_ENDTOEND_FADE)
				{
					// Reset alpha for the next frame.
					_anim_alpha = 0.0;
				}
			}
			else
			{
				if (_anim_type == ANIM_TYPE_ENDTOEND_FADE)
				{
					// Calculate the alpha blend based on frame time.
					_anim_alpha = (float)_anim_frame_time / _anim_frame_speeds_ms[_curr_anim_frame];
				}
			}
		}
			break;
			
		case ANIM_TYPE_LOOP:
		case ANIM_TYPE_LOOP_FADE:
		{
			_anim_frame_time += TIME_LAST_FRAME;
			if (_anim_frame_time >= _anim_frame_speeds_ms[_curr_anim_frame])
			{
				_anim_frame_time = 0;
				
				// Depending on direction, we will either increment the current animation frame or decrement it.
				if (_anim_dir == ANIM_DIR_POS)
				{
					_curr_anim_frame++;
				}
				else if (_anim_dir == ANIM_DIR_NEG)
				{
					_curr_anim_frame--;
				}
				
				// Loop around to the other end of the animation.
				if (_curr_anim_frame < 0)
				{
					_curr_anim_frame = _num_anim_frames - 1;
				}
				if (_curr_anim_frame >= _num_anim_frames)
				{
					_curr_anim_frame = 0;
				}

				if (_anim_type == ANIM_TYPE_LOOP_FADE)
				{
					// Reset alpha for the next frame.
					_anim_alpha = 0.0;
				}
			}
			else
			{
				if (_anim_type == ANIM_TYPE_LOOP_FADE)
				{
					// Calculate the alpha blend based on frame time.
					_anim_alpha = (float)_anim_frame_time / _anim_frame_speeds_ms[_curr_anim_frame];
				}
			}
		}
			break;
			
		case ANIM_TYPE_NONE:
		default:
			return;
			
	}
}

void CSprite::setFrameSpeedsMS(int speed)
{
	for (int i = 0; i < SPRITE_ANIM_FRAMES_MAX; ++i)
	{
		_anim_frame_speeds_ms[i] = speed;
	}
}