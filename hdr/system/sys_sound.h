#pragma once

extern int 		numSoundDevices;
extern bool 	playSounds;
extern bool		as_useSound;
extern int		as_numSamples;
extern int		as_numMultiSamples;
extern bool		pauseSound;
extern float	volumeLevel;
extern float	alertLevelDistance;
extern string	volumeLevelStr;


struct _sounds
	{
	ALLEGRO_SAMPLE			*sample;
	const char				*fileName;
	bool					loadedOk;
	ALLEGRO_SAMPLE_INSTANCE	*instance;
	};

extern _sounds		sound[];

//
// Structure to hold the second or more instance
// of a sample if it is already playing
//
struct _multiSounds
	{
	ALLEGRO_SAMPLE_INSTANCE	*instance;
	};

extern _multiSounds	*multiSounds;

// Install sound system
bool sys_startSound();

// Release the sound systems and samples
void sys_releaseSound();

// Load each sound sample
void sys_loadSoundSamples();

// Play a sample
bool sys_playSound(int whichSound, float pan, ALLEGRO_PLAYMODE loop);

//Stop playing a sound - pass in sound index to get SAMPLE_ID
void sys_stopSound(int whichSound);

// Check if a sample is still playing - return bool
bool sys_isSoundPlaying(int whichSound);

// Stop playing all sounds
void sys_stopAllSounds();

// Pause the sound system
void sys_pauseSoundSystem(bool newState);

// Play a sound from a external script
void sys_playSoundFromScript(int whichSound, float pan, bool loopIt);

// Set volume (gain) for a sample - needs to be already playing
void sys_setVolume(int whichSound, float volLevel);