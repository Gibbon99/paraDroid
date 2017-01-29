#include "../../hdr/sys_globals.h"

_sounds		sound[] =
{
	{NULL, "collosion1.wav", 		false},
	{NULL, "endTransmission1.wav", 	false},
	{NULL, "greenAlert.wav", 		false},
	{NULL, "lift1.wav", 			false},
	{NULL, "scrollBeeps.wav", 		false},
	{NULL, "transferdeadlock.wav", 	false},
	{NULL, "yellowAlert.wav", 		false},
	{NULL, "console1.wav", 			false},
	{NULL, "endTransmission2.wav", 	false},
	{NULL, "keypressBad.wav", 		false},
	{NULL, "lift2.wav", 			false},
	{NULL, "start1.wav", 			false},
	{NULL, "transferMove.wav", 		false},
	{NULL, "console2.wav", 			false},
	{NULL, "energyHeal.wav", 		false},
	{NULL, "keyPressGood.wav", 		false},
	{NULL, "lift3.wav", 			false},
	{NULL, "start2.wav", 			false},
	{NULL, "transferStage1.wav", 	false},
	{NULL, "damage.wav", 			false},
	{NULL, "explode1.wav", 			false},
	{NULL, "laser.wav", 			false},
	{NULL, "lowEnergy.wav", 		false},
	{NULL, "startAll.wav", 			false},
	{NULL, "transferStage2.wav", 	false},
	{NULL, "disruptor.wav", 		false},
	{NULL, "explode2.wav", 			false},
	{NULL, "levelShutdown.wav", 	false},
	{NULL, "redAlert.wav", 			false},
	{NULL, "transfer1.wav", 		false},
	{NULL, "transferStart.wav", 	false},
	{NULL, "door.wav",				false}
};


int 					numSoundDevices = 1;
bool 					playSounds;
int						as_numSamples;
int						as_numMultiSamples;
bool					as_useSound;
bool					pauseSound = false;
float					volumeLevel;
string					volumeLevelStr;
_multiSounds			*multiSounds;
float					alertLevelDistance;

//-------------------------------------------------------------
//
// Pause the sound system
void sys_pauseSoundSystem ( bool newState )
//-------------------------------------------------------------
{
	pauseSound = newState;
}

//-------------------------------------------------------------
//
// Install sound system
bool sys_startSound()
//-------------------------------------------------------------
{
	if ( false == as_useSound )
		{
			playSounds = false;
			io_logToFile ( "WARN: Sound disabled in config file." );
			return false;
		}

	if ( false == al_install_audio() )
		{
			playSounds = false;
			return false;
		}

	if ( false == al_init_acodec_addon() )
		{
			playSounds = false;
			return false;
		}

	uint32_t version = al_get_allegro_acodec_version();
	int major = version >> 24;
	int minor = ( version >> 16 ) & 255;
	int revision = ( version >> 8 ) & 255;
	int release = version & 255;

	io_logToFile ( "Sound started" );
	io_logToFile ( "%i.%i.%i Release: %i", major, minor, revision, release );
	io_logToFile ( "--------------" );

	//
	// Check as_numSamples is ok to use
	if ( ( as_numSamples <= 0 ) || ( as_numMultiSamples <= 0 ) )
		{
			io_logToFile ( "Error: as_numSamples has invalid value. Check script." );
			playSounds = false;
			return false;
		}

	if ( false == al_reserve_samples ( as_numSamples ) )
		{
			con_print ( true, true, "Error: Could not reserve samples [ %i ]", as_numSamples );
			playSounds = false;
			return false;
		}

	multiSounds = al_malloc ( sizeof ( _multiSounds ) * as_numMultiSamples );
	if ( NULL == multiSounds )
		{
			con_print ( true, false, "ERROR: Failed to get memory to hold multiSamples" );
			playSounds = false;
			return false;
		}

	volumeLevel = atof ( volumeLevelStr.c_str() ) / 100.0f;

	return true;
}

//-------------------------------------------------------------------------
//
// Release the sound systems and samples
void sys_releaseSound()
//--------------------------------------------------------------------------
{
	if ( false == as_useSound )
		return;

	sys_stopAllSounds();

	for ( int i = 0; i != NUM_SOUNDS; i++ )
		al_destroy_sample_instance ( sound[i].instance );

	al_free ( multiSounds );

	al_uninstall_audio();
}

//-------------------------------------------------------------------------
//
// Load each sound sample
void sys_loadSoundSamples()
//-------------------------------------------------------------------------
{
	if ( false == as_useSound )
		return;

	for ( int i = 0; i != NUM_SOUNDS; i++ )
		{
			strcpy ( loadingStatus, sound[i].fileName );
			loadingBarPercent += 1.0f / numImages;
			al_rest ( progressBarLoadTimes );

			sound[i].sample = al_load_sample ( sound[i].fileName );
			if ( NULL == sound[i].sample )
				{
					con_print ( true, false, "Error: Failed to load sample [ %s ]", sound[i].fileName );
					sound[i].loadedOk = false;
				}
			else
				{
					con_print ( true, false, "Loaded sample [ %s ]", sound[i].fileName );
					sound[i].loadedOk = true;
					sound[i].instance = al_create_sample_instance ( sound[i].sample );
					al_attach_sample_instance_to_mixer ( sound[i].instance, al_get_default_mixer() );
					al_set_sample_instance_gain ( sound[i].instance, volumeLevel );
				}
		}

	for ( int i = 0; i != as_numMultiSamples; i++ )
		{
			multiSounds[i].instance = al_create_sample_instance ( sound[0].sample );
		}

	loadingBarPercent = 0.0f;
}

//-------------------------------------------------------------------------
//
// Tried to play an already playing sample - create a new instance and play it
//
// Check the current number of multiSamples and if one is not playing, use this slot
// to create the new instance and start it playing
void sys_playMultiSample ( int whichSound, float pan, ALLEGRO_PLAYMODE loop )
//-------------------------------------------------------------------------
{
	for ( int indexCount = 0; indexCount != as_numMultiSamples; indexCount++ )
		{
			if ( false == al_get_sample_instance_playing ( multiSounds[indexCount].instance ) )
				{
					// Get new instance
					multiSounds[indexCount].instance = al_create_sample_instance ( sound[whichSound].sample );
					al_set_sample_instance_gain ( multiSounds[indexCount].instance, volumeLevel );

					al_attach_sample_instance_to_mixer ( multiSounds[indexCount].instance, al_get_default_mixer() );

					al_set_sample_instance_pan ( multiSounds[indexCount].instance, pan );
					al_set_sample_instance_playmode ( multiSounds[indexCount].instance, loop );
					al_play_sample_instance ( multiSounds[indexCount].instance );
					return;
				}
		}
	con_print ( true, false, "WARN: Need to increase as_numMultiSamples. Script setting is to low. Currently [ %i ]", as_numMultiSamples );
}

//-------------------------------------------------------------------------
//
// Check if a sample is still playing - return bool
bool sys_isSoundPlaying ( int whichSound )
//-------------------------------------------------------------------------
{
	if ( false == playSounds )
		return false;

	if ( NULL == sound[whichSound].instance )
		return false;

	return al_get_sample_instance_playing ( sound[whichSound].instance );
}

//-------------------------------------------------------------------------
//
// Play a sample
bool sys_playSound ( int whichSound, float pan, ALLEGRO_PLAYMODE loop )
//-------------------------------------------------------------------------
{
	if ( ( false == playSounds ) || ( true == pauseSound ) )
		return false;

	if ( true == sys_isSoundPlaying ( SND_DAMAGE ) )
		return true;

	if ( true == sys_isSoundPlaying ( whichSound ) )
		{
			sys_playMultiSample ( whichSound, pan, loop );
			return true;
		}

	al_set_sample_instance_pan ( sound[whichSound].instance, pan );
	al_set_sample_instance_playmode ( sound[whichSound].instance, loop );
	//
	// ALLEGRO_SAMPLE_ID in place of null for sample ID
	al_play_sample_instance ( sound[whichSound].instance );

	return true;
}

//-------------------------------------------------------------------------
//
//Stop playing a sound - pass in sound index to get SAMPLE_ID
void sys_stopSound ( int whichSound )
//-------------------------------------------------------------------------
{
	if ( false == playSounds )
		return;

	al_stop_sample_instance ( sound[whichSound].instance );
}

//-------------------------------------------------------------------------
//
// Stop playing all sounds
void sys_stopAllSounds()
//--------------------------------------------------------------------------
{
	if ( false == playSounds )
		return;

	for ( int i = 0; i != NUM_SOUNDS; i++ )
		al_stop_sample_instance ( sound[i].instance );

	for ( int indexCount = 0; indexCount != as_numMultiSamples; indexCount++ )
	{
		al_stop_sample_instance( multiSounds[indexCount].instance );
	}

}

//-------------------------------------------------------------------------
//
// Play a sound from a external script
void sys_playSoundFromScript ( int whichSound, float pan, bool loopIt )
//-------------------------------------------------------------------------
{
	ALLEGRO_PLAYMODE	loopValue;

	if ( ( whichSound < 0 ) || ( whichSound > NUM_SOUNDS ) )
		{
			con_print ( true, false, "Error: Invalid sound index passed to sys_playSoundFromScript - [ %i ]", whichSound );
			return;
		}
	if ( loopIt == true )
		loopValue = ALLEGRO_PLAYMODE_LOOP;
	else
		loopValue = ALLEGRO_PLAYMODE_ONCE;

	sys_playSound ( whichSound, pan, loopValue );
}

//-------------------------------------------------------------------------
//
// Set volume (gain) for a sample - needs to be already playing
void sys_setVolume ( int whichSound, float volLevel )
//-------------------------------------------------------------------------
{
	if ( false == playSounds )
		return;

	if ( false == sys_isSoundPlaying ( whichSound ) )
		return;

	if ( false == al_set_sample_instance_gain ( sound[whichSound].instance, volLevel ) )
		{
			printf ( "Couldn't set sample gain\n" );
		}
}
