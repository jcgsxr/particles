/*
 *  ParticleSystem.h
 *  Wurd
 *
 *  Created by Johnny Chan on 3/12/10.
 *  Copyright 2010 LlamaFace. All rights reserved.
 *
 */

#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "physics_types.h"
#include "ArrayList.h"
#include "physics.h"
#include "Sprite.h"

static const int PARTICLE_RADIUS_DEFAULT = 8;


/*! \enum eParticleDrawMode
 *	\brief The particle draw mode indicates the different ways that the particle system renders its particles.
 */
typedef enum eParticleDrawMode
{
	eParticleDrawModeNormal = 0,	/*!< Particles are rendered using normal textured quads. This mode is used when specialized particle effects are needed, such as rotation. */
	eParticleDrawModeBillBoard,		/*!< Billboard mode is used when rendering particles in 3D mode. All particles are faced at the camera in this mode. */
	eParticleDrawModePoint,	/*!< Point sprites are less expensive to render than quads since it consists of only one vertex (versus 4 for a quad). Specialized effects such as rotation are not possible in this mode. */
	eParticleDrawModeMAX,			/*!< The total number of possible draw modes. */
} eParticleDrawMode;


/*! \struct particleProperties
 *	\brief Particle properties is what defines the behavior of a particle.
 *
 * All particles must set these properties for it to function correctly.
 */
typedef struct particleProperties
{
	int life_time;		/*!< The amount of time that the particle is visible and being updated. This value can be set to #__INF for when the particle has an infinite life time. */
	int fade_speed;		/*!< The speed at which the particle will fade in or out of visibility. This value can be set to #__INF for when the particle never fades. */
	int fade_count;		/*!< The number of times that the particle will fade in and out of visibility. */
	float slowdown_rate;/*!< The rate at which the velocity of the particle deteriorates. Calculated as a percentage from 1.0 (no deterioration) to 0.0 (full deterioration). */
	int vel_base;		/*!< The base velocity of the particle when it is released from the mass. */
	int vel_rand;		/*!< The particle velocity randomness when it is released from the mass. Higher value means larger random threshold. */
	int angle_min;		/*!< Defines a bounding edge that particles can be released. */
	int angle_max;		/*!< Defines the other bounding edge that particles can be released. Must be a larger value than #angle_min. */
	int release_time;	/*!< The speed in MS at which the particles will be released from the mass. */
	int release_rate;	/*!< The rate at which the particles will be released from the mass. */
	int release_rand;	/*!< The release rate randomness in MS. */
	float gravity;		/*!< The strength of gravity on the particle. */
	int	rotation_speed;	/*!< The speed at which particles rotate, measured in the time it takes in MS to do one full rotation. */
	int rotation_rand;	/*!< A random factor for the rotation speed. */
	float size_start;	/*!< The initial size scale of the particle as it is ejected. The particle will pulse back and forth from this size to #size_end. The number of pulses and pulse duration is determined by #size_count and #size_speed. */
	float size_end;		/*!< The end size scale of the particle during its size pulse loop. */
	float size_speed;	/*!< The speed in MS at which the size scale changes from start to end. This value can be set to #__INF for when the particle never changes size. */
	int size_count;		/*!< The number of times that the size scale changes from start to end. */
	int blink_on_time;	/*!< The time that the particle is visible during a blink action. */
	int blink_on_rand;	/*!< The random time value for the blink-on time. */
	int blink_off_time;	/*!< The time that the particle is not visible during a blink action. */
	int blink_off_rand;	/*!< The random time value for the blink-off time. */
	int blink_count;	/*!< The number of times that the particle blinks in and out of visibility. This value can be set to #__INF for infinite blinks to occur. */
	int release_dist;	/*!< The distance from the center of the mass to release the particle. */
	int release_dist_rand;	/*!< The random value added to the release distance. */
	BOOL glows;			/*!< Indicates whether to use the blend function GL_ONE or GL_ONE_MINUS_SRC_ALPHA. When set to GL_ONE, colors become additive when particles overlap each other, creating a "glow" effect.*/
	float r;			/*!< The red color component of the particle. */
	float g;			/*!< The green color component of the particle. */
	float b;			/*!< The blue color component of the particle. */
	bool color_rand;	/*!< Indicates whether colors are randomized. */
	BOOL is_3D_enabled;	/*!< Indicates whether to render this particle in 3d mode. */
	int strand_length;	/*!< Used for making a trail of particles. Indicates the length of the strand in particles. */
	bool frame_skip;	/*!< Indicates whether to allow skipping of physics frames, to have time-accurate rendering, or showing all frames. */
	bool draw_emitter;	/*!< Draws the center point of the mass, also known as the particle emitter. */
	short draw_mode;	/*!< The rendering mode for this particle. */
	int image_id;		/*!< The file image file ID found in gamedata.h */
} particleProperties;

/*! \struct particlePropNames
 *	\brief This is used for querying a specific property of the particle properties.
 *
 */
typedef struct particlePropNames
{
	int type_id;
	int data_type;
	float increment_value;
	const char* type_name;
	const char* description;
} particlePropNames;


/*! \enum eParticlePropertyName
 *	\brief Used as a reference when querying the string name of a specific particle property.
 */
typedef enum eParticlePropertyName
{
	PROP_LIFE = 0,
	PROP_FADE_SPEED,
	PROP_FADE_COUNT,
	PROP_SLOWDOWN_RATE,
	PROP_VEL_BASE,
	PROP_VEL_RAND,
	PROP_ANGLE_MIN,
	PROP_ANGLE_MAX,
	PROP_REL_TIME,
	PROP_REL_RATE,
	PROP_REL_RAND,
	PROP_GRAVITY,
	PROP_ROT_SPEED,
	PROP_ROT_RAND,
	PROP_SIZE_START,
	PROP_SIZE_END,
	PROP_SIZE_SPEED,
	PROP_SIZE_COUNT,
	PROP_BLINK_ON_TIME,
	PROP_BLINK_ON_RAND,
	PROP_BLINK_OFF_TIME,
	PROP_BLINK_OFF_RAND,
	PROP_BLINK_COUNT,
	PROP_REL_DIST,
	PROP_REL_DIST_RAND,
	PROP_GLOWS,
	PROP_COLOR_R,
	PROP_COLOR_G,
	PROP_COLOR_B,
	PROP_COLOR_RAND,
	PROP_IMAGE_ID,
	PROP_MASS_SIZE,
	PROP_STRAND_LENGTH,
	PROP_3D_MODE,
	PROP_DRAW_MODE,
	PROP_DRAW_EMITTER,
	PROP_MAX,
} eParticlePropertyName;

/*! \enum eParticlePropertyDataType
 *	\brief Used as a reference when querying the data type of a specific particle property.
 */
typedef enum eParticlePropertyDataType
{
	PROP_DATA_TYPE_INT = 0,
	PROP_DATA_TYPE_FLOAT,
	PROP_DATA_TYPE_BOOL,
	PROP_DATA_TYPE_STR,
	PROP_DATA_TYPE_GENERIC,
}eParticlePropertyDataType;

// This is used to a general return type when querying for a property.
union propVal
{
	float floatVal;
	int intVal;
	bool boolVal;
	char* strVal;
};

/*! \struct particle
 *	\brief Each particle sets its properties according to the mass center that ejects it.
 *
 * Particles contain a sprite for rendering and special visual effects, and a physics object for emulating velocity, acceleration, and other physical properties.
 */
typedef struct particle
{
	BOOL is_active;				/*!< Indicates wether to run update and render on this particle. */
	CSprite sprite;				/*!< The sprite associated with this particle. */
	physicsObject phys;			/*!< The physics object associated with this particle. */
	ArrayList<Vector3> pos_history;	/*!< Stores the history of the particle rendered positions to enable drawing of strands. */
	int pos_history_counter;	/*!< Used to keep track of how many particles are to be rendered in the strand. */
	int pos_history_active_count;	/*!< This value is used to ensure that particle strands that haven't been set yet won't render. */
	particleProperties props;	/*!< The particle properties associated with this particle. */
	int id;						/*!< Specifies which mass this particle belongs to. */
} particle;


/*! \struct particleMass
 *	\brief A particle mass represents the center particle and the mass of particles that are attached to it.
 */
typedef struct particleMass
{
	ArrayList<particle> particles;	/*!< The particles associated with this particle mass. */
	int num_particles;		/*!< The total number of particles that this mass contains. */
	particle center;		/*!< The center of the particle mass is where the rest of the particle will be ejected from. */
	int rel_counter;		/*!< The release rate time counter. */
	int loop_count;			/*!< The number of times the mass will release its set of particles. */
	int loop_counter;		/*!< Holds the current loop count. */
	int rel_particle_counter;	/*!< Counter for the number of particles that have been released from the mass. Used to help determine when to increment the loop_counter. */
	Vector3 initial_pos;	/*!< The initial position of the particle mass. */
	char* image_name;		/*!< The image name of the particle. */
} particleMass;

/*! \class CParticleSystem
* \brief The Particle System class.
*
* The particle system is used for special visual effects that involve (usually) small textures that represent some type of particle. Particles can be anything from a sparkle, to a flame, to clouds, etc...
*/
class CParticleSystem
{
public:	

	/*! \fn CParticleSystem()
	 *  \brief The CParticleSystem class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CParticleSystem();

	/*! \fn ~CParticleSystem()
	 *  \brief The CParticleSystem class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CParticleSystem();
	
	/*! \fn init(const int numMasses, const int massSize)
	 *  \brief Allocates memory for the particle masses and initializes sprite and physics object properties.
	 *  
	 * This init method will automatically set all particle masses to have the same number of particles. See the other init methods if there is a need to customize the number of particles per mass.
	 *	\param numMasses The number of masses of this particle system.
	 *	\param massSize The number of particles per particle mass.
	 *  \return n/a
	 */
	void init(const int numMasses, const int massSize);

	/*! \fn init(const int numMasses)
	 *  \brief Initialize the particle masses without initializing the mass' particles.
	 *  
	 * This init method is used with #initMass() in order to customize how many particles each mass will have.
	 *	\param numMasses The number of masses of this particle system.
	 *  \return n/a
	 */
	void init(const int numMasses);

	/*! \fn initMass(const int massID, const int massSize)
	 *  \brief Initializes an individual mass. init() MUST be called before this is called.
	 *  
	 *	\param massID The ID of the mass to initialize.
	 *	\param massSize The number of particles to initialize for this particle mass.
	 *  \return n/a
	 */
	void initMass(const int massID, const int massSize);

	/*! \fn destroy()
	 *  \brief The CParticleSystem class destroy function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void destroy(void);

	/*! \fn update()
	 *  \brief Updates physics and particle animations for all active particle and particle masses.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void update(void);

	/*! \fn draw(void* data = NULL)
	 *  \brief The CParticleSystem draw function.
	 *  
	 *	\param data Any data that is needed for a particular draw mode. For instance, some modes require a view matrix in order to align the particles in a certain way.
	 *  \return n/a
	 */
	void draw(void* data = NULL);
	
	/*! \fn recenterMass(int massID, int x, int y)
	 *  \brief Centers the mass and all particles belonging to the mass to the given coordinates.
	 *  
	 *	\param massID The ID of the mass to execute particle recentering on.
	 *	\param x The x location to center the mass at.
	 *	\param y The y location to center the mass at.
	 *  \return n/a
	 */
	void recenterMass(int massID, int x, int y);

	/*! \fn recenterMass(int massID, int x, int y, int z)
	 *  \brief Centers the mass and all particles belonging to the mass to the given coordinates.
	 *  
	 *	\param massID The ID of the mass to execute particle recentering on.
	 *	\param x The x location to center the mass at. This value should be passed in as a screen coordinate since it will be converted to world coordinates.
	 *	\param y The y location to center the mass at. This value should be passed in as a screen coordinate since it will be converted to world coordinates.
 	 *	\param z The z location to center the mass at. This value should be passed in as a screen coordinate since it will be converted to world coordinates.
	 *  \return n/a
	 */
	void recenterMass(int massID, int x, int y, int z);
	
	/*! \fn recenterParticles(int massID)
	 *  \brief Resets the positions of all particles belonging to a mass, back to the center of the mass.
	 *  
	 *	\param massID The ID of the mass to recenter.
	 *  \return n/a
	 */
	void recenterParticles(int massID);

	/*! \fn setMode(int massID, particleProperties &modeData)
	 *  \brief Sets non-pre-defined custom particle properties.
	 *  
	 *	\param massID The particle mass ID.
	 *	\param modeData The particle data to apply to the mass.
	 *	\param imageName The image name is used for editing purposes and can be null.
	 *  \return n/a
	 */
	void setMode(int massID, const particleProperties &modeData, const char* imageName = "");
	
	/*! \fn applyProperties(int massID, int particleID)
	 *  \brief Applies the properties of the center mass particle to a specific particle in same mass.
	 *  
	 *	\param massID The particle mass ID.
	 *	\param particleID The ID for the specific particle in the particle mass.
	 *  \return n/a
	 */
	void applyProperties(int massID, int particleID);
	
	/*! \fn releaseNextParticle(int massID)
	 *  \brief Releases one non-active particle from the given mass.
	 *  
	 *	\param massID The ID for the mass from which to release the particle from.
	 *  \return n/a
	 */
	void releaseNextParticle(int massID);
	
	/*! \fn killParticle(int massID, int particleID)
	 *  \brief Sets the given particle to an inacitve state and moves it back to the center of the mass.
	 *  
	 *	\param massID The particle mass ID.
	 *	\param particleID The ID of the particle to "kill".
	 *  \return n/a
	 */
	void killParticle(int massID, int particleID);
	
	/*! \fn setMassActive(int massID, BOOL active)
	 *  \brief Sets whether the given mass should be updated.
	 *  
	 *	\param massID The particle mass ID.
	 *	\param active TRUE to set the mass to active, FALSE otherwise.
	 *  \return n/a
	 */
	inline void setMassActive(int massID, BOOL active) { _mass[massID].center.is_active = active; }
	
	/*! \fn setIsRunning(BOOL isRunning)
	 *  \brief Sets whether the particle system should be updated.
	 *  
	 *	\param isRunning TRUE to set the particle system to active, FALSE otherwise.
	 *  \return n/a
	 */
	inline void setIsRunning(BOOL isRunning) { _is_running = isRunning; }
	
	/*! \fn pause(void)
	 *  \brief When called, the particle system will not run update().
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	// When called, the particle system will not run update().
	inline void pause(void) { _is_running = FALSE; }
	
	/*! \fn resume(void)
	 *  \brief When called, the particle system will resume running update().
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	inline void resume(void) { _is_running = TRUE; }
	
	/*! \fn setAngles(int massID, int numAngles, ...)
	 *  \brief Takes an arbitrary number of angles and sets particles to be released at those angles.
	 *  
	 * This is useful for when there is a need to eject particles at a specific angle(s).
	 *	\param massID The particle mass ID.
	 *	\param numAngles The number of angles that will be passed in.
	 *  \return n/a
	 */
	void setAngles(int massID, int numAngles, ...);
	
	/*! \fn applyImpulse(int massID, int xImp, int yImp)
	 *  \brief Applies an impulse on the given mass.
	 *  
	 *	\param massID The ID of the particle mass to apply the impulse on.
	 *	\param xImp The x impulse.
 	 *	\param yImp The y impulse.
	 *  \return n/a
	 */
	void applyImpulse(int massID, int xImp, int yImp);

	/*! \fn applyImpulse(int massID, int xImp, int yImp, int zImp)
	 *  \brief Applies an impulse on the given mass.
	 *  
	 *	\param massID The ID of the particle mass to apply the impulse on.
	 *	\param xImp The x impulse.
 	 *	\param yImp The y impulse.
 	 *	\param xImp The x impulse.
	 *  \return n/a
	 */
	void applyImpulse(int massID, int xImp, int yImp, int zImp);
	
	/*! \fn setDrawCenter(int massID, BOOL drawCenter)
	 *  \brief Indicates that the center mass should or should not be rendered.
	 *  
	 *	\param massID The particle mass ID.
	 *	\param drawCenter TRUE to render the particle mass center, FALSE otherwise.
	 *  \return n/a
	 */
	inline void setDrawCenter(int massID, BOOL drawCenter) { _mass[massID].center.props.draw_emitter = drawCenter; }
	
	/*! \fn setLoopCount(int massID, int count)
	 *  \brief This is used to control how many times the mass will release its set of particles.
	 *  
	 *	\param massID The particle mass ID.
	 *	\param count The number of times that this mass will release a full set of particles before becoming inactive.
	 *  \return n/a
	 */
	inline void setLoopCount(int massID, int count) { _mass[massID].loop_count = count; }
	
	/*! \fn resetLoopCounter(int massID)
	 *  \brief Resetting the loop counter will cause the mass to begin updating its particles if it has a #loop_count of higher than zero.
	 *  
	 *	\param massID The ID of the particle mass to have its loop count reset.
	 *  \return n/a
	 */
	inline void resetLoopCounter(int massID) { _mass[massID].loop_counter = 0; }
	
	/*! \fn runOnce(int massID)
	 *  \brief This is called when we want to immediately run one loop of the given mass particles' update.
	 *  
	 *	\param massID The particle mass ID.
	 *  \return n/a
	 */
	inline void runOnce(int massID) { _mass[massID].loop_count = 1; _mass[massID].loop_counter = 0; }
	
	/*! \fn getNumParticles(int massID)
	 *  \brief Returns the total number of particles that the mass contains.
	 *  
	 *	\param massID The particle mass ID.
	 *  \return n/a
	 */
	inline int getNumParticles(int massID) { return _mass[massID].num_particles; }
	
	/*! \fn getNumMasses(void)
	 *  \brief Returns the total number of masses that currently exist in the particle system.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	inline int getNumMasses(void) { return _num_masses; }
	
	/*! \fn reset(const int massID)
	 *  \brief Resets the particle mass to the first update, time 0.
	 *  
	 *	\param massID The particle mass to reset.
	 *  \return n/a
	 */
	void reset(const int massID);
	
	inline const char* getImageName(const int massID) { return _mass[massID].image_name; }
	
	inline const particleMass& getMass(int massID) {return _mass[massID]; }
	
	inline const eParticleDrawMode getDrawMode(int massID) { return (eParticleDrawMode)_mass[massID].center.props.draw_mode; }
	
	inline void setDrawMode(const int massID, eParticleDrawMode mode) { _mass[massID].center.props.draw_mode = mode; }
	
	inline void setEnable3D(int massID, BOOL enable) { _mass[massID].center.props.is_3D_enabled = enable; }
	
	inline BOOL is3DEnabled(int massID) {return _mass[massID].center.props.is_3D_enabled; }
	
	void setLifeTime(const int massID, const int lifeTime);
	
	void setFadeSpeed(const int massID, const int fadeSpeed);
	
	void setFadeCount(const int massID, const int fadeCount);
	
	void setSlowdownRate(const int massID, const float slowdownRate);
	
	void setVelBase(const int massID, const int velBase);

	void setVelRand(const int massID, const int velRand);
	
	void setAngleMin(const int massID, const int angleMin);
	
	void setAngleMax(const int massID, const int angleMax);
	
	void setReleaseTime(const int massID, const int releaseTime);
	
	void setReleaseRate(const int massID, const int releaseRate);
	
	void setReleaseRand(const int massID, const int releaseRand);
	
	void setGravity(const int massID, const float gravity);
	
	void setRotationSpeed(const int massID, const int rotationSpeed);
	
	void setRotationRand(const int massID, const int rotationRand);
	
	void setSizeStart(const int massID, const float sizeStart);
	
	void setSizeEnd(const int massID, const float sizeEnd);
	
	void setSizeSpeed(const int massID, const float sizeSpeed);
	
	void setSizeCount(const int massID, const int sizeCount);
	
	void setBlinkOnTime(const int massID, const int blinkOnTime);
	
	void setBlinkOnRand(const int massID, const int blinkOnRand);
	
	void setBlinkOffTime(const int massID, const int blinkOffTime);
	
	void setBlinkOffRand(const int massID, const int blinkOffRand);
	
	void setBlinkCount(const int massID, const int blinkCount);
	
	void setReleaseDist(const int massID, const int releaseDist);
	
	void setReleaseDistRand(const int massID, const int releaseDistRand);
	
	inline void setGlows(const int massID, const BOOL glows) { _mass[massID].center.props.glows = glows; }
	
	void setColor(const int massID, float r, float g, float b);
	
	inline void setRandColor(const int massID, const bool rand) { _mass[massID].center.props.color_rand = rand; }
	
	void setStrandLength(const int massID, const int length);
	
	void setImageID(const int massID, const int imageID);
	
	void setMassSize(const int massID, const int massSize);

	// Get the mass' particle properties.
	const particleProperties getParticleModeData(int massID) { return _mass[massID].center.props; }
	
	// Returns the name of the given property id.
	static const char* getPropName(int id);
	
	// Get the particle property data type. This is used for the Property view controller to tell how to load a table cell.
	static const int getPropDataType(int id);
	
	// Get the description of the particle property.
	static const char* getPropDesc(int id);
	
	// Get the draw mode string. It is either quads, point sprites, or billboard.
	static const char* getDrawModeStr(int id);
	
	// Get the increment value of the property. Only valid for int and float types.
	static const float getPropIncrementValue(int id);
	
	void setPropertyValue(const int massID, const int propID, const propVal value);
	
	const propVal getPropertyValue(const int massID, const int propID);
	
	/*! \fn setPhysicsState(int state)
	 *  \brief Sets the state of the physics time movement of the given mass and it's particles.
	 *  
	 *	\param massID The particle mass ID.
 	 *	\param state The physics time movement state.
	 *  \return n/a
	 */
	void setPhysicsState(int massID, ePhysicsMovementState state);

private:
	ArrayList<particleMass> _mass;	/*!< The pointer to the particle masses in the particle system. */
	int _num_masses;		/*!< The number of particle masses in the particle system. */
	BOOL _is_running;		/*!< Used to indicate whether update() is run on the particle system. When set to FALSE, all particles will essentially pause, until explicitly told to resume. */
	GLfloat _point_sizes[2];	/*!< Holds the max and min sizes that a point sprite can be. Only used in point sprite draw mode eParticleDrawModePoint. */
};


#endif