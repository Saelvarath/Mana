#include "BaseFrameListener.h"
#include <OgreMath.h>
#include <stdio.h>

bool BaseFrameListener::keepRendering = true;

bool BaseFrameListener::frameStarted( const FrameEvent &evt )
{
	tm += evt.timeSinceLastFrame;

	//Character Animations
	bool startOrEndJumpPlaying = ( app->sJumpState->getEnabled() || app->eJumpState->getEnabled() ); //if either animation is playing, then true

	app->lJumpState->setEnabled( false );
	app->bWalkState->setEnabled( false );
	app->tWalkState->setEnabled( false );
	app->bIdleState->setEnabled( false );
	app->tIdleState->setEnabled( false );

	if ( !startOrEndJumpPlaying )
	{
		if ( app->getAirTime() > 0.2 )
		{
			app->lJumpState->setEnabled( true ); 
		}
		else if ( moving )
		{
			app->bWalkState->setEnabled( true );
			app->tWalkState->setEnabled( true );
		}
		else
		{
			app->bIdleState->setEnabled( true );
			app->tIdleState->setEnabled( true );
		}		
	}

	app->addTimes( evt.timeSinceLastFrame );

	//Charater Movement
	//$don't allow control while jumping?
	if ( moving )
	{
		hasMoved = true;

		moveVect.normalise();
		moveVect /= 3;

	//COLLISION DETECTION
		Ogre::Ray qr_ray = Ogre::Ray( charPos, moveVect );
		Ogre::Real hitDist;
		Ogre::RaySceneQueryResultEntry* Coll;
		//Sinbad's depth (4.8) / 2 + displacement = 2.7
		Coll = app->FirstGeoRayResult( qr_ray, 2.7, hitDist );

		if ( Coll != NULL )//will hit something this frame
		{		
			moving = false;
			hasMoved = false;
			moveVect = Ogre::Vector3::ZERO;
		}
		else
		{
			app->getCharNode()->translate( moveVect );
			
			//ROTATION	
			Ogre::Vector3 cmV = app->getActiveCamera()->getOrientation().zAxis();
				cmV.y = 0; cmV.normalise();
			Ogre::Radian ang = cmV.angleBetween( app->getCharNode()->getOrientation().zAxis().normalisedCopy() );
			//a•b = |a||b|cosθ in radians
			
			if ( ang < lastAng ) // angle getting larger
			{
				C2CRot *= -1; //C2CRot ~ 3º
			}

			lastAng = ang;

			if ( ang >= Ogre::Radian( 3.0892 ) ) //ang < 3º
			{	
				app->getCharNode()->setOrientation( Ogre::Quaternion( Ogre::Radian( app->getCP()[2] ), Ogre::Vector3( 0, -1, 0 ) ) );
			}
			else if ( ang <= Ogre::Radian( 0.1745 ) ) //ang > 170º
			{
				moveVect *= -1;
			}
			else
			{
				app->getCharNode()->yaw( C2CRot );
			}
		}
	}

	moveVect = Ogre::Vector3::ZERO;
	moving = false;

//Calculate Camera Position
	float *CP = app->getCP();  // radius, y, angle( in Rads )
	app->getCamNode()->setPosition( charPos.x + ( CP[0] * sin( CP[2] ) ), 
									charPos.y +   CP[1], 
									charPos.z + ( CP[0] * -cos( CP[2] ) ) );

//TERRAIN CLAMP
	// d= ( at^2 )/2, y = gt^2/2 + Vi*T
	Ogre::Real disp = ( 9.8 * app->getAirTime() * app->getAirTime() / 2 );
	if ( app->getJumped() ) {	disp -= 0.4;	}

	if (  hasMoved || app->getAirTime() > 0.0 || app->isCasting() )
	{
		charPos = app->getCharNode()->getPosition();
		Ogre::Ray down = Ogre::Ray( charPos, -Ogre::Vector3::UNIT_Y );

		//Sinbad Height: 9.11. Half Height 5.055 || 4.055
		Ogre::Real hitDist = 0.0;

		NY = ++frameNum ? NULL : app->FirstGeoRayResult( down, 5.05 + disp, hitDist ); //#doesn't matter for small falls but larger ones can skip

		if ( NY != NULL )// you are going to hit something this frame
		{
			hasMoved = false;
			app->getCharNode()->translate( 0.0, 5.05 - hitDist, 0 );
			app->setJumped( false );

			if ( app->getAirTime() > 0.2 && !app->sJumpState->getEnabled() )
			{
				app->eJumpState->setEnabled( true );
			}

			//$FALL DAMAGE
			// a human's terminal velocity is 185km/h to 337km/h depending on posture. therefore ~3-8 seconds to reach terminal velocity
			//? how fast can a human  hit the ground without injury?
			// if done correctly a human can consistantly fall ~5m without injury. it takes ~1 second to fall that far
			if ( app->getAirTime() > 0.7 )//fall will hurt you
			{	
				double test = app->getAirTime();
				app->incHealth( (int) ( app->getAirTime() * 150 ) );
				//basicly means if the fall isn't going to do over 100 damage don't apply it
			}	

			app->setAirTime( 0.0 );					
		}
		else //you're still falling
		{
			app->getCharNode()->translate( 0, -disp, 0 );//temp
			app->incAirTime( evt.timeSinceLastFrame );

			if ( app->getAirTime () >= 5.0 ) //if you've fallen for 3-7 seconds you've likely fallen off the map
			{
				app->setHealth( 0 );
			}
		}
	}

	return true;
}


void BaseFrameListener::processKeyboardInput( const FrameEvent &evt )
{
/*Default orientation is -z
http://www.ogre3d.org/tikiwiki/display1921 */
		
	if ( app->getKeyboard()->isKeyDown( OIS::KC_ESCAPE ) )
	{
		keepRendering = false;
		return;
	}

//MOVEMENT KEYS
	if ( app->getKeyboard()->isKeyDown( OIS::KC_W ) )
	{
		moveVect += app->getCharNode()->getOrientation().zAxis();
		moving = true;
	}
	if ( app->getKeyboard()->isKeyDown( OIS::KC_S ) )
	{
		moveVect -= app->getCharNode()->getOrientation().zAxis();
		moving = true;
	}

	if ( app->getKeyboard()->isKeyDown( OIS::KC_A ) )
	{
		moveVect += app->getCharNode()->getOrientation().xAxis();
		moving = true;
	}
	if ( app->getKeyboard()->isKeyDown( OIS::KC_D ) )
	{
		moveVect -= app->getCharNode()->getOrientation().xAxis();
		moving = true;
	}
}

void BaseFrameListener::processMouseInput()
{
	if ( !app->isStoryTime() )
	{
	//CAMERA
		OIS::MouseState MS = app->getMouse()->getMouseState();
		app->camRot(0.0085 * MS.X.rel);	// .5 degree / pixel
		app->camHei(MS.Y.rel / 3);		//$ limit displacement
		app->camRad( MS.Z.rel / 25 );	//$ limit displacement

	//MANA
		//$re-write the particle file for initial values
		/*	quota = targ->mana.a/e/f/w;
			emitter Box
			{ 
				area width/height/depth	 = AABB
				calculate deviance angle
				calculate lifetime
			}*/

		if ( app->rayTarget != NULL && ( MS.buttonDown( OIS::MB_Left ) || MS.buttonDown( OIS::MB_Right ) ) )
		{
			fourBoolean AM;
			
			if ( MS.buttonDown( OIS::MB_Left ) )
			{//GIVE MANA TO TARGET
				//$if ( too close )	{	move back;	}
				AM = app->getPlayerMO()->giveManaTo( app->manaTarget, app->getActiveElements(), true );

				/*Ogre::Vector3 V = app->rayTarget->getPosition() - app->getCharNode()->getPosition();
				Ogre::Ray R		= Ogre::Ray( app->getCharNode()->getPosition(), V );
				Ogre::Real dst;
				if ( app->runGeoCheck( R, app->rayResult, dst, 2.8 ) )
				{
					Ogre::LogManager::getSingleton().logMessage( app->rayResult->movable->getName() + ", " + Ogre::StringConverter::toString( dst ) );
					moving = true;
					moveVect = -V.normalisedCopy();
				}*/

				app->getDrainNode()->setPosition( app->getCharNode()->getPosition() ); 
				app->getDrainNode()->setAutoTracking( true, app->rayTarget );
			}
			else if ( MS.buttonDown( OIS::MB_Right ) )
			{//TAKE MANA FROM TARGET
				AM = app->manaTarget->giveManaTo( app->getPlayerMO(), app->getActiveElements(), false );

				app->getDrainNode()->setPosition( app->rayTarget->getPosition() ); 
				app->getDrainNode()->setAutoTracking( true, app->getCharNode() );
			}

			app->updateGUI();
					
			app->getDrainFX()[0]->setEmitting( AM.a ? true : false );
			app->getDrainFX()[1]->setEmitting( AM.e ? true : false );
			app->getDrainFX()[2]->setEmitting( AM.f ? true : false );
			app->getDrainFX()[3]->setEmitting( AM.w ? true : false );
		}
	}
}

bool BaseFrameListener::frameEnded( const FrameEvent &evt )
{
	// grab the keyboard & mouse state
	app->getMouse()->capture();
	app->getKeyboard()->capture();

	processMouseInput();
	processKeyboardInput( evt );
	

	keepRendering = keepRendering && app->getHealth() > 0;

	/*if ( ! keepRendering )
	{
		//OGRE_APP.shutdown();
	}*/

	return keepRendering;
}

void BaseFrameListener::terminateApp()
{
	keepRendering = false;
}