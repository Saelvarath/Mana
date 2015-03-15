#ifndef GUI_APP_H
#define GUI_APP_H

#include "OgreApp.h"
#include "DotSceneLoader.h"
#include "ManaObject.h"
#include <OgreSceneQuery.h>
#include <hash_map>
#include "Gorilla.h"
#include <Terrain\OgreTerrainGroup.h>

class CameraApp: public OgreApp
{
//+friend NextLvl;
private:
	Ogre::TerrainGroup* terrainGrp;

	//+enum gameState {} //$ title, options, story, play, pause

// Character Data
	Ogre::Light* aura;
	Ogre::SceneNode* charNode;
	ManaObject Player;

	fourBoolean activeMana; //Mana the Player is currently using. denoted as "fourBoolean* active" in methods
	double airTime;
	bool jumped;
	bool casting;
	bool StoryTime;
	int health;

	Ogre::RaySceneQuery* rayQuery;

//Camera Data
	Ogre::Camera* camera;
	Ogre::SceneNode* camNode;

	float Cam[3];// radius, y, angle( in Rads )

//mana/particle system stuff
	stdext::hash_map<Ogre::String, ManaObject*> manaMap;
	Ogre::ParticleSystem** ManaDrainFX;
	Ogre::SceneNode *drainNode;

	DotSceneLoader loader;	

public:
//GUI Stuff
	Gorilla::Silverback* Silver;
	Gorilla::Rectangle* HUDRect[10];

	Gorilla::Screen* HUDScreen;
	Gorilla::Layer* HUDLayer;
	

	Ogre::RaySceneQueryResultEntry* rayResult;
	Ogre::SceneNode* rayTarget;
	ManaObject* manaTarget;

	CameraApp( Ogre::FrameListener *listener, bool useOIS ) : OgreApp( listener, useOIS ), camNode( NULL ){ }

	Ogre::AnimationState *bIdleState, *tIdleState, *bWalkState, *tWalkState, *sJumpState, *lJumpState, *eJumpState;

	virtual void createScene();

	virtual void createFrameListener();

	virtual void createSceneManager();

	virtual void shutdownApp();

//Methods for OIS::KeyListener
	virtual bool keyPressed		( const OIS::KeyEvent &arg );
	virtual bool keyReleased	( const OIS::KeyEvent &arg );
//-	virtual bool mouseMoved		( const OIS::MouseEvent &arg );
    virtual bool mousePressed	( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased	( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void addTimes( Ogre::Real );

//find or create a mana object for the object named
	ManaObject* CameraApp::getManaObject( Ogre::String& name );
	
//returns the first valid result of a raySceneQuery
	Ogre::RaySceneQueryResultEntry* CameraApp::FirstRayResult( Ogre::Ray R, ManaObject*& MO );

//returns object of first collision with Geometry	
	Ogre::RaySceneQueryResultEntry* CameraApp::FirstGeoRayResult( Ogre::Ray R, Ogre::Real cullDist, Ogre::Real &distTo );

	bool runGeoCheck( const Ogre::Ray ray, const Ogre::RaySceneQueryResultEntry query_result, Ogre::Real &distTo, Ogre::Real cullDist);

// raycast from a point into the scene. returns success or failure. on success the point is returned in the result.		Code modified from: http://www.ogre3d.org/tikiwiki/Raycasting+to+the+polygon+level
	bool CameraApp::CheckGeometry( const Ogre::Ray ray, const Ogre::RaySceneQueryResultEntry query_result, Ogre::Real &distTo );

// Get the mesh information for the given mesh.		Code found: www.ogre3d.org/wiki/index.php/RetrieveVertexData
	void CameraApp::getMeshInformation( const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices,
										const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale );

	
	Ogre::SceneNode* getCharNode()						{	return charNode;	}
	fourBoolean*	 CameraApp::getActiveElements()		{	return &activeMana;	}
	ManaObject*		 CameraApp::getPlayerMO()			{	return &Player;		}

	double	getAirTime()			{	return airTime;	}
	void	setAirTime( double a )	{	airTime = a;	}
	void	incAirTime( double i )	{	airTime += i;	}

	bool	getJumped()				{	return jumped;	}
	void	setJumped( bool j )		{	jumped = j;		}

	bool	isCasting()				{	return casting;	}
	//void	setCasting( bool c )	{	casting = c;	}

	bool	isStoryTime()			{	return StoryTime;	}

	int		getHealth()				{	return health;	}
	void	setHealth( int h )		{	health = h;		}
	void	incHealth( int h )		{	health -= h;
										/*Ogre::LogManager::getSingleton().logMessage( "health: " + health );*/	}
	
	Ogre::SceneNode* getCamNode()	{	return camNode;	}
	float* getCP()					{	return Cam;	}
	void camRad( float increment )	{	Cam[0] += increment;	}
	void camHei( float increment )	{	Cam[1] += increment;	}
	void camRot( float increment )	{	Cam[2] += increment;	}

	stdext::hash_map<Ogre::String, ManaObject*> CameraApp::getHash(){	return manaMap;		}

	Ogre::SceneNode*		CameraApp::getDrainNode()	{	return drainNode;	}
	Ogre::ParticleSystem**	CameraApp::getDrainFX()		{	return ManaDrainFX;	}
	//Ogre::SceneNode*		CameraApp::getRayTarget()	{	return &*rayTarget;	}

	void CameraApp::updateGUI();

	void CameraApp::loadLevel( Ogre::String lvlName );
};
#endif