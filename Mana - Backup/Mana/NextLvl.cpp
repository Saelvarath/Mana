#include "NextLvl.h"
#include "BaseFrameListener.h"

NextLvl* NextLvl::Instance = NULL;

NextLvl::NextLvl(){}

NextLvl* NextLvl::getInstance()
{
	if ( Instance == NULL )
	{
		Instance = new NextLvl();
	}
	return Instance;
}

void NextLvl::applyEffect( Ogre::SceneNode* Subject, fourBoolean* releventMana, fourBoolean* incomingMana, manaGauge* m, bool add )
{
	int total = m->air * releventMana->a + 
				m->earth* releventMana->e + 
				m->fire * releventMana->f + 
				m->water * releventMana->w;

	if ( !add && total == 0)
	{
		//+cameraApp::nextLevel( "" );
		BaseFrameListener::keepRendering = false;
	}
	else if ( add )
	{
		int max =	m->aMax * releventMana->a + 
					m->eMax * releventMana->e + 
					m->fMax * releventMana->f + 
					m->wMax * releventMana->w;

		if ( total == max )
		{
			//+cameraApp::nextLevel( "" );
			BaseFrameListener::keepRendering = false;
		}
		
	}
}