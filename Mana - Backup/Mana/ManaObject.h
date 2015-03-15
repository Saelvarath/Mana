#ifndef MANA_OBJECT_H
#define MANA_OBJECT_H

//-#include <Ogre.h>
#include <list>
//-#include "Effect.h"
#include <utility>
#include "SizeX.h"
#include "SizeY.h"
#include "SizeZ.h"
#include "NextLvl.h"

class ManaObject
{
	manaGauge mana;
	Ogre::SceneNode* subject;
	std::list< std::pair< Effect*, fourBoolean* > > EffectList;

public:

	ManaObject::ManaObject();
	ManaObject::ManaObject( Ogre::SceneNode* object, manaGauge* mG );
	ManaObject::ManaObject( Ogre::SceneNode* object, unsigned int a = 500, unsigned int aM = 1000, unsigned int e = 500, unsigned int eM = 1000, 
													 unsigned int f = 500, unsigned int fM = 1000, unsigned int w = 500, unsigned int wM = 1000 );

	void ManaObject::addEffect( Effect* Ef, fourBoolean* activatedBy );
	void ManaObject::applyEffects( fourBoolean* transferedMana, bool givingMana );

	fourBoolean ManaObject::giveManaTo( ManaObject* target, fourBoolean* active, bool givingMana );
	void ManaObject::takeMana( fourBoolean* active );

	void ManaObject::LogMana();

	manaGauge* getGuage(){	return &mana;	}
};
#endif