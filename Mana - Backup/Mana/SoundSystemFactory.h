#ifndef SOUND_SYSTEM_FACTORY_H
#define SOUND_SYSTEM_FACTORY_H

#include <string>
#include "OpenAlSoundSystem.h"

/**
* This is the factory which can create various implementations of the
* abstract SoundSystem class.
*/
class SoundSystemFactory
{
public:
	/**
	* Create an instance of the names sound system and return it.
	* @param nm name of sound system to create
	* @return the sound system requested or NULL if it cannot be created.
	*/
	static SoundSystem* createSoundSystem(std::string nm);
};

#endif