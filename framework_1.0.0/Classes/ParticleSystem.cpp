/*
 *  ParticleSystem.cpp
 *  Wurd
 *
 *  Created by Johnny Chan on 3/12/10.
 *  Copyright 2010 LlamaFace. All rights reserved.
 *
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include "MathUtil.h"
#include "Utils.h"
#include "Engine.h"
#include "ImageLoader.h"
#include "SystemDefines.h"
#include "physics_types.h"
#include "physics.h"
#include "Image.h"
#include "Sprite.h"
#include "GameObject.h"
#include "GameData.h"
#include "Graphics.h"
#include "ParticleSystem.h"


#if defined (ENABLE_PARTICLESYSTEM)
// Used as a reference when querying the string name of a specific particle property.
const particlePropNames propNames[PROP_MAX] = 
{
	{ PROP_LIFE, PROP_DATA_TYPE_INT, 1.0f, "Life Span", "Life span of the particle, measured in milliseconds. Range: 0 to INF", },
	{ PROP_FADE_SPEED, PROP_DATA_TYPE_INT, 1.0f, "Fade Speed", "Time in milliseconds that the particle fades out of visibility. Range: 0 to INF", },
	{ PROP_FADE_COUNT, PROP_DATA_TYPE_INT, 1.0f, "Fade Count", "The number times the particle fades in and out of visibility. Range: 0 to INF", },
	{ PROP_SLOWDOWN_RATE, PROP_DATA_TYPE_FLOAT, 0.01f, "Slowdown Rate", "Speed deterioration of the particle. Range: 0.0 to 100.0" },
	{ PROP_VEL_BASE, PROP_DATA_TYPE_INT, 1.0f, "Base Velocity", "Initial velocity of the particle, measured in pixels per frame. Range: 0 to INF", },
	{ PROP_VEL_RAND, PROP_DATA_TYPE_INT, 1.0f, "Velocity Randomness", "Randomness factor of the particle velocity. The value determines the range of the randomness. Range: 0 to INF", },
	{ PROP_ANGLE_MIN, PROP_DATA_TYPE_INT, 1.0f, "Start Angle", "The starting angle at which particles may be released. Range: 0 to 360", },
	{ PROP_ANGLE_MAX, PROP_DATA_TYPE_INT, 1.0f, "End Angle", "The ending angle at which particles may be released. Range: 0 to 360", },
	{ PROP_REL_TIME, PROP_DATA_TYPE_INT, 1.0f, "Release Time Interval", "Time in milliseconds at which particles are emitted. Range: 0 to INF", },
	{ PROP_REL_RATE, PROP_DATA_TYPE_INT, 1.0f, "Release Rate", "The number of particles emitted at each time interval. Range: 0 to number of particles", },
	{ PROP_REL_RAND, PROP_DATA_TYPE_INT, 1.0f, "Release Time Randomness", "The random time factor at which particle are released, measured in milliseconds. Range: 0 to INF", },
	{ PROP_GRAVITY, PROP_DATA_TYPE_FLOAT, 0.1f, "Gravity", "Gravity is measured in pixels/second^2. Range: 0.0 to INF", },
	{ PROP_ROT_SPEED, PROP_DATA_TYPE_INT, 1.0f, "Rotation Speed", "One full rotation occurs at the given millisecond value. Range: 0 to INF", },
	{ PROP_ROT_RAND, PROP_DATA_TYPE_INT, 1.0f, "Rotation Speed Randomness", "the randomness time factor for rotation speed. Range: 0 to INF", },
	{ PROP_SIZE_START, PROP_DATA_TYPE_FLOAT, 0.1f, "Starting Size", "Initial size of the particle. 1.0 represents the original size of the particle. Range: 0.0 to INF", },
	{ PROP_SIZE_END, PROP_DATA_TYPE_FLOAT, 0.1f, "Ending Size", "Ending size of the particle. 1.0 represents the original size of the particle. Range: 0.0 to INF", },
	{ PROP_SIZE_SPEED, PROP_DATA_TYPE_INT, 1.0f, "Size Change Speed", "The time in milliseconds that the particle size changes from start to end. Range: 0 to INF", },
	{ PROP_SIZE_COUNT, PROP_DATA_TYPE_INT, 1.0f, "Size Change Count", "The number of times the the particle will change sizes. Range: 0 to INF", },
	{ PROP_BLINK_ON_TIME, PROP_DATA_TYPE_INT, 1.0f, "Blink-On Time", "The time in milliseconds that the particle is visible during a blink action. Range: 0 to INF", },
	{ PROP_BLINK_ON_RAND, PROP_DATA_TYPE_INT, 1.0f, "Blink-On Randomness", "The random time factor of the blink-on action. Range: 0 to INF", },
	{ PROP_BLINK_OFF_TIME, PROP_DATA_TYPE_INT, 1.0f, "Blink-Off Time", "The time in milliseconds that the particle is not visible during a blink action. Range: 0 to INF", },
	{ PROP_BLINK_OFF_RAND, PROP_DATA_TYPE_INT, 1.0f, "Blink-Off Randomness", "The random time factor of the blink-off action. Range: 0 to INF", },
	{ PROP_BLINK_COUNT, PROP_DATA_TYPE_INT, 1.0f, "Blink Count", "The number of times the the particle blinks. Range: 0 to INF", },
	{ PROP_REL_DIST, PROP_DATA_TYPE_INT, 1.0f, "Release Distance", "The distance that the particle are emitted from the particle mass center, measure in pixels. Range: 0 to INF", },
	{ PROP_REL_DIST_RAND, PROP_DATA_TYPE_INT, 1.0f, "Release Distance Randomness", "A random range value for release distance. Range: 0 to INF", },
	{ PROP_GLOWS, PROP_DATA_TYPE_BOOL, 0.0f, "Glow", "Disables/enables the Open GL alpha function, resulting in a glow effect (or not).",  },
	{ PROP_COLOR_R, PROP_DATA_TYPE_FLOAT, 0.01f, "Red Component", "The red color component of the particle. Range: 0.0 to 1.0", },
	{ PROP_COLOR_G, PROP_DATA_TYPE_FLOAT, 0.01f, "Green Component", "The green color component of the particle. Range: 0.0 to 1.0", },
	{ PROP_COLOR_B, PROP_DATA_TYPE_FLOAT, 0.01f, "Blue Component", "The blue color component of the particle. Range: 0.0 to 1.0", },
	{ PROP_COLOR_RAND, PROP_DATA_TYPE_BOOL, 0.0f, "Color Randomization", "Disables/enables color randomization.", },
	{ PROP_IMAGE_ID, PROP_DATA_TYPE_GENERIC, 0.0f, "Particle Image", "The image used in the current active particle mode.", },
	{ PROP_MASS_SIZE, PROP_DATA_TYPE_INT, 1.0f, "Particle Count", "The total number of particles in the current particle mass. When a particle dissappears, it becomes emittable again. Range: 0 to 4000", },
	{ PROP_STRAND_LENGTH, PROP_DATA_TYPE_INT, 1.0f, "Strand Length", "The length of the strand of the particles. This creates a trail or streak effect. Range: 0 to 4000", },
	{ PROP_3D_MODE, PROP_DATA_TYPE_BOOL, 0.0f, "Enable 3D", "Disables/enables 3D rendering mode. Touch controls are enabled when this mode is enabled.", },
	{ PROP_DRAW_MODE, PROP_DATA_TYPE_STR, 1.0f, "Draw Mode", "Quad mode is used for 2D rendering, billboard mode is used for 3D, and point sprite mode can be used for both.", },
	{ PROP_DRAW_EMITTER, PROP_DATA_TYPE_BOOL, 0.0f, "Draw Emitter", "Disables/enables the point of origin of the particle mass; the emitter.", },
};

// Used as a reference when querying the string name of the current draw mode.
const char* drawModeStr[eParticleDrawModeMAX] =
{
	"QUAD (2D)",
	"BILLBOARD (3D)",
	"POINT SPRITE",	
};

const char* CParticleSystem::getPropName(int id)
{
	return propNames[id].type_name;
}

const char* CParticleSystem::getDrawModeStr(int id)
{
	return drawModeStr[id];
}

const int CParticleSystem::getPropDataType(int id)
{
	return propNames[id].data_type;
}

const char* CParticleSystem::getPropDesc(int id)
{
	return propNames[id].description;
}

const float CParticleSystem::getPropIncrementValue(int id)
{
	return propNames[id].increment_value;
}

CParticleSystem::CParticleSystem()
{
	//_mass = NULL;
	_num_masses = 0;
	_is_running = FALSE;
	memset(&_point_sizes, 0, sizeof(GLfloat) * 2);
}


CParticleSystem::~CParticleSystem()
{
	
}


void CParticleSystem::initMass(const int massID, const int massSize)
{	
	// It is assumed that the particle sprite has already been loaded!!!
	CImage* particle_image = GET_IMGLOADER->getImage(FILE_ID_IMAGE_PARTICLE);
	
	if (!particle_image)
	{
		DPRINT_PARTICLESYS("CParticleSystem::initMass failed: Particle image not present");
		return;
	}
		
	// Very important to store the number of particles per _mass!
	_mass[massID].num_particles = massSize;
	
	// Init _mass center particle sprite and physics object. This will always be the center of the _mass.
	_mass[massID].center.sprite.init();
	_mass[massID].center.sprite.loadSpriteImage(particle_image);
	CPhysics::initCircleObject((POCircle*)&_mass[massID].center.phys, PARTICLE_RADIUS_DEFAULT);
	_mass[massID].center.id = massID;

	// Allocate memory for the individual particles.
	_mass[massID].particles = ArrayList<particle>::alloc(massSize);
	
	for (int j = 0; j < massSize; ++j)
	{	
		// Init individual particle sprite and physics object.
		_mass[massID].particles[j].sprite.init();
		_mass[massID].particles[j].sprite.loadSpriteImage(particle_image);
		CPhysics::initCircleObject((POCircle*)&_mass[massID].particles[j].phys, PARTICLE_RADIUS_DEFAULT);
		_mass[massID].particles[j].id = _mass[massID].center.id;
		_mass[massID].particles[j].pos_history_counter = 0;
		_mass[massID].particles[j].pos_history_active_count = 0;
	}
	
	// This essentially means that this mass will keep ejecting it's particle indefinitely.
	_mass[massID].loop_count = __INF;
}


void CParticleSystem::init(const int numMasses)
{
	// It is assumed that the particle sprite has already been loaded!!!
	CImage* particle_image = GET_IMGLOADER->getImage(FILE_ID_IMAGE_PARTICLE);
	
	if (!particle_image)
	{
		DPRINT_PARTICLESYS("CParticleSystem::init failed: Particle image not present");
		return;
	}
	
	_mass = ArrayList<particleMass>::alloc(numMasses);
	
	_num_masses = numMasses;
}


void CParticleSystem::init(const int numMasses, const int massSize)
{
	destroy();
	
	// It is assumed that the particle sprite has already been loaded!!!
	CImage* particle_image = GET_IMGLOADER->getImage(FILE_ID_IMAGE_PARTICLE);
	
	if (!particle_image)
	{
		DPRINT_PARTICLESYS("CParticleSystem::init failed: Particle image not present");
		return;
	}
	
	_mass = ArrayList<particleMass>::alloc(numMasses);
	_num_masses = numMasses;
	
	for (int i = 0; i < numMasses; ++i)
	{		
		initMass(i, massSize);
	}
}


void CParticleSystem::destroy()
{
	if (_mass.length() <= 0)
	{
		return;
	}
	
	for (int i = 0; i < _num_masses; ++i)
	{
		for (int j = 0; j < _mass[i].num_particles; ++j)
		{
			_mass[i].particles[j].sprite.destroy();
		}
		_mass[i].particles = NULL;
		_mass[i].center.sprite.destroy();
	}
	_mass = NULL;
}


void CParticleSystem::draw(void* data)
{	
	if (_mass.length() <= 0)
	{
		return;
	}
		
	for (int i = 0; i < _num_masses; ++i)
	{
		eParticleDrawMode drawMode = (eParticleDrawMode)_mass[i].center.props.draw_mode;
		
		switch (drawMode)
		{
			case eParticleDrawModeNormal:
			{
				if (!_mass[i].center.is_active)
				{
					continue;
				}
				
				if (_mass[i].center.props.glows)
				{
					// This causes colors to be additive when particles overlap each other, creating a "glow" effect.
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					// Turn off depth masking so particles in front will not occlude particles behind them.
					glDepthMask(GL_FALSE);				
				}
				
				for (int j = 0; j < _mass[i].num_particles; ++j)
				{
					if (_mass[i].particles[j].is_active)
					{
						// Call different rendering methods depending on which mode is enabled.
						if (_mass[i].center.props.is_3D_enabled)
						{
							// Draw strands if enabled.
							if (_mass[i].particles[j].props.strand_length > 0)
							{
								for (int k = 0; k < _mass[i].particles[j].pos_history_active_count; ++k)
								{
									CGraphics::draw3DSpriteCentered(
																	&_mass[i].particles[j].sprite, 
																	_mass[i].particles[j].pos_history[k].x,
																	_mass[i].particles[j].pos_history[k].y, 
																	_mass[i].particles[j].pos_history[k].z);									
								}
							}
							
							// Draw the particle itself in 3d.
							CGraphics::draw3DSpriteCentered(
															&_mass[i].particles[j].sprite, 
															_mass[i].particles[j].phys.pos.x, 
															_mass[i].particles[j].phys.pos.y, 
															_mass[i].particles[j].phys.pos.z);
							
						}
						else
						{
							// Draw strands if enabled.
							if (_mass[i].particles[j].props.strand_length > 0)
							{
								for (int k = 0; k < _mass[i].particles[j].pos_history_active_count; ++k)
								{
									CGraphics::drawSpriteCentered(
																  &_mass[i].particles[j].sprite, 
																  _mass[i].particles[j].pos_history[k].x,
																  _mass[i].particles[j].pos_history[k].y);									
								}
							}
							
							// Draw the particle itself in 2d.
							CGraphics::drawSpriteCentered(
														  &_mass[i].particles[j].sprite, 
														  _mass[i].particles[j].phys.pos.x, 
														  _mass[i].particles[j].phys.pos.y);
						}
					}
				}
				
				if (_mass[i].center.props.draw_emitter)
				{
					// Call different rendering methods depending on which mode is enabled.
					if (_mass[i].center.props.is_3D_enabled)
					{
						CGraphics::draw3DSpriteCentered(
														&_mass[i].center.sprite, 
														_mass[i].center.phys.pos.x, 
														_mass[i].center.phys.pos.y, 
														_mass[i].center.phys.pos.z);
					}
					else
					{
						CGraphics::drawSpriteCentered(
													  &_mass[i].center.sprite, 
													  _mass[i].center.phys.pos.x, 
													  _mass[i].center.phys.pos.y);
					}
				}
				
				// The 'glow' effect is actually just a different blend function.
				if (_mass[i].center.props.glows)
				{
					glDepthMask(GL_TRUE);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			
				}
			}
				break;
				
			case eParticleDrawModeBillBoard:
			{			
				float* camMat = (float*)data;
				
				if (!_mass[i].center.is_active)
				{
					continue;
				}
				
				// The billboard drawing mode only draws in 3D mode. This is because it is useless to front-face the particles towards the camera if we are in 2D mode.
				if (!_mass[i].center.props.is_3D_enabled)
				{
					DPRINT_PARTICLESYS("CParticleSystem::draw billboard mode is only available when 3D is enabled! \n");
					continue;
				}
				
				if (_mass[i].center.props.glows)
				{
					// This causes colors to be additive when particles overlap each other, creating a "glow" effect.
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					// Turn off depth masking so particles in front will not occlude particles behind them.
					glDepthMask(GL_FALSE);				
				}
				
				for (int j = 0; j < _mass[i].num_particles; ++j)
				{
					if (_mass[i].particles[j].is_active)
					{
						// Draw strands if enabled.
						if (_mass[i].particles[j].props.strand_length > 0)
						{
							for (int k = 0; k < _mass[i].particles[j].pos_history_active_count; ++k)
							{
								CGraphics::draw3DSpriteCenteredLookAt(
																	  &_mass[i].particles[j].sprite, 
																	  _mass[i].particles[j].pos_history[k].x,
																	  _mass[i].particles[j].pos_history[k].y, 
																	  _mass[i].particles[j].pos_history[k].z,
																	  camMat);
							}
						}
						
						// Draw the particle itself in 3d.
						CGraphics::draw3DSpriteCenteredLookAt(
															  &_mass[i].particles[j].sprite, 
															  _mass[i].particles[j].phys.pos.x, 
															  _mass[i].particles[j].phys.pos.y, 
															  _mass[i].particles[j].phys.pos.z, 
															  camMat);
					}
				}
				
				if (_mass[i].center.props.draw_emitter)
				{					
					CGraphics::draw3DSpriteCenteredLookAt(
														  &_mass[i].center.sprite, 
														  _mass[i].center.phys.pos.x, 
														  _mass[i].center.phys.pos.y, 
														  _mass[i].center.phys.pos.z, 
														  camMat);
				}
				
				if (_mass[i].center.props.glows)
				{
					glDepthMask(GL_TRUE);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			
				}
			}
				break;
				
			case eParticleDrawModePoint:
			{
				glEnable(GL_POINT_SPRITE_OES);
				glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
				glEnable(GL_TEXTURE_2D);
				// Get the maximum and minimum sizes that the point sprite can be.
				glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, _point_sizes);
				
				// Set point sizes.
				glPointParameterfv(GL_POINT_SIZE_MIN, &_point_sizes[0]);
				glPointParameterfv(GL_POINT_SIZE_MAX, &_point_sizes[1]);
				
				// Iterate through the particles, setting color and specifying their positions.
				
				if (!_mass[i].center.is_active)
				{
					continue;
				}
				
#if !defined (GL_ATTENUATION_NOT_SUPPORTED)
				float coeffs[] =  { 1.0f, 0.0f, 0.0f };
				if (_mass[i].center.props.is_3D_enabled)
				{
					// We are concerned with the z parameter for 3d mode.
					coeffs[0] = 0.0f;
					coeffs[1] = 0.0f;
					coeffs[2] = 1.0f;
				}
				glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, coeffs );
#endif
				
				if (_mass[i].center.props.glows)
				{
					// This causes colors to be additive when particles overlap each other, creating a "glow" effect.
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					// Turn off depth masking so particles in front will not occlude particles behind them.
					glDepthMask(GL_FALSE);				
				}
				
				// We are using the center mass's sprite texture for all particles in this mass.
				glBindTexture(GL_TEXTURE_2D, _mass[i].center.sprite.getTexName());
				
				// Save the current transform.
				glPushMatrix();
				
				// These will hold the translated particle coordinates.
				float trans_x, trans_y, trans_z;
				GLfloat vertices[3];
				
				for (int j = 0; j < _mass[i].num_particles; ++j)
				{
					if (_mass[i].particles[j].is_active)
					{						
						// The sprite contains all the animation information. This includes color (alpha) and size, so grab that info and apply it here.
						glColor4f( 
								  _mass[i].particles[j].sprite._color.r,
								  _mass[i].particles[j].sprite._color.g,
								  _mass[i].particles[j].sprite._color.b,
								  _mass[i].particles[j].sprite._color.a);
						
						if (_mass[i].center.props.is_3D_enabled)
						{
							// Draw strands if enabled.
							if (_mass[i].particles[j].props.strand_length > 0)
							{
								for (int k = 0; k < _mass[i].particles[j].pos_history_active_count; ++k)
								{
									coordsScreenTo3D(
													 _mass[i].particles[j].pos_history[k].x,
													 _mass[i].particles[j].pos_history[k].y, 
													 _mass[i].particles[j].pos_history[k].z,
													 &trans_x, 
													 &trans_y, 
													 &trans_z);
									vertices[0] = trans_x;
									vertices[1] = trans_y;
									vertices[2] = trans_z;
									
									glVertexPointer(3, GL_FLOAT, 0, vertices);
									glDrawArrays(GL_POINTS, 0, 1);
#if defined (ENABLE_POLY_COUNT)
									updatePolyCount(1);
#endif
								}
							}
							
							//float* camMat = (float*)data;
							// Translate the coordinates to the 3d view coordinates.
							coordsScreenTo3D(
											 _mass[i].particles[j].phys.pos.x, 
											 _mass[i].particles[j].phys.pos.y, 
											 _mass[i].particles[j].phys.pos.z, 
											 &trans_x, 
											 &trans_y, 
											 &trans_z);
							vertices[0] = trans_x;
							vertices[1] = trans_y;
							vertices[2] = trans_z;
							
							float size = _mass[i].center.sprite.getWidth() * _mass[i].particles[j].sprite._scale.x;
							
#if defined (GL_ATTENUATION_NOT_SUPPORTED)
							// JC: TODO: This is broken, fix this.
							float a = 1.0f;
							float b = 0.0f;
							float c = 0.0f;
							float cam_x = camMat[0];
							float cam_y = camMat[1];
							float cam_z = camMat[2];
							float d = sqrt( ((cam_x - _mass[i].particles[j].phys.pos.x) * (cam_x - _mass[i].particles[j].phys.pos.x)) +
										   ((cam_y - _mass[i].particles[j].phys.pos.y) * (cam_y - _mass[i].particles[j].phys.pos.y)) +
										   ((cam_z - _mass[i].particles[j].phys.pos.z) * (cam_z - _mass[i].particles[j].phys.pos.z)) );
							size *= sqrt((float)1 / (a + (b * d) + (c * d * d)));
							// Optimized formula.
							//size *= (1.0f / (d * d));
#endif
							// Set point sprite size.
							glPointSize(size);
							
							glVertexPointer(3, GL_FLOAT, 0, vertices);
							glDrawArrays(GL_POINTS, 0, 1);
						}
						else 
						{
							// Draw strands if enabled.
							if (_mass[i].particles[j].props.strand_length > 0)
							{
								for (int k = 0; k < _mass[i].particles[j].pos_history_active_count; ++k)
								{
									vertices[0] = _mass[i].particles[j].pos_history[k].x;
									vertices[1] = _mass[i].particles[j].pos_history[k].y;
									vertices[2] = _mass[i].particles[j].pos_history[k].z;
									
									glVertexPointer(3, GL_FLOAT, 0, vertices);
									glDrawArrays(GL_POINTS, 0, 1);
#if defined (ENABLE_POLY_COUNT)
									updatePolyCount(1);
#endif
								}
							}
							
							
							vertices[0] = _mass[i].particles[j].phys.pos.x;
							vertices[1] = _mass[i].particles[j].phys.pos.y;
							vertices[2] = _mass[i].particles[j].phys.pos.z;
							// Set point sprite size. We use just the width and x scale here since a point sprite can only be resized in one dimension.
							glPointSize(_mass[i].particles[j].sprite.getWidth() * _mass[i].particles[j].sprite._scale.x);
							
							glVertexPointer(3, GL_FLOAT, 0, vertices);
							glDrawArrays(GL_POINTS, 0, 1);
						}
						
#if defined (ENABLE_POLY_COUNT)
						updatePolyCount(1);
#endif
						
					}
				}
				
				if (_mass[i].center.props.draw_emitter)
				{
					// The sprite contains all the animation information. This includes color (alpha) and size, so grab that info and apply it here.
					glColor4f( 
							  _mass[i].center.sprite._color.r,
							  _mass[i].center.sprite._color.g,
							  _mass[i].center.sprite._color.b,
							  _mass[i].center.sprite._color.a);
					
					// Set point sprite size. We use just the width and x scale here since a point sprite can only be resized in one dimension.
					glPointSize(_mass[i].center.sprite.getWidth() * _mass[i].center.sprite._scale.x);
					
					if (_mass[i].center.props.is_3D_enabled)
					{
						coordsScreenTo3D(
										 _mass[i].center.phys.pos.x, 
										 _mass[i].center.phys.pos.y, 
										 _mass[i].center.phys.pos.z, 
										 &trans_x, 
										 &trans_y, 
										 &trans_z);
						vertices[0] = trans_x;
						vertices[1] = trans_y;
						vertices[2] = trans_z;
						
						// JC: TODO: Calculate size based on z distance from the camera.
					}
					else
					{
						vertices[0] = _mass[i].center.phys.pos.x;
						vertices[1] = _mass[i].center.phys.pos.y;
						vertices[2] = _mass[i].center.phys.pos.z;
					}
					
					glVertexPointer(3, GL_FLOAT, 0, vertices);
					glDrawArrays(GL_POINTS, 0, 1);
#if defined (ENABLE_POLY_COUNT)
					updatePolyCount(1);
#endif
				}
				
				glPopMatrix();
				
				if (_mass[i].center.props.glows)
				{
					glDepthMask(GL_TRUE);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			
				}
				
				glDisable(GL_POINT_SPRITE_OES);
				glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_FALSE);
				glDisable(GL_TEXTURE_2D);
			}
				break;
				
			default:
				break;
		}
	}
}


void CParticleSystem::update()
{
	if (!_is_running)
	{
		return;
	}
	
	if (_mass.length() <= 0)
	{
		return;
	}
	
	int rel_time = 0;
	for (int i = 0; i < _num_masses; ++i)
	{
		if (_mass[i].particles.length() <= 0)
		{
			continue;
		}
		
		if (!_mass[i].center.is_active)
		{
			continue;
		}
		
		_mass[i].rel_counter += TIME_LAST_FRAME;
		rel_time = _mass[i].center.props.release_time;
		if (_mass[i].center.props.release_rand > 0)
		{
			rel_time += (rand() % _mass[i].center.props.release_rand);
		}
		
		// When the release time counter reaches the relase time, we will set the next available particle to be active.
		if (_mass[i].rel_counter >= rel_time)
		{
			_mass[i].rel_counter = 0;
			releaseNextParticle(i);
		}
		
		// JC: Put this back in later if needed.
		_mass[i].center.sprite.updateAction();
		CPhysics::updatePhysics(&_mass[i].center.phys, _mass[i].center.props.frame_skip);
		for (int j = 0; j < _mass[i].num_particles; ++j)
		{
			// Check for death of the particle.
			if (_mass[i].particles[j].props.life_time != __INF)
			{
				_mass[i].particles[j].props.life_time -= TIME_LAST_FRAME;
				if (_mass[i].particles[j].props.life_time <= 0)
				{
					// Reset the life time of the particle and set it to inactive.
					killParticle(i, j);
				}
			}
			else
			{
				// We will rely on other factors to determine the death of a particle if the life time is set to __INF.
				
				//// Check the size scale to determine particle death if the life time is set to __INF.
				//if ((_mass[i].particles[j].sprite._scale.x <= 0) && 
				//	(_mass[i].particles[j].sprite._scale.y <= 0) &&
				//	(_mass[i].particles[j].sprite._scale.z <= 0))
				//{
				//	killParticle(i, j);
				//}
				
				// Check the alpha value to determine particle death. We will also never kill the particle if it's lifetime is infinite.
				//if (_mass[i].particles[j].sprite._color.a <= 0.0)
				if ((_mass[i].particles[j].sprite.getCurrentNumColorPulses() <= 0) && (_mass[i].particles[j].props.fade_speed != __INF))
				{
					killParticle(i, j);
				}
			}
			
			if (_mass[i].particles[j].is_active)
			{
				// If the strand value is greater than zero, then we are rendering strands, and we must save off the last position before updating.
				if (_mass[i].particles[j].props.strand_length > 0)
				{
					_mass[i].particles[j].pos_history[_mass[i].particles[j].pos_history_counter].set(_mass[i].particles[j].phys.pos);
					_mass[i].particles[j].pos_history_counter++;
					_mass[i].particles[j].pos_history_active_count++;
					
					// Wrap around back to the start of the counter once we reach the strand length.
					// NOTE: If this happens, then it indicates that the strand length should be increased.
					if (_mass[i].particles[j].pos_history_counter >= _mass[i].particles[j].props.strand_length)
					{
						_mass[i].particles[j].pos_history_counter = 0;
						
					}
					
					if (_mass[i].particles[j].pos_history_active_count >= _mass[i].particles[j].props.strand_length)
					{
						_mass[i].particles[j].pos_history_active_count = _mass[i].particles[j].props.strand_length;
					}
				}
				
				_mass[i].particles[j].sprite.updateAction();
				CPhysics::updatePhysics(&_mass[i].particles[j].phys, _mass[i].particles[j].props.frame_skip);
			}
		}
	}
}


void CParticleSystem::releaseNextParticle(int massID)
{
	int rate_counter = 0;
	Vector3 temp_vec = Vector3(0.0f, 0.0f, 360.0f);
	int rand_flag = 0;
	eSpriteRotateDir rot_dir;
	int dist_from_center = 0;
	
	// Do not release any more particles if we have reached the loop count limit. Ignore if the loop count is set to __INF
	if ((_mass[massID].loop_counter >= _mass[massID].loop_count) && (_mass[massID].loop_count != __INF))
	{
		return;
	}
	
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		if (!_mass[massID].particles[i].is_active)
		{			
			rate_counter++;
			// Check wheter we have reached the release limit for this mass.
			if (rate_counter > _mass[massID].center.props.release_rate)
			{
				return;
			}
			
			if (_mass[massID].loop_count != __INF)
			{
				_mass[massID].rel_particle_counter++;
				if (_mass[massID].rel_particle_counter >= _mass[massID].num_particles)
				{
					// Once we reach the last particle in the mass, we will increase the loop counter.
					_mass[massID].loop_counter++;
					_mass[massID].rel_particle_counter = 0;
				}
			}
			
			_mass[massID].particles[i].props.life_time = _mass[massID].center.props.life_time;
			// Center the particle on the center of the mass.
			_mass[massID].particles[i].phys.pos = _mass[massID].center.phys.pos;
			_mass[massID].particles[i].is_active = TRUE;
			
			// Check for color randomization mode.
			if (_mass[massID].center.props.color_rand)
			{
				// Set a random color.
				_mass[massID].particles[i].sprite.setColor(
														   (float)(rand() % 100) / 100,
														   (float)(rand() % 100) / 100,
														   (float)(rand() % 100) / 100);
			}
			else
			{
				// Set the particle color to match the center color.
				_mass[massID].particles[i].sprite.setColor(_mass[massID].center.sprite.getColor());
			}
			
			// Ensure that a newly ejected particle is fully visible.
			_mass[massID].particles[i].sprite._color.a = 1.0;
			
			if ((_mass[massID].center.props.fade_speed > 0) && (_mass[massID].center.props.fade_speed != __INF))
			{
				// This will run any fade of the particle.
				_mass[massID].particles[i].sprite.setColorPulseAction(
																		_mass[massID].particles[i].sprite._color.r,
																		_mass[massID].particles[i].sprite._color.g,
																		_mass[massID].particles[i].sprite._color.b,
																		0,
																		_mass[massID].center.props.fade_speed,
																		_mass[massID].center.props.fade_count);
			}

			if ((_mass[massID].center.props.rotation_speed > 0) && (_mass[massID].center.props.rotation_speed != __INF))
			{
				// JC: For now, just set continuous rotation.
				_mass[massID].particles[i].sprite._angle.zero();
				
				// Randomize the rotation direction.
				rand_flag = rand() % 2;
				if (rand_flag == 0)
				{
					rot_dir = eSpriteRotClock;
				}
				else
				{
					rot_dir = eSpriteRotCounterClock;
				}
				
				// Factor in any randomness in rotation speed.
				if (_mass[massID].center.props.rotation_rand > 0)
				{
					_mass[massID].particles[i].sprite.setRotateAction(
																		temp_vec,
																		rot_dir, 
																		_mass[massID].center.props.rotation_speed + (rand() % _mass[massID].center.props.rotation_rand),
																		__INF);
				}
				else
				{
					_mass[massID].particles[i].sprite.setRotateAction(temp_vec, rot_dir, _mass[massID].center.props.rotation_speed, __INF);
				}
			}
			
			// Reset scale.
			_mass[massID].particles[i].sprite._scale = Vector3(
																 _mass[massID].center.props.size_start, 
																 _mass[massID].center.props.size_start, 
																 _mass[massID].center.props.size_start);
			// Check for whether we need to run scaling actions at all.
			if ((_mass[massID].center.props.size_speed > 0) && (_mass[massID].center.props.size_speed != __INF))
			{
				// Run scaling actions.
				_mass[massID].particles[i].sprite.setSizeScaleAction(
																	   _mass[massID].center.props.size_end, 
																	   _mass[massID].center.props.size_end, 
																	   _mass[massID].center.props.size_end,
																	   _mass[massID].center.props.size_speed,
																	   _mass[massID].center.props.size_count);
			}
			
			// Check for whether we need to apply a blink action.
			if ((_mass[massID].center.props.blink_count > 0) && (_mass[massID].center.props.blink_count != __INF))
			{
				_mass[massID].particles[i].sprite.setBlinkAction(
																   _mass[massID].center.props.blink_on_time,
																   _mass[massID].center.props.blink_on_rand,
																   _mass[massID].center.props.blink_off_time,
																   _mass[massID].center.props.blink_off_rand,
																   _mass[massID].center.props.blink_count);
			}
			
			// Check for whether the particle has some starting distance from the center.
			dist_from_center = _mass[massID].center.props.release_dist;
			if (_mass[massID].center.props.release_dist_rand > 0)
			{
				dist_from_center += (rand() % _mass[massID].center.props.release_dist_rand);
			}
			// Only set release distance if it is some value larger than zero.
			if (dist_from_center > 0)
			{
				int angle_rand = rand() % 360;
				_mass[massID].particles[i].phys.pos.x += pixel(dist_from_center * cos(DEGREES_TO_RADIANS(angle_rand)));
				_mass[massID].particles[i].phys.pos.y += pixel(dist_from_center * sin(DEGREES_TO_RADIANS(angle_rand)));
			}
			dist_from_center = 0;
			
			// Set strand length if any.
			if (_mass[massID].center.props.strand_length > 0)
			{
				_mass[massID].particles[i].props.strand_length = _mass[massID].center.props.strand_length;
				// Clear out some values.
				_mass[massID].particles[i].pos_history_counter = 0;
				_mass[massID].particles[i].pos_history_active_count = 0;
			}
			
			// Set frame skip, to be inherited by mass center.
			_mass[massID].particles[i].props.frame_skip = _mass[massID].center.props.frame_skip;
			
			applyProperties(massID, i);																	
		}
	}
}


void CParticleSystem::killParticle(int massID, int particleID)
{
	_mass[massID].particles[particleID].is_active = FALSE;
}	


void CParticleSystem::recenterMass(int massID, int x, int y)
{
	if (massID >= _num_masses)
	{
		DPRINT_PARTICLESYS("CParticleSystem::setMassPosition failed: massID out of bounds");
		return;
	}
	
	_mass[massID].center.phys.pos.x = x;
	_mass[massID].center.phys.pos.y = y;
	
	// Save off this position as the initial position so if the mass is reset, it will be placed at this location again.
	_mass[massID].initial_pos.x = x;
	_mass[massID].initial_pos.y = y;
	
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		_mass[massID].particles[i].phys.pos = _mass[massID].center.phys.pos;
	}
}


void CParticleSystem::recenterMass(int massID, int x, int y, int z)
{
	if (massID >= _num_masses)
	{
		DPRINT_PARTICLESYS("CParticleSystem::setMassPosition failed: massID out of bounds");
		return;
	}
	
	_mass[massID].center.phys.pos.x = x;
	_mass[massID].center.phys.pos.y = y;
	_mass[massID].center.phys.pos.z = z;
	
	// Save off this position as the initial position so if the mass is reset, it will be placed at this location again.
	_mass[massID].initial_pos.x = x;
	_mass[massID].initial_pos.y = y;
	_mass[massID].initial_pos.z = z;
	
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		_mass[massID].particles[i].phys.pos = _mass[massID].center.phys.pos;
	}
}


void CParticleSystem::recenterParticles(int massID)
{
	if (massID >= _num_masses)
	{
		DPRINT_PARTICLESYS("CParticleSystem::recenterParticles failed: massID out of bounds");
		return;
	}
	
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		_mass[massID].particles[i].phys.pos = _mass[massID].center.phys.pos;
	}
}


void CParticleSystem::applyProperties(int massID, int particleID)
{	
	// Set physics properties. We can safely set all properties here including velocities since physic won't be 
	// updated on the particle until it becomes active.
	int angle = 0;
	int vel = 0;
	particleProperties *props = &_mass[massID].center.props;
	
	// Apply the properties to the particle.
	CPhysics::resetVelocity(&_mass[massID].particles[particleID].phys);
	
	if (_mass[massID].center.props.is_3D_enabled)
	{
		int phi = 0;
		int theta = 0;
		
		// Set up the random value of the angle using the min and max angle values as bounds.
		if ((props->angle_max - props->angle_min) <= 0)
		{
			// Don't allow for negative angles. Also we must make sure that when the angle is zero, we set it to angle_min, otherwise we will get a divide by 0 error.
			phi = props->angle_min;
			theta = props->angle_min;
		}
		else
		{
			phi = (rand() % (props->angle_max - props->angle_min)) + props->angle_min;
			theta = (rand() % (props->angle_max - props->angle_min)) + props->angle_min;
		}
		
		if (props->vel_rand > 0)
		{	
			vel = (rand() % props->vel_rand) + props->vel_base;
		}
		else
		{
			vel = props->vel_base;
		}
		
		CPhysics::applyImpulse(
							   &_mass[massID].particles[particleID].phys, 
							   pixel(vel * sin(DEGREES_TO_RADIANS(phi)) * cos(DEGREES_TO_RADIANS(theta))),
							   pixel(vel * sin(DEGREES_TO_RADIANS(phi)) * sin(DEGREES_TO_RADIANS(theta))),
							   pixel(vel * cos(DEGREES_TO_RADIANS(phi))));
	}
	else
	{
		// Set up the random value of the angle using the min and max angle values as bounds.
		if ((props->angle_max - props->angle_min) <= 0)
		{
			// Don't allow for negative angles. Also we must make sure that when the angle is zero, we set it to angle_min, otherwise we will get a divide by 0 error.
			angle = props->angle_min;
		}
		else
		{
			angle = (rand() % (props->angle_max - props->angle_min)) + props->angle_min;
		}
		
		if (props->vel_rand > 0)
		{	
			vel = (rand() % props->vel_rand) + props->vel_base;
		}
		else
		{
			vel = props->vel_base;
		}
		
		CPhysics::applyImpulse(
							   &_mass[massID].particles[particleID].phys, 
							   pixel(vel * cos(DEGREES_TO_RADIANS(angle))),
							   pixel(vel * sin(DEGREES_TO_RADIANS(angle))));
	}
	
	_mass[massID].particles[particleID].phys.acc.y = pixel(props->gravity);
	_mass[massID].particles[particleID].props.life_time = props->life_time;
	
	_mass[massID].particles[particleID].phys.resistance = props->slowdown_rate;
}


void CParticleSystem::reset(const int massID)
{
	//particleMass massTmp;
	particleProperties props;
		
	// Save off temp copy of mass.
	//memcpy(&massTmp, &_mass[massID], sizeof(particleMass));
	
	// Save off temp copy of properties.
	memcpy(&props, &_mass[massID].center.props, sizeof(particleProperties));
	
	recenterMass(massID, _mass[massID].initial_pos.x, _mass[massID].initial_pos.y, _mass[massID].initial_pos.z);
	recenterParticles(massID);
	setMassActive(massID, TRUE);
	
	setMode(massID, props);	
}


void CParticleSystem::setMode(int massID, const particleProperties &modeData, const char* imageName)
{
	memcpy(&_mass[massID].center.props, &modeData, sizeof(particleProperties));

	CImage* particle_image = GET_IMGLOADER->getImage(modeData.image_id);
	
	_mass[massID].center.sprite.destroy();
	_mass[massID].center.sprite.init();
	_mass[massID].center.sprite.loadSpriteImage(particle_image);
	_mass[massID].center.sprite.setColor(modeData.r, modeData.g, modeData.b);
	
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		killParticle(massID, i);
		
		// Copy the properties to each individual particle in case we need to grab it for any reason.
		memcpy(&_mass[massID].particles[i].props, &modeData, sizeof(particleProperties));
		
		applyProperties(massID, i);
		
		_mass[massID].particles[i].sprite.destroy();
		_mass[massID].particles[i].sprite.init();
		_mass[massID].particles[i].sprite.loadSpriteImage(particle_image);
		
		// Set up position history array.
		if (modeData.strand_length > 0)
		{
			_mass[massID].particles[i].pos_history = ArrayList<Vector3>::alloc(modeData.strand_length);
		}
	}
	
	// Save the image name.
	_mass[massID].image_name = (char*)imageName;
}


void CParticleSystem::setAngles(int massID, int numAngles, ...)
{
	va_list ap;
	memset(&ap, 0, sizeof(va_list));
	
	va_start(ap, numAngles);
	
	int angle = 0;
	int vel = 0;
	particleProperties *props = (particleProperties*)&_mass[massID].center.props;
	for (int i = 0; i < numAngles; ++i)
	{
		angle = va_arg(ap, int);
		if (props->vel_rand > 0)
		{	
			vel = (rand() % props->vel_rand) + props->vel_base;
		}
		else
		{
			vel = props->vel_base;
		}
		
		// Apply the properties to the particle.
		CPhysics::resetVelocity(&_mass[massID].particles[i].phys);
		CPhysics::applyImpulse(
							   &_mass[massID].particles[i].phys, 
							   vel * cos(DEGREES_TO_RADIANS(angle)),
							   vel * sin(DEGREES_TO_RADIANS(angle)));
	}
	
	va_end(ap);
}


void CParticleSystem::applyImpulse(int massID, int xImp, int yImp)
{
	CPhysics::applyImpulse(&_mass[massID].center.phys, xImp, yImp);
}


void CParticleSystem::applyImpulse(int massID, int xImp, int yImp, int zImp)
{
	CPhysics::applyImpulse(&_mass[massID].center.phys, xImp, yImp, zImp);
}


void CParticleSystem::setImageID(const int massID, const int imageID)
{	
	CImage* particle_image = GET_IMGLOADER->getImage(imageID);
	_mass[massID].center.sprite.destroy();
	_mass[massID].center.sprite.init();
	_mass[massID].center.sprite.loadSpriteImage(particle_image);
	
	// Set image colors.
	_mass[massID].center.sprite.setColor(_mass[massID].center.props.r, _mass[massID].center.props.g, _mass[massID].center.props.b);
	
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		_mass[massID].particles[i].sprite.destroy();
		_mass[massID].particles[i].sprite.init();
		_mass[massID].particles[i].sprite.loadSpriteImage(particle_image);
		
		// Set image colors.
		_mass[massID].particles[i].sprite.setColor(_mass[massID].center.props.r, _mass[massID].center.props.g, _mass[massID].center.props.b);
	}	
	
	_mass[massID].center.props.image_id = imageID;
	
	
}


void CParticleSystem::setMassSize(const int massID, const int massSize)
{	
	int numParticles = massSize;
	// Check for invalid input.
	if (numParticles < 0)
	{
		numParticles = 0;
	}
	
	// Save off current mass properties. This will need to be re-set after we delete the old mass and reinitialize the new one.
	particleProperties props;
	memcpy(&props, &_mass[massID].center.props, sizeof(particleProperties));
	
	// Also save off current position.
	float x = _mass[massID].center.phys.pos.x;
	float y = _mass[massID].center.phys.pos.y;
	float z = _mass[massID].center.phys.pos.z;
	
	BOOL is_active = _mass[massID].center.is_active;
	BOOL draw_center = _mass[massID].center.props.draw_emitter; 
	
	// Destroy any previous exising mass.
	for (int j = 0; j < _mass[massID].num_particles; ++j)
	{
		_mass[massID].particles[j].sprite.destroy();
	}
	_mass[massID].center.sprite.destroy();
	_mass[massID].particles = NULL;
	
	// Re-initialize mass.
	initMass(massID, numParticles);
	
	// Re-set properties.
	setMode(massID, props);
	
	// Re-set positions.
	_mass[massID].center.phys.pos.x = x;
	_mass[massID].center.phys.pos.y = y;
	_mass[massID].center.phys.pos.z = z;
	
	_mass[massID].center.is_active = is_active;
	_mass[massID].center.props.draw_emitter = draw_center;
}


void CParticleSystem::setStrandLength(const int massID, const int length)
{	
	_mass[massID].center.props.strand_length = length;

	if (_mass[massID].center.props.strand_length < 0)
	{
		_mass[massID].center.props.strand_length = 0;
	}
	
	// Clear previous history and reinit.
	for (int i = 0; i < _mass[massID].num_particles; ++i)
	{
		_mass[massID].particles[i].pos_history = NULL;
		_mass[massID].particles[i].pos_history_counter = 0;
		_mass[massID].particles[i].pos_history_active_count = 0;
		_mass[massID].particles[i].pos_history = ArrayList<Vector3>::alloc(length);
		_mass[massID].particles[i].props.strand_length = length;
	}
}


void CParticleSystem::setPhysicsState(int massID, ePhysicsMovementState state)
{
	for (int j = 0; j < _mass[massID].num_particles; ++j)
	{
		_mass[massID].particles[j].phys.movement_state = state;
	}
}


void CParticleSystem::setLifeTime(const int massID, const int lifeTime)
{
	_mass[massID].center.props.life_time = lifeTime; 
			
	if (_mass[massID].center.props.life_time < 0)
	{
		_mass[massID].center.props.life_time = __INF;
	}
}


void CParticleSystem::setFadeSpeed(const int massID, const int fadeSpeed) 
{ 
	_mass[massID].center.props.fade_speed = fadeSpeed;
	
	if (_mass[massID].center.props.fade_speed < 0)
	{
		_mass[massID].center.props.fade_speed = __INF;
	}
}


void CParticleSystem::setFadeCount(const int massID, const int fadeCount) 
{
	_mass[massID].center.props.fade_count = fadeCount;
	
	if (_mass[massID].center.props.fade_count < 0)
	{
		_mass[massID].center.props.fade_count = 0;
	}
}


void CParticleSystem::setSlowdownRate(const int massID, const float slowdownRate)
{
	_mass[massID].center.props.slowdown_rate = slowdownRate; 
	
	if (_mass[massID].center.props.slowdown_rate < 0.0f)
	{
		_mass[massID].center.props.slowdown_rate = 100.0f;
	}
	else if (_mass[massID].center.props.slowdown_rate >= 100.0f)
	{
		_mass[massID].center.props.slowdown_rate = 0.0f;
	}
}


void CParticleSystem::setVelBase(const int massID, const int velBase) 
{
	_mass[massID].center.props.vel_base = velBase;
	
	if (_mass[massID].center.props.vel_base < 0)
	{
		_mass[massID].center.props.vel_base = 0;
	}
}


void CParticleSystem::setVelRand(const int massID, const int velRand) 
{ 
	_mass[massID].center.props.vel_rand = velRand; 
	
	if (_mass[massID].center.props.vel_rand < 0)
	{
		_mass[massID].center.props.vel_rand = 0;
	}
}


void CParticleSystem::setAngleMin(const int massID, const int angleMin) 
{
	_mass[massID].center.props.angle_min = angleMin; 
	
	if (_mass[massID].center.props.angle_min < 0)
	{
		_mass[massID].center.props.angle_min = 360;
	}
	else if (_mass[massID].center.props.angle_min >= 360)
	{
		_mass[massID].center.props.angle_min = 0;
	}
	
	// Make sure that the min angle is never greater than the max angle.
	if (_mass[massID].center.props.angle_min > _mass[massID].center.props.angle_max)
	{
		// Just adjust the max angle to one degree greater than angle min.
		_mass[massID].center.props.angle_max = _mass[massID].center.props.angle_min + 1;
	}
}


void CParticleSystem::setAngleMax(const int massID, const int angleMax) 
{
	_mass[massID].center.props.angle_max = angleMax; 
	
	if (_mass[massID].center.props.angle_max < 0)
	{
		_mass[massID].center.props.angle_max = 360;
	}
	else if (_mass[massID].center.props.angle_max >= 360)
	{
		_mass[massID].center.props.angle_max = 0;
	}
	
	// Make sure that the min angle is never greater than the max angle.
	if (_mass[massID].center.props.angle_min > _mass[massID].center.props.angle_max)
	{
		// Just adjust the max angle to one degree greater than angle min.
		_mass[massID].center.props.angle_max = _mass[massID].center.props.angle_min + 1;
	}
}


void CParticleSystem::setReleaseTime(const int massID, const int releaseTime) 
{
	_mass[massID].center.props.release_time = releaseTime;
	
	if (_mass[massID].center.props.release_time < 0)
	{
		_mass[massID].center.props.release_time = 0;
	}
}


void CParticleSystem::setReleaseRate(const int massID, const int releaseRate) 
{
	_mass[massID].center.props.release_rate = releaseRate; 
	
	if (_mass[massID].center.props.release_rate < 0)
	{
		_mass[massID].center.props.release_rate = 0;
	}
}


void CParticleSystem::setReleaseRand(const int massID, const int releaseRand)
{
	_mass[massID].center.props.release_rand = releaseRand;
	
	if (_mass[massID].center.props.release_rand < 0)
	{
		_mass[massID].center.props.release_rand = 0;
	}
}


void CParticleSystem::setGravity(const int massID, const float gravity)
{
	_mass[massID].center.props.gravity = gravity; 
}


void CParticleSystem::setRotationSpeed(const int massID, const int rotationSpeed)
{
	_mass[massID].center.props.rotation_speed = rotationSpeed; 
	
	if (_mass[massID].center.props.rotation_speed < 0)
	{
		_mass[massID].center.props.rotation_speed = 0;
	}
}


void CParticleSystem::setRotationRand(const int massID, const int rotationRand) 
{ 
	_mass[massID].center.props.rotation_rand = rotationRand; 
	
	if (_mass[massID].center.props.rotation_rand < 0)
	{
		_mass[massID].center.props.rotation_rand = 0;
	}
}


void CParticleSystem::setSizeStart(const int massID, const float sizeStart) 
{
	_mass[massID].center.props.size_start = sizeStart; 
	
	if (_mass[massID].center.props.size_start < 0)
	{
		_mass[massID].center.props.size_start = 0;
	}
}


void CParticleSystem::setSizeEnd(const int massID, const float sizeEnd)
{
	_mass[massID].center.props.size_end = sizeEnd; 
	
	if (_mass[massID].center.props.size_end < 0)
	{
		_mass[massID].center.props.size_end = 0;
	}
}


void CParticleSystem::setSizeSpeed(const int massID, const float sizeSpeed)
{
	_mass[massID].center.props.size_speed = sizeSpeed; 
	
	if (_mass[massID].center.props.size_speed < 0)
	{
		_mass[massID].center.props.size_speed = 0;
	}
}


void CParticleSystem::setSizeCount(const int massID, const int sizeCount)
{
	_mass[massID].center.props.size_count = sizeCount; 
	
	if (_mass[massID].center.props.size_count < 0)
	{
		_mass[massID].center.props.size_count = 0;
	}
}


void CParticleSystem::setBlinkOnTime(const int massID, const int blinkOnTime) 
{
	_mass[massID].center.props.blink_on_time = blinkOnTime; 
		
	if (_mass[massID].center.props.blink_on_time < 0)
	{
		_mass[massID].center.props.blink_on_time = __INF;
	}
}


void CParticleSystem::setBlinkOnRand(const int massID, const int blinkOnRand) 
{
	_mass[massID].center.props.blink_on_rand = blinkOnRand; 
	
	if (_mass[massID].center.props.blink_on_rand < 0)
	{
		_mass[massID].center.props.blink_on_rand = 0;
	}
}


void CParticleSystem::setBlinkOffTime(const int massID, const int blinkOffTime)
{
	_mass[massID].center.props.blink_off_time = blinkOffTime; 

	if (_mass[massID].center.props.blink_off_time < 0)
	{
		_mass[massID].center.props.blink_off_time = __INF;
	}
}


void CParticleSystem::setBlinkOffRand(const int massID, const int blinkOffRand) 
{
	_mass[massID].center.props.blink_off_rand = blinkOffRand; 
	
	if (_mass[massID].center.props.blink_off_rand < 0)
	{
		_mass[massID].center.props.blink_off_rand = 0;
	}
}


void CParticleSystem::setBlinkCount(const int massID, const int blinkCount)
{
	_mass[massID].center.props.blink_count = blinkCount; 
	
	if (_mass[massID].center.props.blink_count < 0)
	{
		_mass[massID].center.props.blink_count = __INF;
	}
}


void CParticleSystem::setReleaseDist(const int massID, const int releaseDist)
{
	_mass[massID].center.props.release_dist = releaseDist; 
	
	if (_mass[massID].center.props.release_dist < 0)
	{
		_mass[massID].center.props.release_dist = 0;
	}
}


void CParticleSystem::setReleaseDistRand(const int massID, const int releaseDistRand)
{ 
	_mass[massID].center.props.release_dist_rand = releaseDistRand; 
	
	if (_mass[massID].center.props.release_dist_rand < 0)
	{
		_mass[massID].center.props.release_dist_rand = 0;
	}
}


void CParticleSystem::setColor(const int massID, float r, float g, float b)
{
	// Adjust the red color component of the particle mass.
	_mass[massID].center.props.r = r;
	
	if (_mass[massID].center.props.r < 0.0f)
	{
		_mass[massID].center.props.r = 0.0f;
	}
	else if (_mass[massID].center.props.r > 1.0f)
	{
		_mass[massID].center.props.r = 1.0f;
	}
	
	// Adjust the green color component of the particle mass.
	_mass[massID].center.props.g = g;
	
	if (_mass[massID].center.props.g < 0.0f)
	{
		_mass[massID].center.props.g = 0.0f;
	}
	else if (_mass[massID].center.props.g > 1.0f)
	{
		_mass[massID].center.props.g = 1.0f;
	}
	
	// Adjust the blue color component of the particle mass.
	_mass[massID].center.props.b = b;
	
	if (_mass[massID].center.props.b < 0.0f)
	{
		_mass[massID].center.props.b = 0.0f;
	}
	else if (_mass[massID].center.props.b > 1.0f)
	{
		_mass[massID].center.props.b = 1.0f;
	}
	
	// Set the color of the sprite.
	_mass[massID].center.sprite.setColor(r, g, b);
}


void CParticleSystem::setPropertyValue(const int massID, const int propID, const propVal value)
{	
	switch (propID)
	{
		case PROP_LIFE:
			// Adjust life time of the particles.
			setLifeTime(massID, value.intVal);
			break;
			
		case PROP_FADE_SPEED:
			// Adjust fade speed of the particles.
			setFadeSpeed(massID, value.intVal);
			break;
			
		case PROP_FADE_COUNT:
			// Adjust fade loop count of the particles.
			setFadeCount(massID, value.intVal);
			break;
			
		case PROP_SLOWDOWN_RATE:
			// Adjust speed deterioration of the particles.
			setSlowdownRate(massID, value.floatVal);
			break;
			
		case PROP_VEL_BASE:
			// Adjust base velocity of the particles.
			setVelBase(massID, value.intVal);
			break;
			
		case PROP_VEL_RAND:
			// Adjust velocity randomness of the particles.
			setVelRand(massID, value.intVal);
			break;
			
		case PROP_ANGLE_MIN:
			// Adjust ejection angle of the particles.
			setAngleMin(massID, value.intVal);
			break;
			
		case PROP_ANGLE_MAX:
			// Adjust ejection angle of the particles.
			setAngleMax(massID, value.intVal);
			break;
			
		case PROP_REL_TIME:
			// Adjust the speed in MS at which particles are released.
			setReleaseTime(massID, value.intVal);
			break;
			
		case PROP_REL_RATE:
			// Adjust the number of particles that are released at each interval.
			setReleaseRate(massID, value.intVal);
			break;
			
		case PROP_REL_RAND:
			// Adjust release rate randomness.
			setReleaseRand(massID, value.intVal);
			break;
			
		case PROP_GRAVITY:
			// Adjust gravity on the particles.
			setGravity(massID, value.floatVal);
			break;
			
		case PROP_ROT_SPEED:
			// Adjust rotation speed of the particles.
			setRotationSpeed(massID, value.intVal);
			break;
			
		case PROP_ROT_RAND:
			// Adjust rotation randomness of the particles.
			setRotationRand(massID, value.intVal);
			break;
			
		case PROP_SIZE_START:
			// Adjust the starting size of the particles as they are ejected.
			setSizeStart(massID, value.floatVal);
			break;
			
		case PROP_SIZE_END:
			// Adjust the ending size of the particles.
			setSizeEnd(massID, value.floatVal);
			break;
			
		case PROP_SIZE_SPEED:
			// Adjust the speed at which the particle size changes.
			setSizeSpeed(massID, value.intVal);
			break;
			
		case PROP_SIZE_COUNT:
			// Adjust the number of times that the size changes from start to end within the size_speed time.
			setSizeCount(massID, value.intVal);
			break;
			
		case PROP_BLINK_ON_TIME:
			// Adjust the time that the particle is visible during a blink action.
			setBlinkOnTime(massID, value.intVal);
			break;
			
		case PROP_BLINK_ON_RAND:
			// Adjust the random time value for the blink-on time.
			setBlinkOnRand(massID, value.intVal);
			break;
			
		case PROP_BLINK_OFF_TIME:
			// Adjust the time that the particle is not visible during a blink action.
			setBlinkOffTime(massID, value.intVal);
			break;
			
		case PROP_BLINK_OFF_RAND:
			// Adjust the random time value for the blink-off time.
			setBlinkOffRand(massID, value.intVal);
			break;
			
		case PROP_BLINK_COUNT:
			// Adjust the number of times that the particle blinks in and out of visibility.			
			setBlinkCount(massID, value.intVal);
			break;
			
		case PROP_REL_DIST:
			// Adjust the release distance from the center mass.
			setReleaseDist(massID, value.intVal);
			break;
			
		case PROP_REL_DIST_RAND:
			// Adjust the release distance randomness from the center mass.
			setReleaseDistRand(massID, value.intVal);
			break;
			
		case PROP_GLOWS:
			// Changes the gl alpha function. Makes particle rendering additive or not.
			setGlows(massID, value.boolVal);
			break;
			
		case PROP_COLOR_R:
			// Adjust the red color component of the particle mass.
			setColor(massID, value.floatVal, _mass[massID].center.props.g, _mass[massID].center.props.b);
			break;
			
		case PROP_COLOR_G:
			// Adjust the green color component of the particle mass.
			setColor(massID, _mass[massID].center.props.r, value.floatVal, _mass[massID].center.props.b);
			break;
			
		case PROP_COLOR_B:
			// Adjust the blue color component of the particle mass.
			setColor(massID, _mass[massID].center.props.r, _mass[massID].center.props.g, value.floatVal);
			break;
			
		case PROP_COLOR_RAND:
			// Toggle color randomization mode.
			setRandColor(massID, value.boolVal);
			break;
			
		case PROP_IMAGE_ID:
			// Changes the particle image.
			setImageID(massID, value.intVal);
			break;
			
		case PROP_MASS_SIZE:
			setMassSize(massID, value.intVal);
			break;
			
		case PROP_STRAND_LENGTH:		
			setStrandLength(massID, value.intVal);
			break;
			
		case PROP_3D_MODE:
			setEnable3D(massID, value.boolVal);
			break;
			
		case PROP_DRAW_MODE:
			setDrawMode(massID, (eParticleDrawMode) value.intVal);
			break;
			
		case PROP_DRAW_EMITTER:
			//setDrawEmitter(massID, value.boolVal);
			setDrawCenter(massID, value.boolVal);
			break;			
	}	
}


const propVal CParticleSystem::getPropertyValue(const int massID, const int propID)
{
	propVal retVal;
	
	switch (propID)
	{
		case PROP_LIFE:
			retVal.intVal = _mass[massID].center.props.life_time;
			break;
			
		case PROP_FADE_SPEED:
			retVal.intVal = _mass[massID].center.props.fade_speed;
			break;
			
		case PROP_FADE_COUNT:
			retVal.intVal = _mass[massID].center.props.fade_count;
			break;
			
		case PROP_SLOWDOWN_RATE:
			retVal.floatVal = _mass[massID].center.props.slowdown_rate;
			break;
			
		case PROP_VEL_BASE:
			retVal.intVal = _mass[massID].center.props.vel_base;
			break;
			
		case PROP_VEL_RAND:
			retVal.intVal = _mass[massID].center.props.vel_rand;
			break;
			
		case PROP_ANGLE_MIN:
			retVal.intVal = _mass[massID].center.props.angle_min;
			break;
			
		case PROP_ANGLE_MAX:
			retVal.intVal = _mass[massID].center.props.angle_max;
			break;
			
		case PROP_REL_TIME:
			retVal.intVal = _mass[massID].center.props.release_time;
			break;
			
		case PROP_REL_RATE:
			retVal.intVal = _mass[massID].center.props.release_rate;
			break;
			
		case PROP_REL_RAND:
			retVal.intVal = _mass[massID].center.props.release_rand;
			break;
			
		case PROP_GRAVITY:
			retVal.floatVal = _mass[massID].center.props.gravity;
			break;
			
		case PROP_ROT_SPEED:
			retVal.intVal = _mass[massID].center.props.rotation_speed;
			break;
			
		case PROP_ROT_RAND:
			retVal.intVal = _mass[massID].center.props.rotation_rand;
			break;
			
		case PROP_SIZE_START:
			retVal.floatVal = _mass[massID].center.props.size_start;
			break;
			
		case PROP_SIZE_END:
			retVal.floatVal = _mass[massID].center.props.size_end;
			break;
			
		case PROP_SIZE_SPEED:
			retVal.intVal = _mass[massID].center.props.size_speed;
			break;
			
		case PROP_SIZE_COUNT:
			retVal.intVal = _mass[massID].center.props.size_count;
			break;
			
		case PROP_BLINK_ON_TIME:
			retVal.intVal = _mass[massID].center.props.blink_on_time;
			break;
			
		case PROP_BLINK_ON_RAND:
			retVal.intVal = _mass[massID].center.props.blink_on_rand;
			break;
			
		case PROP_BLINK_OFF_TIME:
			retVal.intVal = _mass[massID].center.props.blink_off_time;
			break;
			
		case PROP_BLINK_OFF_RAND:
			retVal.intVal = _mass[massID].center.props.blink_off_rand;
			break;
			
		case PROP_BLINK_COUNT:
			retVal.intVal = _mass[massID].center.props.blink_count;
			break;
			
		case PROP_REL_DIST:
			retVal.intVal = _mass[massID].center.props.release_dist;
			break;
			
		case PROP_REL_DIST_RAND:
			retVal.intVal = _mass[massID].center.props.release_dist_rand;
			break;
			
		case PROP_GLOWS:
			retVal.boolVal = _mass[massID].center.props.glows;
			break;
			
		case PROP_COLOR_R:
			retVal.floatVal = _mass[massID].center.props.r;
			break;
			
		case PROP_COLOR_G:
			retVal.floatVal = _mass[massID].center.props.g;
			break;
			
		case PROP_COLOR_B:
			retVal.floatVal = _mass[massID].center.props.b;
			break;
			
		case PROP_COLOR_RAND:
			retVal.boolVal = _mass[massID].center.props.color_rand;
			break;
			
		case PROP_IMAGE_ID:
			retVal.strVal = (char*)getImageName(massID);
			retVal.intVal = _mass[massID].center.props.image_id;
			break;
			
		case PROP_MASS_SIZE:
			retVal.intVal = getNumParticles(massID);
			break;
			
		case PROP_STRAND_LENGTH:
			retVal.intVal = _mass[massID].center.props.strand_length;
			break;
			
		case PROP_3D_MODE:
			retVal.boolVal = _mass[massID].center.props.is_3D_enabled;
			break;
			
		case PROP_DRAW_MODE:
			// Notice that the draw mode contains both a string and an id.
			retVal.strVal = (char*)getDrawModeStr(getDrawMode(massID));
			retVal.intVal = getDrawMode(massID);
			break;
			
		case PROP_DRAW_EMITTER:
			retVal.boolVal = _mass[massID].center.props.draw_emitter;
			break;
	}
	
	return retVal;
	
}




#else



CParticleSystem::CParticleSystem()
{
	
}


CParticleSystem::~CParticleSystem()
{
	
}


void CParticleSystem::initMass(const int massID, const int massSize)
{

}


void CParticleSystem::init(const int numMasses)
{

}


void CParticleSystem::init(const int numMasses, const int massSize)
{

}


void CParticleSystem::destroy()
{

}


void CParticleSystem::draw(eParticleDrawMode drawMode, void* data)
{	

}


void CParticleSystem::update()
{

}


void CParticleSystem::releaseNextParticle(int massID)
{

}


void CParticleSystem::killParticle(int massID, int particleID)
{

}	


void CParticleSystem::recenterMass(int massID, int x, int y)
{

}


void CParticleSystem::recenterMass(int massID, int x, int y, int z)
{

}


void CParticleSystem::recenterParticles(int massID)
{

}


void CParticleSystem::applyProperties(int massID, int particleID)
{	

}


void CParticleSystem::setMode(int massID, eParticleMode mode)
{

}


void CParticleSystem::setMode(int massID, particleProperties *modeData)
{

}

void CParticleSystem::setAngles(int massID, int numAngles, ...)
{

}

void CParticleSystem::applyImpulse(int massID, int xImp, int yImp)
{

}

void CParticleSystem::applyImpulse(int massID, int xImp, int yImp, int zImp)
{

}

void CParticleSystem::setImageID(const int massID, const int imageID)
{
	
}

void CParticleSystem::setPhysicsState(int massID, ePhysicsMovementState state)
{
	
}


#endif