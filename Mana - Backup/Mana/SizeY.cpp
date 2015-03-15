#include "SizeY.h"

SizeY* SizeY::Instance = NULL;

SizeY::SizeY(){}

SizeY* SizeY::getInstance()
{
	if ( Instance == NULL )
	{
		Instance = new SizeY();
	}
	return Instance;
}

void SizeY::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add )
{
	fourBoolean culledMana = {	( releventMana->a && incomingMana->a ) ? 1 : 0, 
								( releventMana->e && incomingMana->e ) ? 1 : 0, 
								( releventMana->f && incomingMana->f ) ? 1 : 0, 
								( releventMana->w && incomingMana->w ) ? 1 : 0	};

	int CM = culledMana.a + culledMana.e + culledMana.f + culledMana.w;

	// as long as they have one in common
	if ( CM ) 
	{
		Ogre::Vector3 stat = Subject->getScale();
		
		//how much change each point of mana is worth									//total mana possibly affecting the change
		float ratio = 1.0 / ( ( releventMana->a * m->aMax ) + ( releventMana->e * m->eMax ) + ( releventMana->f * m->fMax ) + ( releventMana->w * m->wMax ) );

		if ( add )
		{
			Subject->setScale( stat.x, stat.y + ( stat.y * ratio * CM ), stat.z );
		}
		else
		{
			Subject->setScale( stat.x, stat.y - ( stat.y * ratio * CM ), stat.z );
		}
	}
}