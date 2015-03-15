#ifndef SIZEX_H
#define SIZEX_H

#include "Effect.h"

class SizeX : public Effect
{
protected:
	SizeX();

public:
	static SizeX* Instance;
	static SizeX* getInstance();
	virtual std::string getName(){ return std::string( "SizeX" );}
	void SizeX::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add = true );
	~SizeX();
};
#endif