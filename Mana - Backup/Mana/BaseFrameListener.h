#ifndef BASE_FRAME_LISTENER_H
#define BASE_FRAME_LISTENER_H

#include "OgreFrameListener.h"
#include "CameraApp.h"
//-#include <OIS.h>

using namespace Ogre;
using namespace OIS;

class BaseFrameListener : public FrameListener 
{
friend class NextLvl;
public:
	// ctor/dtor
	BaseFrameListener( OgreApp *ap )
	{
		
		keepRendering = true;
		
		tm = 0.0f;
		app = ( CameraApp* )ap;
		lastAng = 0.0;
		C2CRot = Ogre::Radian( 0.051 );
		
		frameNum = 0;
		hasMoved = false;
		moving = false;
	}
	virtual ~BaseFrameListener() {}

	// We will provide some meat to this method override
	virtual bool frameStarted( const FrameEvent &evt );

	// This little puppy will process the input events
	virtual bool frameEnded( const FrameEvent &evt );

	// handle the keyboard input
	virtual void processKeyboardInput( const FrameEvent &evt );

	// handle the mouse input
	virtual void processMouseInput();

	// call this to terminate the application
	void terminateApp();

	void setHasMoved() {	hasMoved = true;	}

private:
	Ogre::Vector3 charPos;

	unsigned int frameNum : 1;

	Ogre::Vector3 moveVect;
	bool moving;
	bool hasMoved; //need to do a full terrain clamp?

	Ogre::Radian lastAng;
	Ogre::Radian C2CRot;
	Ogre::RaySceneQueryResultEntry* NY;

	static bool keepRendering;
	float tm;

	CameraApp *app;
};

#endif