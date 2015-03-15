#include "SizeZ.h"

SizeZ* SizeZ::Instance = NULL;

SizeZ::SizeZ(){}

SizeZ* SizeZ::getInstance()
{
	if ( Instance == NULL )
	{
		Instance = new SizeZ();
	}
	return Instance;
}

void SizeZ::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add )
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
			Subject->setScale( stat.x, stat.y, stat.z + stat.z * ratio * CM );
		}
		else
		{
			Subject->setScale( stat.x, stat.y, stat.z - stat.z * ratio * CM );
		}
	}
}