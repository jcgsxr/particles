#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "types.h"
#include "SystemDefines.h"
#include "SoundEngine.h"
#include "Font.h"

//////////////////////////////////////////
// The following are used in ImageLoader
//////////////////////////////////////////
// The enum of all available image packs to select from.
// This will be passed in to loadImagePack to load a desired set of images.
typedef enum _eImagePacks
{
	eImagePackNULL = -1,
	eImagePackComplete = 0,
	eImagePackSplash,
	eImagePackTitle,
	eImagePackMain,
	eImagePackMenu,
	eImagePackExit,
	eImagePackUnitTest,
	eImagePackMAX,
} eImagePacks;

extern const char* imageFileNames[];

typedef enum _eImageFileId
{
	FILE_ID_IMAGE_NULL,
	FILE_ID_IMAGE_LLAMAFACE_SPLASH,
	FILE_ID_IMAGE_PARTICLE,
	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARK,
	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARK_BIG,
	FILE_ID_IMAGE_PARTICLE_ORANGE_SPARK_BIG,
	FILE_ID_IMAGE_PARTICLE_CLOUD,
	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARKLE,
	FILE_ID_IMAGE_PARTICLE_BLUE_SPARKLE,
	FILE_ID_IMAGE_PARTICLE_DROPLET,
} eImageFileId;



extern const soundLoadInfo soundLoadInfoData[];
typedef enum _eSoundFileId
{
	FILE_ID_SOUND_BONK,
	FILE_ID_SOUND_MAX,
} eSoundFileId;



extern const fontLoadInfo fontLoadInfoData[];
// The different font types.
typedef enum _eFont
{
	eFontBlack8x12 = 0,
	eFontBlack16x24,
	eFontMAX,
} eFont;



// The following are the actual indexes to the file ids used for image loading.
static const uint32 image_pack_complete[] =
{
	FILE_ID_IMAGE_NULL,
};

static const uint32 image_pack_splash[] =
{
	FILE_ID_IMAGE_LLAMAFACE_SPLASH,
};

static const uint32 image_pack_title[] = 
{

};

static const uint32 image_pack_main[] = 
{

};

static const uint32 image_pack_unittest[] = 
{

};







/*! \enum eParticleMode
 *	\brief Pre-set particle modes. The associated data is stored in the #particle_mode_props data array located in GameData.cpp.
 */
typedef enum eParticleMode
{
	eParticleModeNone = 0,		/*!< Null particle mode. Nothing occurs in this mode. */
	eParticleModeSparkler,		/*!< Particles are released in a constant stream randomly in all directions at a low velocity. */
	eParticleModeScreenSparkler,/*!< Particles are spread out on the screen, rotate and twinkle. */
	eParticleModeFall,			/*!< Particles have a starting velocity facing downwards within a small angle. */
	eParticleModeFireworks,		/*!< Particles shoot outwards in all directions with high velocity and within a small timeframe. */
	eParticleModeCircle,		/*!< Particles are all released in one frame in a circular pattern. */
	eParticleModeFire,			/*!< Particles move upwards within a small angle. */
	eParticleModeBurst,			/*!< Similar to fireworks but much smaller and more spread out. */
	eParticleModeSmokeTrail,	/*!< Used for moving masses that leave a trail of particles behind that expand and fade slowly. */
	eParticleModeSteam,			/*!< Particles eject upwards while expanding quickly. */
	eParticleModeBurstSmall,	/*!< Like eParticleModeBurst but smaller. */
	eParticleModeSun,			/*!< Like fire mode but in all directions and with no gravity. */
	eParticleModeShooter,		/*!< Very slow rate of particle release, and very long life time. */
	eParticleModeBigBang,		/*!< Very high rate of particle release, and very long life time. */
	eParticleModeSnow,
	eParticleModeMAX,			/*!< The total number of preset particle modes. */
} eParticleMode;

// Particle property data.
extern const particleProperties particle_mode_props[eParticleModeMAX];


#endif