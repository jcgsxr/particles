/*
 *  SoundEngine.h
 *  test_framework
 *
 *  Created by Johnny Chan on 11/14/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */

#ifndef __SOUNDENGINE_H__
#define __SOUNDENGINE_H__

#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>
//#import <UIKit/UIKit.h>
#include "ArrayList.h"


static const int SOUND_CHANNEL_MAX = 8;		/*!< The maximum number of sounds that can be played simultaneously. */
static const int SOUND_CHANNEL_FREE = -1;	/*!< Value to indicate that a sound channel is free to use. */


/*! \struct soundHeader
 *	\brief The sound header.
 *
 * A sound header holds information for one sound effect.
 */
typedef struct soundHeader
{
	char* file_name;			/*!< The file name for the actual sound file. */
	int sound_id;				/*!< The id associated with this particular sound. */
	int priority;				/*!< The priority of this sound. */
	unsigned int al_source_id;	/*!< Used by openal to identify the sound. */
//	NSString *ns_file_name;		/*!< Used to determine the location of the sound file in the file system, used for loading. */
	BOOL loaded;				/*!< Indicates that this sound was loaded. */
} soundHeader;

/*! \struct soundLoadInfo
 *	\brief The sound loading info.
 *
 * This is used for passing in sound loading data upon initialization.
 */
typedef struct soundLoadInfo
{
	const char *file_name;
	int priority;
} soundLoadInfo;

/*! \class CSoundEngine
 * \brief The Sound Engine class.
 *
 * The Sound Engine class is responsible for loading and playing sound effects or music.
 */
class CSoundEngine
{
public:
	
	/*! \fn CSoundEngine()
	 *  \brief The CSoundEngine class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CSoundEngine();
	
	/*! \fn ~CSoundEngine()
	 *  \brief The CSoundEngine class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CSoundEngine();
	
	/*! \fn init(const char** fileNames, int* priorities, int numSounds)
	 *  \brief The CSoundEngine class initializer.
	 *  
	 * Sound files are loaded into memory here. The sound engine will not work until this function is called.
	 *	\param fileNames A pointer to the file names array.
	 *	\param priorities A pointer to the priorities of each sound. The size of the array must be the same as the file names array since each priority must correspond to one sound file.
	 *	\param numSounds The number of sounds to load. This must be the same as the size of the #fileNames and #priorities arrays.
	 *  \return n/a
	 */
	void init(const soundLoadInfo** soundData, int numSounds);
	
	/*! \fn destroy(void)
	 *  \brief The CSoundEngine class destroy function.
	 *  
	 * Any memory allocated for loading sounds in #init() is released here. This is called by CEngine::destroy() upon application termination.
	 *	\param n/a
	 *  \return n/a
	 */
	void destroy(void);
	
	/*! \fn playSound(int soundID)
	 *  \brief Plays the sound with the given sound ID.
	 *  
	 * Sound channels and priorities are sorted out before a sound is played. #playSound is called when it is determined that the sound may be played.
	 *	\param soundID The sound ID of the sound to be played.
	 *  \return n/a
	 */
	void playSound(int soundID);
	
	/*! \fn loadSoundWithKey(int soundID, BOOL loop)
	 *  \brief Loads the sound into the #bufferStorageArray and returns the al source id associated with the sound.
	 *  
	 *	\param soundID The sound ID of the sound to be loaded.
	 *	\param loop Indicates whether this sound should be looped.
	 *  \return The al source ID associate with the sound that has just been loaded into memory.
	 */
	unsigned int loadSoundWithKey(int soundID, BOOL loop);
	
	/*! \fn isPlayingSound(int soundID)
	 *  \brief Checks whether the sound with the given sound ID is currently playing.
	 *  
	 *	\param soundID The sound ID of the sound to be checked.
	 *  \return TRUE if the sound is currently playing, FALSE otherwise.
	 */
	BOOL isPlayingSound(int soundID);
	
	/*! \fn rewindSound(int soundID)
	 *  \brief Rewinds a sound.
	 *  
	 *	\param soundID The ID of the sound to rewind.
	 *  \return n/a
	 */
	void rewindSound(int soundID);
	
	/*! \fn stopSound(int soundID)
	 *  \brief Stops the sound with the given sound id.
	 *  
	 *	\param soundID The ID of the sound to stop.
	 *  \return n/a
	 */
	void stopSound(int soundID);
	
	/*! \fn playSound(int soundID, int channel)
	 *  \brief Plays a sound with the given sound id in the given sound channel.
	 *  
	 *	\param soundID The ID of the sound to play.
	 *	\param The channel to play the sound in.
	 *  \return n/a
	 */
	void playSound(int soundID, int channel);

	/*! \fn stopAllSounds(void)
	 *  \brief Stop all currently playing sounds.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void stopAllSounds(void);

	/*! \fn enableSounds(void)
	 *  \brief Enable the sound engine to play sounds.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	inline void enableSounds(void) { _is_enabled = TRUE; }
	
	/*! \fn disableSounds(void)
	 *  \brief Disable the sound engine from playing sounds.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	inline void disableSounds(void) { stopAllSounds(); _is_enabled = FALSE; }
	
	/*! \fn isSoundEnabled(void)
	 *  \brief Returns a boolean indicating whether the sound engine is enabled or not.
	 *  
	 *	\param n/a
	 *  \return Returns TRUE if the sound engine is enabled, FALSE otherwise.
	 */
	inline BOOL isSoundEnabled(void) { return _is_enabled; }
	
private:
//	NSMutableArray *bufferStorageArray;	/*!< The buffer where the sound data is loaded into. */
	ALCcontext *mContext;				/*!< Stores the context. The context is the current 'session' that all sounds are going to be played in. Think of this as the sound room that all sources and the listener is in. */
	ALCdevice *mDevice;					/*!< Stores the device. The device is the actual sound hardware. */
	ArrayList<soundHeader> _sound_headers;	/*!< The sound headers contain information about each sound to be loaded. */
	int _num_sounds;					/*!< The total number of sounds to be loaded. */
	BOOL _is_enabled;					/*!< Indicates whether sounds are enabled or not. */
	int _sound_channels[SOUND_CHANNEL_MAX];	/*!< Represents the sound channels and holds the sound id that is currently playing in that channel. */
};


// OpenAL related util functions.
typedef ALvoid	AL_APIENTRY	(*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);
extern ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);
extern void* getOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate);


#endif