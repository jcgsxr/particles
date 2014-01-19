#include "GameData.h"
#include "SoundEngine.h"



//////////////////////////////////////////
// The following are used in ImageLoader
//////////////////////////////////////////

// Pointer to the the images file id arrays, for easy access.
const uint32* image_packs[] =
{
	image_pack_complete,
	image_pack_splash,
	image_pack_title,
};
//////////////////////////////////////////////////////////

const char* imageFileNames[FILE_ID_IMAGE_MAX] =
{
	"null.png",
	"llama_splash.png",
	"particle_16x16.png",
	"particle_spark_yellow_16x16.png",
	"particle_spark_yellow_32x32.png",
	"particle_spark_orange_32x32.png",
	"particle_cloud_32x32.png",
	"particle_sparkle_yellow_32x32.png",
	"particle_sparkle_blue_32x32.png",
	"particle_droplet_16x16.png",
};



// Lower priority values mean lower priority.
const soundLoadInfo soundLoadInfoData[FILE_ID_SOUND_MAX] =
{
	//	filename			priority
	{	"bonk.wav",         1, },
};



const fontLoadInfo fontLoadInfoData[eFontMAX] =
{
	//	filename					stchr	chrw	chrh	chrperrow	chrpercol	tbloffx	tbloffy
	{	"JoFontBlack_8x12.png",		32,		8,		12,		16,			6,			0,		0,	},
	{	"JoFontBlack_16x24.png",	32,		16,		24,		16,			6,			0,		0,	},	
};











const particleProperties particle_mode_props[eParticleModeMAX] =
{		
	//	life	fadespd	fadecnt	slwrate	velbase	velrand	angmin	angmax	reltime	relrate	relrand	grav	rotspd	rotrand	szStrt	szEnd	szSpd	szCnt	bkOn	bkOnRd	bkOff	bkOffRd	bkCnt	relDst	relDRnd	glows	r	g	b		colRand	3D		strdLen	frmSkip	drawEm	rendMode						imgID	// eParticleModeNone
	{	0,		0,		1,		0.0f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0.0,	0.0,	0,		1,		0,		0,		0,		0,		0,		0,		0,		FALSE,	0.0f,0.0f,0.0f,	FALSE,	FALSE,	0,		TRUE,	FALSE,	eParticleDrawModeNormal,	0},
	// eParticleModeSparkler
	{	__INF,	500,	1,		0.9f,	400,	500,	0,		360,	50,		10,		0,		10,		500,	1000,	0.75,	0.0,	2000,	1,		0,		50,		0,		250,	__INF,	0,		0,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		FALSE,	FALSE,	eParticleDrawModeNormal,	FILE_ID_IMAGE_PARTICLE_YELLOW_SPARK_BIG},
	// eParticleModeScreenSparkler
	{	__INF,	5000,	1,		1.0f,	0,		0,		0,		360,	100,	5,		0,		0,		2000,	1000,	0.0,	1.0,	1000,	2,		0,		50,		0,		250,	__INF,	0,		500,	TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		FALSE,	FALSE,	eParticleDrawModeNormal,	FILE_ID_IMAGE_PARTICLE_BLUE_SPARKLE},
	// eParticleModeFall
	{	__INF,	4000,	1,		1.0f,	50,		0,		235,	305,	25,		5,		0,		5,		500,	1000,	0.5,	2.0,	2000,	1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		TRUE,	FALSE,	eParticleDrawModeNormal,	FILE_ID_IMAGE_PARTICLE_DROPLET},
	// eParticleModeFireworks
	{	__INF,	5000,	1,		0.9f,	150,	100,	0,		360,	100,	200,	0,		.2,		0,		0,		1.0,	0.0,	5000,	1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	3,		FALSE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE_ORANGE_SPARK_BIG},
	// eParticleModeCircle
	{	__INF,	2000,	1,		0.9f,	100,	0,		0,		360,	500,	100,	0,		0,		0,		0,		1.0,	1.0,	0,		1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		FALSE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE},
	// eParticleModeFire
	{	__INF,	1000,	1,		1.0f,	1,		10,		255,	285,	25,		5,		0,		-1,		0,		0,		0.2,	1.0,	500,	2,		0,		0,		0,		0,		0,		0,		10,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		TRUE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE_ORANGE_SPARK_BIG},
	// eParticleModeBurst
	{	__INF,	5000,	1,		1.0f,	10,		50,		0,		360,	50,		5,		0,		0,		500,	1000,	1.0,	0.0,	5000,	1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	TRUE,	FALSE,	5,		FALSE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE},
	// eParticleModeSmokeTrail
	{	__INF,	40000,	1,		0.8f,	10,		0,		0,		360,	50,		1,		0,		0,		10000,	30000,	0.0,	5.0,	5000,	1,		0,		0,		0,		0,		0,		0,		0,		FALSE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		TRUE,	FALSE,	eParticleDrawModeNormal,	FILE_ID_IMAGE_PARTICLE_CLOUD},
	// eParticleModeSteam
	{	__INF,	2000,	1,		0.98f,	10,		0,		255,	285,	100,	3,		0,		-2,		1000,	2000,	0.0,	4.0,	1000,	1,		0,		0,		0,		0,		0,		0,		0,		FALSE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		TRUE,	FALSE,	eParticleDrawModeNormal,	FILE_ID_IMAGE_PARTICLE_CLOUD},
	// eParticleModeBurstSmall
	{	__INF,	1000,	1,		0.95f,	50,		50,		0,		360,	50,		20,		0,		0,		500,	1000,	0.5,	0.0,	2000,	1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		FALSE,	FALSE,	eParticleDrawModeNormal,	FILE_ID_IMAGE_PARTICLE_BLUE_SPARKLE},
	// eParticleModeSun
	{	__INF,	1000,	1,		1.0f,	10,		10,		0,		360,	10,		10,		0,		0,		0,		0,		0.2,	1.0,	500,	2,		0,		0,		0,		0,		0,		0,		10,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	FALSE,	0,		TRUE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE_ORANGE_SPARK_BIG},
	// eParticleModeShooter
	{	__INF,	__INF,	1,		1.0f,	10,		20,		0,		360,	50,		10,		0,		0,		0,		0,		1.0,	1.0,	__INF,	1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	TRUE,	FALSE,	0,		FALSE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE},
	// eParticleModeBigBang
	{	__INF,	__INF,	1,		1.0f,	1,		150,	0,		360,	1,		10000,	0,		0,		0,		0,		1.0,	1.0,	__INF,	1,		0,		0,		0,		0,		0,		0,		0,		TRUE,	1.0f,1.0f,1.0f,	FALSE,	TRUE,	0,		FALSE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE_ORANGE_SPARK_BIG},
	// eParticleModeSnow
	{	__INF,	40000,	1,		1.0f,	10,		0,		110,	160,	200,	2,		0,		0,		2000,	1000,	1.0,	1.0,	__INF,	1,		0,		0,		0,		0,		0,		0,		100,	TRUE,	1.0f,1.0f,1.0f,	FALSE,	TRUE,	0,		TRUE,	FALSE,	eParticleDrawModePoint,		FILE_ID_IMAGE_PARTICLE_SMALL},
};