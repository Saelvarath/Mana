#ifndef EFFECT_H
#define EFFECT_H

#include <Ogre.h>
#include <ostream>

/*
	//all effects go from 0 to maxMana
	//$ include how many properties are being affected by each mana in calculating effetiveness

___Effect______________________Max_________________
	rotate x, y, z				360 degrees
	translate x, y, z				only used to keep object centred //$include this in scale
	scale x, y, z					*0.5 - *2
	colour r, g, b, a				0 - ( 1||255 )
	+/- lighting				brightness?
	+/- particle effects		#?, lifespan?, colour?, 
	switch to new animation		maxMana
	+/- animation frames		0-maxframe#
	more as they come.

	Swap Level
	Storyboard
*/

struct fourBoolean
{
	unsigned int a : 1;
	unsigned int e : 1;
	unsigned int f : 1;
	unsigned int w : 1;
};

struct manaGauge
{
	unsigned int air, aMax, earth, eMax, fire, fMax, water, wMax;
};

class Effect
{
friend class manaObject;
public:
	virtual std::string getName(){return std::string( "Effect" );}
	friend std::ostream& operator<<( std::ostream &os, Effect &ef );

	//$ make ratio equations more complex to account for less variable mana ( 0-25 as opposed to 0-1000 )?
	//$ take how many effects an element is involved with into account
	virtual void Effect::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add = true ) = 0;
};
#endif