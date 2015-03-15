#ifndef NextLVL_H
#define NextLVL_H

#include "Effect.h"

class NextLvl : public Effect
{
protected:
	NextLvl();
//	friend void CameraApp::loadLevel( Ogre::String lvlName );

public:
	static NextLvl* Instance;
	static NextLvl* getInstance();
	virtual std::string getName(){ return std::string( "NextLvl" );}
	void NextLvl::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add = true );
	~NextLvl();
};
#endif