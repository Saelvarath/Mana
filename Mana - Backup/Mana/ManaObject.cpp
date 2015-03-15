#include "ManaObject.h"
#include <iostream>
#include <sstream>

/*
Air:	speed, 
Earth:	density, mass, solidity
Fire:	heat, light, energy
Water:	fluidity, health
*/

//$is max mana backwards? it should take *more* mana to affect discordant transformations
ManaObject::ManaObject()
{
	mana.air = 500;
	mana.aMax = 1000;
	mana.earth = 500;
	mana.eMax = 1000;
	mana.fire = 500;
	mana.fMax = 1000;
	mana.water = 500;
	mana.wMax = 1000;
}
ManaObject::ManaObject( Ogre::SceneNode* object, manaGauge* MG )
{
	subject = object;
	mana = *MG;
}
ManaObject::ManaObject( Ogre::SceneNode* object, unsigned int a, unsigned int aM, unsigned int e, unsigned int eM, 
												unsigned int f, unsigned int fM, unsigned int w, unsigned int wM )
{
	subject = object;

	mana.air = a;		mana.aMax = aM;
	mana.earth = e;		mana.eMax = eM;
	mana.fire = f;		mana.fMax = fM;
	mana.water = w;		mana.wMax = wM;
}
 
void ManaObject::addEffect( Effect *Ef, fourBoolean *activatedBy )
{
	std::pair< Effect*, fourBoolean* > *p = new std::pair< Effect*, fourBoolean* >( Ef, activatedBy );
	EffectList.push_front( *p );
}
void ManaObject::applyEffects( fourBoolean* transferedMana, bool givingMana )
{
	std::list< std::pair< Effect*, fourBoolean* > >::iterator i;
	//mana is coming in pre-culled for activity and availability
	for( i = EffectList.begin(); i != EffectList.end(); i++ )
	{
		i->first->applyEffect( subject, i->second, transferedMana, &mana, givingMana );
	}
}

//$place GUI and Particle code here so only one check is needed
												//activeElements
fourBoolean ManaObject::giveManaTo( ManaObject* target, fourBoolean* active, bool givingMana )
{
//						 element active  &&  giver has mana	 &&				target isn't full
	fourBoolean culled = {	( active->a  &&  mana.air	> 0  &&  target->mana.air   < target->mana.aMax ) ? 1 : 0, 
							( active->e  &&  mana.earth	> 0  &&  target->mana.earth < target->mana.eMax ) ? 1 : 0, 
							( active->f  &&  mana.fire	> 0  &&  target->mana.fire  < target->mana.fMax ) ? 1 : 0, 
							( active->w  &&  mana.water	> 0  &&  target->mana.water < target->mana.wMax ) ? 1 : 0};

	if ( culled.a + culled.e + culled.f + culled.w > 0 )
	{
		mana.air   -= culled.a;
		mana.earth -= culled.e;
		mana.fire  -= culled.f;
		mana.water -= culled.w;

		applyEffects( &culled, givingMana );
		target->takeMana( &culled );
		target->applyEffects( &culled, givingMana );
	}

	return culled;
}

void ManaObject::takeMana( fourBoolean* active )
{
	mana.air   += active->a;
	mana.earth += active->e;
	mana.fire  += active->f;
	mana.water += active->w;
}

void ManaObject::LogMana()
{
	//Ogre::String msg = "Name: Saelvarath";
//	msg += subject->getName();

	Ogre::String msg = "A: ";
	msg += Ogre::StringConverter::toString( mana.air );
	msg += "/";
	msg += Ogre::StringConverter::toString( mana.aMax );

	msg += ", E: ";
	msg += Ogre::StringConverter::toString( mana.earth );
	msg += "/";
	msg += Ogre::StringConverter::toString( mana.eMax );

	msg += ", F: ";
	msg += Ogre::StringConverter::toString( mana.fire );
	msg += "/";
	msg += Ogre::StringConverter::toString( mana.fMax );

	msg += ", W: ";
	msg += Ogre::StringConverter::toString( mana.water );
	msg += "/";
	msg += Ogre::StringConverter::toString( mana.wMax );

	Ogre::LogManager::getSingleton().logMessage( msg );
}