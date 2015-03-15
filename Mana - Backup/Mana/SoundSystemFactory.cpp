#include "SoundSystemFactory.h"

SoundSystem* SoundSystemFactory::createSoundSystem(std::string nm)
{
	SoundSystem* result = NULL;
	if(nm == std::string("openAlSoundSystem") )
	{
		result = new OpenAlSoundSystem();
	}
	return result;
}