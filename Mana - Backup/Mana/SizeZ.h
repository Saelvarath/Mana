#ifndef SIZEZ_H
#define SIZEZ_H

#include "Effect.h"

class SizeZ : public Effect
{
protected:
	SizeZ();

public:
	static SizeZ* Instance;
	static SizeZ* getInstance();
	void SizeZ::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add = true );
	~SizeZ();
};
#endif