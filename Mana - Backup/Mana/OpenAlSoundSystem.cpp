#include "OpenAlSoundSystem.h"
#include "BasicTypes.h"

SoundSystem* SoundSystem::instance = NULL;

void OpenAlSoundBuffer::destroy()
{
	if(bufferName)
	{
		alDeleteBuffers(1, &bufferName);
		bufferName = 0;
	}
}

void OpenAlSoundBuffer::loadFile(std::string &fileName)
{
	ALenum fmt;
	ALsizei size, freq;
	ALvoid *data;
	ALboolean loop;

	if(bufferName == 0)
	{
		alGenBuffers(1, &bufferName);
		if(alGetError() != AL_NO_ERROR)
		{
			throw Exception(std::string(alGetString(alGetError() )),
				std::string(__FILE__), __LINE__);
		}
	}

	alutLoadWAVFile( (ALbyte*)fileName.c_str(), &fmt, &data, &size, &freq, &loop);
    alBufferData(bufferName, fmt, data, size, freq);
    alutUnloadWAV(fmt, data, size, freq);

	if(alGetError() != AL_NO_ERROR)
	{
		throw Exception(std::string(alGetString(alGetError() )),
			std::string(__FILE__), __LINE__);
	}
}

void OpenAlSoundSource::destroy()
{
	if(source)
	{
		alDeleteSources(1, &source);
		source = 0;
	}
}

void OpenAlSoundSource::setPosition(float x, float y, float z)
{
	alSource3f(source, AL_POSITION, x, y, z);
}

void OpenAlSoundSource::setLooping(bool loop)
{
	alSourcei(source, AL_LOOPING, loop);
}

void OpenAlSoundSource::setVelocity(float x, float y, float z)
{
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void OpenAlSoundSource::bindBuffer(SoundBuffer &buf)
{
	if(! source)
	{
		alGenSources(1, &source);
	}
	OpenAlSoundBuffer &b = (OpenAlSoundBuffer&)buf;
	alSourcei(source, AL_BUFFER, b.bufferName);
	if(alGetError() != AL_NO_ERROR)
	{
		throw Exception(std::string(alGetString(alGetError() )),
			std::string(__FILE__), __LINE__);
	}
	bound = true;
}

void OpenAlSoundSource::play()
{
	if(source && bound)
	{
		alSourcePlay(source);
	}
}

void OpenAlSoundSource::pause()
{
	if(source && bound)
	{
		alSourcePause(source);
	}
}

void OpenAlSoundSource::stop()
{
	if(source && bound)
	{
		alSourceStop(source);
	}
}


OpenAlSoundSystem::OpenAlSoundSystem()
{
	initialized = false;
	alutInit(NULL, 0);
	if(alGetError() != AL_NO_ERROR)
	{
		throw Exception(std::string(alGetString(alGetError() )),
			std::string(__FILE__), __LINE__);
	}
	initialized = true;
	xpos = ypos = zpos = 0.0;
}

void OpenAlSoundSystem::destroy()
{
	if(initialized)
	{
		alutExit();
		initialized = false;
	}
}

SoundBuffer* OpenAlSoundSystem::createSoundBuffer()
{
	return new OpenAlSoundBuffer();
}

SoundSource* OpenAlSoundSystem::createSoundSource()
{
	return new OpenAlSoundSource();
}

void OpenAlSoundSystem::setListenerPosition(float x, float y, float z)
{
	if(initialized)
	{
		alListener3f(AL_POSITION, x, y, z);
		xpos = x;
		ypos = y;
		zpos = z;
	}
}

/*
void OpenAlSoundSystem::setListenerVelocity(float x, float y, float z)
{
	if(initialized)
	{
		alListener3f(AL_VELOCITY, x, y, z);
	}
}
*/

void OpenAlSoundSystem::setListenerOrientation(float x, float y, float z)
{
	float orient[] = {xpos, ypos, zpos, x, y, z};
	if(initialized)
	{
		alListenerfv(AL_ORIENTATION, orient);
	}
}