#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include <string>

/**
* A sound buffer represents a collection of sound data which has been
* loaded into memory.  A buffer can be played through a sound source.
* This is an abstract superclass and subclasses will provide concrete implementations.
* @author R.N. Robson
*/
class SoundBuffer
{
public:
	/**
	* Construct an empty sound buffer.
	*/
	SoundBuffer(){}

	/**
	* This will perform whatever action is necessary to destroy the sound buffer.
	*/
	virtual void destroy() = 0;

	/**
	* Destructor which will call the destroy() method to do the actual work.
	*/ 
	~SoundBuffer()
	{
		destroy();
	}

	/**
	* Load a sound from a file into this buffer in memory.
	* @param fileName The name of the file to load.
	*/
	virtual void loadFile(std::string &fileName) = 0;
};

/**
* A sound source is a place in a scene from which sound emanates.  A source
* can be in motion and can move from one location to another.  A source represents
* the location and direction of a sound.  A source must be associated with a
* buffer of sound data before it can be played.  This is an abstract superclass
* and must be subclassed to create concrete classes.
* @author R.N. Robson
*/
class SoundSource
{
public:
	/**
	* Create a motionless sound source at a default position.
	*/
	SoundSource(){}

	/**
	* This will perform whatever action is necessary to destroy the sound source.
	*/
	virtual void destroy() = 0;

	/**
	* Destructor which will call the destroy() method to do the actual work.
	*/ 
	~SoundSource()
	{
		destroy();
	}

	/**
	* Set the position of the sound source in world coordinates.
	* @param x X coordinate
	* @param y Y coordinate
	* @param z Z coordinate
	*/
	virtual void setPosition(float x, float y, float z) = 0;

	/**
	* Set the velocity of the sound source.
	* @param x X coordinate
	* @param y Y coordinate
	* @param z Z coordinate
	*/
	virtual void setVelocity(float x, float y, float z) = 0;

	/**
	* Determine if the source will loop its sound or not.
	* @param loop If true, the sound will be repeated
	*/
	virtual void setLooping(bool loop) = 0;

	/**
	* Bind the sound source to a buffer of audio data that the source will play.
	* buf The sound buffer the source should play.
	*/
	virtual void bindBuffer(SoundBuffer &buf) = 0;

	/**
	* Play this sound.
	*/
	virtual void play() = 0;

	/**
	* Stop playing the sound.
	*/
	virtual void stop() = 0;

	/**
	* Pause playing the sound so that it can be resumed with the play method.
	*/
	virtual void pause() = 0;


};



/**
* This represents an idealized sound system and provides access to the
* sound system.  This is an abstract superclass and will be implemented 
* by concrete subclasses.
* @author R.N. Robson
*/
class SoundSystem
{
private:
	static SoundSystem					*instance;

	



public:
	/**
	* Construct a sound system ready for use
	*/
	SoundSystem(){}

	/**
	* Return an istance of the sound system which is a singleton.
	* @return A reference to the sole instance of the sound system.
	*/
	static  SoundSystem& getInstance();

	/**
	* Do whatever is necessary to shutdown the sound system.
	*/
	virtual void destroy(){};

	/**
	* Call destroy method to shut down the sound system.
	*/
	~SoundSystem()
	{
		destroy();
	}

	/**
	* Create a new Sound buffer and return it.
	* @return a newly created sound buffer.  The user is responsible for
	* deleting this buffer to avoid memory leaks.
	*/
	virtual SoundBuffer* createSoundBuffer() = 0;

	/**
	* Create a new sound source and return it.
	* @return a new sound source.  The user is responsible for deleting
	* this object when it is no longer needed.
	*/
	virtual SoundSource* createSoundSource() = 0;

	/**
	* Set the position of the listener.
	* @param x X coordinate
	* @param y Y coordinate
	* @param z Z coordinate
	*/
	virtual void setListenerPosition(float x, float y, float z) = 0;

	/**
	* Set the up direction of the listener.
	* @param x X coordinate of up vector
	* @param y Y coordinate of up vector
	* @param z Z coordinate of up vector
	*/
	virtual void setListenerOrientation(float x, float y, float z) = 0;
};

#endif