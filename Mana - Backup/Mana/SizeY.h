#ifndef SIZEY_H
#define SIZEY_H

#include "Effect.h"

class SizeY : public Effect
{
protected:
	SizeY();

public:
	static SizeY* Instance;
	static SizeY* getInstance();
	void SizeY::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add = true );
	~SizeY();
};
#endif