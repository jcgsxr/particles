/*
 *  SoundSystem.cpp
 *  test_framework
 *
 *  Created by Johnny Chan on 11/14/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */

#import "SoundEngine.h"
#include "Utils.h"
#include "Engine.h"
#include "SystemDefines.h"

// seek in audio file for the property 'size'
// return the size in bytes
UInt32 audioFileSize(AudioFileID fileDescriptor)
{
	UInt64 outDataSize = 0;
	UInt32 thePropSize = sizeof(UInt64);
	OSStatus result = AudioFileGetProperty(fileDescriptor, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize);
	if(result != 0) NSLog(@"cannot find file size");
		return (UInt32)outDataSize;
}


CSoundEngine::CSoundEngine()
{
	
}

CSoundEngine::~CSoundEngine()
{
	
}

static NSMutableArray *bufferStorageArray;
static NSMutableArray *fileNames;

void CSoundEngine::init(const soundLoadInfo** soundData, int numSounds)
{	
	mDevice = alcOpenDevice(NULL); // select the "preferred device"
	
	if (mDevice)
	{
		// use the device to make a context
		mContext = alcCreateContext(mDevice,NULL);
		
		// set my context to the currently active one
		alcMakeContextCurrent(mContext);
	}

	bufferStorageArray = [[NSMutableArray alloc] init];
	fileNames = [[NSMutableArray alloc] init];
	
	_num_sounds = numSounds;

	_sound_headers = ArrayList<soundHeader>::alloc(numSounds);
	
	NSString *nsFullFileName;
	NSString *nsLongFileName;
	NSString *nsFileName;
	NSString *fileType;
	NSString *path;
	
	// For convenience.
	soundLoadInfo theSoundInfo;
	
	for (int i = 0; i < numSounds; ++i)
	{	
		theSoundInfo = ((soundLoadInfo*)soundData)[i];
		
		_sound_headers[i].file_name = (char*)theSoundInfo.file_name;
		_sound_headers[i].sound_id = i;
		_sound_headers[i].priority = theSoundInfo.priority;
		
		nsFullFileName = [NSString stringWithUTF8String:_sound_headers[i].file_name];
		nsLongFileName = [nsFullFileName stringByDeletingPathExtension];
		nsFileName = [nsLongFileName lastPathComponent];
		fileType = [nsFullFileName pathExtension];
		path = [nsFullFileName stringByDeletingLastPathComponent];
		// Get the absolute path to the file
		fileNames[i] = [[[NSBundle mainBundle] pathForResource:nsFileName ofType:fileType inDirectory:path] retain];
//		_sound_headers[i].ns_file_name = [[[NSBundle mainBundle] pathForResource:nsFileName ofType:fileType inDirectory:path] retain];
		
		_sound_headers[i].loaded = TRUE;
		
		_sound_headers[i].al_source_id = loadSoundWithKey(i, false);
	}
	
	// Initialize the sound channels.
	memset(&_sound_channels, SOUND_CHANNEL_FREE, sizeof(int) * SOUND_CHANNEL_MAX);
	
	_is_enabled = TRUE;
}

void CSoundEngine::destroy()
{
	// delete the buffers
	for (NSNumber * bufferNumber in bufferStorageArray) 
	{
		NSUInteger bufferID = [bufferNumber unsignedIntegerValue];
		alDeleteBuffers(1, &bufferID);
	}
	[bufferStorageArray removeAllObjects];
	bufferStorageArray = nil;
	
//	for (int i = 0; i < _num_sounds; ++i)
//	{
//		[_sound_headers[i].ns_file_name release];
//	}
	_sound_headers = NULL;

	// destroy the context
	alcDestroyContext(mContext);
	// close the device
	alcCloseDevice(mDevice);
}


void CSoundEngine::playSound(int soundID)
{	
	if (!_is_enabled)
	{
		return;
	}
	
	// Check for any available sound channels.
	for (int i = 0; i < SOUND_CHANNEL_MAX; ++i)
	{
		// Check for any free sound channels first.
		if (_sound_channels[i] == SOUND_CHANNEL_FREE)
		{
			playSound(soundID, i);
			break;
		}
		// Now check priorities, lower values mean lower priority.
		else if (_sound_headers[_sound_channels[i]].priority == _sound_headers[soundID].priority)
		{
			if (isPlayingSound(_sound_channels[i]))
			{
				continue;
			}
			playSound(soundID, i);
			break;
		}
		// If the new priority is higher, we must stop the current sound immediately and play the new one.
		// Check for a higher incoming priority.
		else if (_sound_headers[_sound_channels[i]].priority < _sound_headers[soundID].priority)
		{
			if (isPlayingSound(_sound_channels[i]))
			{
				stopSound(_sound_channels[i]);
			}
			playSound(soundID, i);
			break;
		}
	}
}


// the main method: grab the sound ID from the library
// and start the source playing
void CSoundEngine::playSound(int soundID, int channel)
{
	if (_sound_headers[soundID].al_source_id == 0)
	{
		return;
	}
	
	ALuint sourceID = _sound_headers[soundID].al_source_id;
	
	alSourcePlay(sourceID);
	
	// Indicate that the channel is now playing a sound.
	_sound_channels[channel] = soundID;
}

void CSoundEngine::stopSound(int soundID)
{	
	if (_sound_headers[soundID].al_source_id == 0)
	{
		return;
	}
	
	ALuint sourceID = _sound_headers[soundID].al_source_id;
	
	alSourceStop(sourceID);
}

void CSoundEngine::stopAllSounds()
{
	for (int i = 0; i < SOUND_CHANNEL_MAX; ++i)
	{
		stopSound(i);
	}	
}

void CSoundEngine::rewindSound(int soundID)
{	
	if (_sound_headers[soundID].al_source_id == 0)
	{
		return;
	}
	
	ALuint sourceID = _sound_headers[soundID].al_source_id;
	
	alSourceRewind (sourceID);
}

BOOL CSoundEngine::isPlayingSound(int soundID)
{	
	if (_sound_headers[soundID].al_source_id == 0)
	{
		return FALSE;
	}	
	
	ALuint sourceID = _sound_headers[soundID].al_source_id;
	
//	DPRINT_SOUND("CSoundEngine::isPlayingSound sourceID: %d \n", sourceID);
	
	ALenum state;
	
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	
	// Set the state to free if the sound is not playing.
	if (state != AL_PLAYING)
	{
		_sound_channels[soundID] = SOUND_CHANNEL_FREE;
	}
	
    return (state == AL_PLAYING);
}

unsigned int CSoundEngine::loadSoundWithKey(int soundID, BOOL loop)
{			
	ALvoid *outData;
	ALenum  error = AL_NO_ERROR;
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
//	NSURL *fileURL = [NSURL fileURLWithPath:_sound_headers[soundID].ns_file_name];
	NSURL *fileURL = [NSURL fileURLWithPath:fileNames[soundID]];
	
	if (!fileURL)
	{
		DPRINT_SOUND("file not found. \n");
		return 0;
	}
	
	outData = getOpenALAudioData((CFURLRef)fileURL, &size, &format, &freq);
		
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		if (outData)
		{
			free(outData);
			outData = NULL;
		}
		DPRINT_SOUND("loadSoundWithKey error loading sound: %x\n", error);
		
		return 0;
		
		//exit(1);
	}
	
	//ALog(@"getting a free buffer from openAL.");
	NSUInteger bufferID;
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	//ALog(@"loading audio data into openAL buffer.");
	// load the awaiting data blob into the openAL buffer.
	alBufferData(bufferID, format, outData, size, freq); 
	
	// save the buffer so we can release it later
	[bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
	
	//ALog(@"getting a free source from openAL.");
	NSUInteger sourceID;
	// grab a source ID from openAL
	alGenSources(1, &sourceID); 
	
	//ALog(@"attatching the buffer to the source and setting up preferences");
	// attach the buffer to the source
	alSourcei(sourceID, AL_BUFFER, bufferID);
	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	if (loop) 
		alSourcei(sourceID, AL_LOOPING, AL_TRUE);
		
	//ALog(@"free %i bytes of temporary allocated memory.", size);
	// clean up the buffer
	if (outData)
	{
		free(outData);
		outData = NULL;
	}
	
	//DPRINT_SOUND("Audiofile successfully loaded. \n");
	return sourceID;
}



ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
	static	alBufferDataStaticProcPtr	proc = NULL;
	
    if (proc == NULL) {
        proc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    }
	
    if (proc)
        proc(bid, format, data, size, freq);
	
    return;
}

void* getOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate)
{
	OSStatus err = noErr;
	SInt64 theFileLengthInFrames = 0;
	AudioStreamBasicDescription theFileFormat;
	UInt32 thePropertySize = sizeof(theFileFormat);
	ExtAudioFileRef extRef = NULL;
	void* theData = NULL;
	AudioStreamBasicDescription theOutputFormat;
	UInt32 dataSize;
	
	// Open a file with ExtAudioFileOpen()
	err = ExtAudioFileOpenURL(inFileURL, &extRef);
	
	if(err) 
	{
		DPRINT_SOUND("getOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ld\n", err); 
		goto Exit;
	}
	
	// Get the audio data format
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	if(err) 
	{ 
		DPRINT_SOUND("getOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err); 
		goto Exit; 
	}
	if (theFileFormat.mChannelsPerFrame > 2)  
	{
		DPRINT_SOUND("getOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
		goto Exit;
	}
	
	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
	
	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	// Set the desired client (output) data format
	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	if(err) 
	{
		DPRINT_SOUND("getOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err);
		goto Exit; 
	}
	
	// Get the total frame count
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	if(err) { 
		DPRINT_SOUND("getOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err); 
		goto Exit; 
	}
	
	// Read all the data into memory
	dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
	theData = malloc(dataSize);
	if (theData)
	{
		AudioBufferList theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = theData;
		
		// Read the data into an AudioBufferList
		err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
		if(err == noErr)
		{
			// success
			*outDataSize = (ALsizei)dataSize;
			*outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			*outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
		}
		else
		{
			// failure
			free (theData);
			theData = NULL; // make sure to return NULL
			DPRINT_SOUND("getOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err); goto Exit;
		}
	}
	
Exit:
	// Dispose the ExtAudioFileRef, it is no longer needed
	if (extRef) ExtAudioFileDispose(extRef);
	return theData;
}
