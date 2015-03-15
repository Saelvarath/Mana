#include "SizeX.h"

SizeX* SizeX::Instance = NULL;

SizeX::SizeX(){}

SizeX* SizeX::getInstance()
{
	if ( Instance == NULL )
	{
		Instance = new SizeX();
	}
	return Instance;
}

void SizeX::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add )
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
			Subject->setScale( stat.x + stat.x * ratio * CM, stat.y, stat.z );
		}
		else
		{
			Subject->setScale( stat.x - stat.x * ratio * CM, stat.y, stat.z );
		}
	}
}