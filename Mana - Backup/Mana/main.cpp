//			comment
//!			important comment
//?			issue to be investigated

//-			code to be removed once no longer relevant

//$			features to be moddified or implemented in the future
//+			code to be re implemented later (usually removed for testing purposes or was buggy/taking to much time)
//#			known bug to be fixed

#include "CameraApp.h"

#if defined( WIN32 )
#	include <windows.h>
#endif

#if defined ( WIN32 )
INT WINAPI WinMain ( 
					HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine, 
					int nCmdShow )
#else
int main ( int argc, char *argv[] )
#endif
{
	CameraApp *app = 0;
	try 
	{
		app = new CameraApp( NULL, true );
		app->constructObject();
		app->getRoot()->startRendering();

	}
	catch ( Ogre::Exception &e ) 
	{
		//-ErrorDialog *ed = PlatformManager::getSingleton().createErrorDialog();
		//-ed->display( e.getFullDescription() );
		Ogre::LogManager::getSingleton().logMessage( e.getFullDescription() );
	}

	if ( app ) 
	{
		app->shutdown();
		delete app;
	}
	return 0;
}