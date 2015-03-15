#include "CameraApp.h"
#include "BaseFrameListener.h"

void CameraApp::createScene()
{
	activeMana.a = 1;
	activeMana.e = 1;
	activeMana.f = 1;
	activeMana.w = 1;

	Player = ManaObject( charNode, 115, 1000, 115, 1000, 115, 1000, 115, 1000 );

	health = 1000;
	jumped = false;
	casting = false;
	StoryTime = true;

//initialize camera stuff
	camera = sceneMgr->createCamera( "MainCam" );

	Ogre::Viewport *vp = window->addViewport( camera );
	vp->setDimensions( 0.0f, 0.0f, 1.0f, 1.0f );
	camera->setAspectRatio( ( float )vp->getActualWidth() / ( float ) vp->getActualHeight() );
	camera->setFarClipDistance( 1000.0f );
	camera->setNearClipDistance( 5.0f );
	setActiveCamera( camera );

//that most useful of tools
	rayQuery = sceneMgr->createRayQuery( Ogre::Ray() );
	rayQuery->setSortByDistance( true );

//initializing GUI stuff
	Silver = new Gorilla::Silverback();
	Silver->loadAtlas( "GUISprites" );

	HUDScreen = Silver->createScreen( vp, "GUISprites" );
	HUDLayer = HUDScreen->createLayer( 0 );	//goes from 0-15

	HUDRect[0] = HUDLayer->createRectangle( 0, 0, 1600, 900 );			//tutorial, storyboard etc
	 
	/*__________
	|	A	|	|
	|_______| W	|
	|	|___|___|
	| F	|		|
	|___|___E___|*/		

	HUDRect[1] = HUDLayer->createRectangle( 0,    0,    105.5, 70 );	//Air
	HUDRect[2] = HUDLayer->createRectangle( 45,   80.5, 105.5, 70 );	//Earth
	HUDRect[3] = HUDLayer->createRectangle( 0,    45,   70,    105.5 );	//Fire
	HUDRect[4] = HUDLayer->createRectangle( 80.5, 0,    70,    105.5 );	//Water

	HUDRect[5] = HUDLayer->createRectangle( 70, 61, 10, 10 );	//Air indicator
	HUDRect[6] = HUDLayer->createRectangle( 70, 79, 10, 10 );	//Earth indicater
	HUDRect[7] = HUDLayer->createRectangle( 61, 70,	10, 10 );	//Fire indicator
	HUDRect[8] = HUDLayer->createRectangle( 79, 70, 10, 10 );	//Water indicator
	
	HUDRect[9] = HUDLayer->createRectangle( 795, 445, 10, 10 );	//crosshair //! resolution: 1600*900 -0.5*width*/

	//$add health indicator

	updateGUI();	

//$title screen

//$story/load screen

	loadLevel( "Cave2.scene" );
	//loadLevel( "Tree.scene" ); //Debug level
	//loadLevel( "Demo.scene" );
	//loadLevel( "SampleScene3.scene" );
	
//$pausescreen
}

void CameraApp::loadLevel( Ogre::String LvlName )
{
	sceneMgr->clearScene();
	manaMap.clear();

	//+Ogre::String grpName = LvlName.substr( 0, LvlName.length() - 6 );

	loader.parseDotScene( Ogre::String( LvlName ), Ogre::String( "General" ), sceneMgr );

	// THis call is necessary to ensure the destructor for the DotSceneLoader
	// does not delete the terrain group it created.  THis is a HACK!!
	terrainGrp = loader.takeTerrainGroup();

//reset ray query target
	rayTarget = NULL;
	manaTarget = NULL;
	
//initializing character data
	airTime = 0.001;

	charNode = sceneMgr->getRootSceneNode()->createChildSceneNode( "CharacterNode" );
	Ogre::Entity *sinbadEntity = sceneMgr->createEntity( "Sinbad", "Sinbad.mesh" );
	charNode->attachObject( sinbadEntity );

	aura = sceneMgr->createLight( "Aura" );
	aura->setType( Ogre::Light::LT_POINT );
	aura->setVisible( true );
	aura->setCastShadows( false );
	aura->setDiffuseColour( 1.0, 1.0, 1.0 );
	aura->setSpecularColour( 0.6, 0.6, 0.6 );
	aura->setAttenuation( 160, 1.0, 0.027, 0.0028 );
	//range		The absolute upper range of the light in world units
	//constant	The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation
	//linear	The linear factor in the attenuation formula: 1 means attenuate evenly over the distance
	//quadratic	The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula.
	charNode->attachObject( aura );/**/

	//$ find start position based upon hidden object within level || take control of the Sinbad placed within the level
	if ( LvlName == "Cave.scene" )//-
	{
		charNode->setPosition( -10, 5, -146 );
	}
	else if ( LvlName == "Cave2.scene" )
	{
		charNode->setPosition( 13, 6, -197 );
	}
	else if ( LvlName == "Tree.scene" )
	{
		charNode->setPosition( 0, 700, 0 );
	}
	else
	{
		charNode->setPosition( -100, 100, -140 ); 
	}
																	//Length
	bIdleState = sinbadEntity->getAnimationState( "IdleBase" );		//7.3333
	bIdleState->setLoop( true );
	tIdleState = sinbadEntity->getAnimationState( "IdleTop" );		//10
	tIdleState->setLoop( true );

	bWalkState = sinbadEntity->getAnimationState( "RunBase" );		//0.6667
	bWalkState->setLoop( true );
	tWalkState = sinbadEntity->getAnimationState( "RunTop" );		//0.6667
	tWalkState->setLoop( true );

	sJumpState = sinbadEntity->getAnimationState( "JumpStart" );	//0.1667
	sJumpState->setLoop( false );
	lJumpState = sinbadEntity->getAnimationState( "JumpLoop" );		//1
	lJumpState->setLoop( true );
	eJumpState = sinbadEntity->getAnimationState( "JumpEnd" );		//0.1667
	eJumpState->setLoop( false );
												//$DrawSwords		  0.3333
												//$SliceHoriz		  0.5
												//$SliceVerti		  0.5
  
//reset camera data
//$start behind character in any position
	Cam[0] = 50.0; //Radius
	Cam[1] = 20.0; //Y
	Cam[2] = 0.0;//3.14;  //Radians

	camNode = sceneMgr->getRootSceneNode()->createChildSceneNode( "CameraNode" );
	camNode->attachObject( camera );
	//$make this look over the closest shoulder and displace further from character when moved
	camera->setAutoTracking( true, charNode, Ogre::Vector3( 0, 3.5, 0 ) );

//initializing mana/particle stuff
	ManaDrainFX = new Ogre::ParticleSystem*[4];
	ManaDrainFX[0] = sceneMgr->createParticleSystem( "aDrain", "Mana/aDrain" );
	ManaDrainFX[1] = sceneMgr->createParticleSystem( "eDrain", "Mana/eDrain" );
	ManaDrainFX[2] = sceneMgr->createParticleSystem( "fDrain", "Mana/fDrain" );
	ManaDrainFX[3] = sceneMgr->createParticleSystem( "wDrain", "Mana/wDrain" );
	//+Ogre::ParticleSystem::createEmitterParticle( const String & emitterName );

	drainNode = sceneMgr->getRootSceneNode()->createChildSceneNode( "DrainFX" );
	for( int ps = 0; ps < 4; ps++ )
	{
		drainNode->attachObject( ManaDrainFX[ps] );
		ManaDrainFX[ps]->setEmitting( false );
	}	
	
	//$ make this light dimmer and lead the charact slightly, remove/reduce ambient light
	//+sceneMgr->setAmbientLight( Ogre::ColourValue( 0, 1, 1 ) );	
}

void CameraApp::createFrameListener()
{
	frameListener = new BaseFrameListener( this );
}

void CameraApp::createSceneManager()
{
	// get a pointer to the default base scene manager -- sufficient for our purposes
	sceneMgr = root->createSceneManager( Ogre::ST_EXTERIOR_CLOSE );
}

//
bool CameraApp::mousePressed	( const OIS::MouseEvent &arg, OIS::MouseButtonID id )	
{	
	if ( manaTarget == NULL && !StoryTime )
	{
		rayResult = FirstRayResult( Ogre::Ray( camNode->getPosition(), ( activeCamera->getOrientation() * -Ogre::Vector3::UNIT_Z ) ), manaTarget );
		if ( rayResult != NULL )
		{
			casting = true;
			rayTarget = rayResult->movable->getParentSceneNode();
			rayTarget->showBoundingBox( true );
		}
	}

	return OgreApp::mousePressed( arg, id );

}

bool CameraApp::mouseReleased	( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	casting = false;

	if ( rayTarget != NULL )
	{
		for( int ps = 0; ps <4; ps++ )
		{
			ManaDrainFX[ps]->setEmitting( false );
		}

		rayTarget->showBoundingBox( false );
		rayTarget = NULL;
		manaTarget = NULL;
	}

	return OgreApp::mouseReleased( arg, id );
}

//-bool CameraApp::mouseMoved	( const OIS::MouseEvent &arg )			{	return OgreApp::mouseMoved( arg );	}
bool CameraApp::keyPressed		( const OIS::KeyEvent &arg )
{
//JUMP
	if ( arg.key == OIS::KC_SPACE && !StoryTime )
	{
		( ( BaseFrameListener* )frameListener )->setHasMoved();
		jumped = true;
		sJumpState->setEnabled( true );

	}

	return OgreApp::keyPressed( arg );
}
bool CameraApp::keyReleased		( const OIS::KeyEvent &arg )
{
//MANA SELECTION

	if ( arg.key == OIS::KC_T )
	{
		StoryTime = !StoryTime;
		updateGUI();
	}

	if ( !StoryTime )
	{
		if ( arg.key == OIS::KC_1 )
		{
			++activeMana.a;
			updateGUI();
		}
		
		if ( arg.key == OIS::KC_2 )
		{
			++activeMana.e;
			updateGUI();
		}
		
		if ( arg.key == OIS::KC_3 )
		{
			++activeMana.f;
			updateGUI();
		}

		if ( arg.key == OIS::KC_4 )
		{
			++activeMana.w;
			updateGUI();
		}

	//JUMP
		if ( arg.key == OIS::KC_SPACE )
		{
			jumped = false;
		}
	}

//-LEVEL LOADER TESTING
	/*if ( arg.key == OIS::KC_J )
	{
		//loadLevel( "SampleScene3.scene" );
		//loadLevel( "Demo.scene" );
		//loadLevel( "Lab 4.scene" );
		//loadLevel( "Tree.scene" );
	}

	if ( arg.key == OIS::KC_K )
	{
		//loadLevel( "Tree2.scene" );
		//loadLevel( "Lab 4.scene" );
	}*/

	return OgreApp::keyReleased( arg );
}
//
Ogre::RaySceneQueryResultEntry* CameraApp::FirstRayResult( Ogre::Ray R, ManaObject*& MO)
{//returns first collision with AABB or NULL

	rayQuery->setRay( R );
	Ogre::RaySceneQueryResult &QueryResults = rayQuery->execute();
	Ogre::RaySceneQueryResult::iterator ResultIterator = QueryResults.begin();

	//compare() returns 0 when strings are equal, 0 evaluates to false.
	while ( ResultIterator != QueryResults.end() && ResultIterator->distance < 200 && MO == NULL )
	{
		if ( ResultIterator->movable->getMovableType() == "Entity" &&
			 R.getDirection().normalisedCopy().directionEquals( ( ResultIterator->movable->getParentNode()->getPosition() - charNode->getPosition() ).normalisedCopy() , Ogre::Radian( 0.524) ) ) //0.524 rads ~= 30 degs
		{
			 MO = getManaObject( (Ogre::String)ResultIterator->movable->getName() );
		}

		ResultIterator++;
	}
	//$ if next entity's centre of mass is closer that the current entity's return it instead.
	
	return MO == NULL ? NULL : &*--ResultIterator;
}


ManaObject* CameraApp::getManaObject( Ogre::String& name )
{
	if ( manaMap[name] )
	{
		return manaMap[name];
	}
	else
	{
		//$get scale and adjust manaGuage accordingly
		
		Ogre::String nSub = name.substr( 0, name.find_first_of( '#', 3 ) );
		ManaObject* newMO = NULL;

		if ( nSub == "Rock" )
		{
			manaGauge mG = {10, 20, 500, 1000, 50, 500, 20, 20};
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );

			fourBoolean* sz = new fourBoolean;
			sz->a = 0;	sz->e = 1;	sz->f = 0;	sz->w = 0;
			  newMO->addEffect( SizeX::getInstance(), sz );
			  newMO->addEffect( SizeY::getInstance(), sz );
			  newMO->addEffect( SizeZ::getInstance(), sz );

			//$ Air:	linked with Earth. when Air is up earth increases volume, otherwise it increases density
			//$ Earth:	adds mass. how depends on Air. when density is high stone sinks into the ground
			//$ Fire:	colour shift, glow red + permanent melt at high levels
			//$ Water:	smooth rock? limestone
		}
		else if ( nSub == "Stalactite" || nSub == "Stalagmite" )
		{
			manaGauge mG = { 15, 20, 400, 1000, 10, 500, 400, 1000 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );

			fourBoolean* rad = new fourBoolean;
			rad->a = 0;	rad->e = 1;	rad->f = 0;	rad->w = 0;
			  newMO->addEffect( SizeX::getInstance(), rad );
			  newMO->addEffect( SizeZ::getInstance(), rad );

			fourBoolean* len = new fourBoolean;
			len->a = 0;	len->e = 0;	len->f = 0;	len->w = 1;
				newMO->addEffect( SizeY::getInstance(), len );
			//tite
			//$ Earth:	at high levels of water start forming a stalagmite
			//$ Fire:	melt and drip down?
			//$ Water:	add particle for water dripping

			//mite
			//$ Water:	become wet/shiny?
			//$ Fire:	melt. droop?
		}
		else if ( nSub == "Stalactites" || nSub == "Stalagmites" )
		{
			manaGauge mG = { 15, 20, 400, 1000, 10, 500, 400, 1000 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );

			fourBoolean* len = new fourBoolean;
			len->a = 0;	len->e = 1;	len->f = 0;	len->w = 1;
			  newMO->addEffect( SizeY::getInstance(), len );

		}
		else if ( nSub == "Column" )
		{
			manaGauge mG = { 15, 20, 500, 1000, 10, 500, 200, 800 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );
			
			fourBoolean* rad = new fourBoolean;
			rad->a = 0;	rad->e = 1;	rad->f = 0;	rad->w = 0;
			  newMO->addEffect( SizeX::getInstance(), rad );
			  newMO->addEffect( SizeZ::getInstance(), rad );

			  //$link water and earth. too much/little water turns column into stalacite/mite pair and back again
		}
		else if ( nSub == "Ledge" )
		{
			manaGauge mG = { 10, 20, 500, 1000, 15, 500, 10, 500 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );
			
			fourBoolean* area = new fourBoolean;
			area->a = 0;	area->e = 1;	area->f = 0;	area->w = 0;
			  newMO->addEffect( SizeX::getInstance(), area );
			  newMO->addEffect( SizeZ::getInstance(), area );
		}
		else if ( nSub == "Roots" || nSub == "Mushroom" || nSub == "tree" )
		{
			manaGauge mG = { 400, 800, 400, 800, 150, 300, 400, 800 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );

			fourBoolean* wid = new fourBoolean;
			wid->a = 0;	wid->e = 1;	wid->f = 0;	wid->w = 1;
			  newMO->addEffect( SizeX::getInstance(), wid );
			  newMO->addEffect( SizeZ::getInstance(), wid );


			fourBoolean* hei = new fourBoolean;
			hei->a = 1;	hei->e = 0;	hei->f = 1;	hei->w = 0;
			  newMO->addEffect( SizeY::getInstance(), hei );

			//$ change/blend species of plant, cactus to banyan with mana
			//$ Air:	growYxz, animate growth cycle
			//$ Earth:	growXYZ, animate growth cycle
			//$ Fire:	lack of light - burning
			//$ Water:	wilted - drowned, 
		}
		else if ( nSub == "FirePit" )
		{
			manaGauge mG = { 250, 500, 10, 200, 800, 1000, 5, 10 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );
			
			//$ Air:	dark - bright. taller flame particle
			//$ Earth:	more sparks. fire lasts longer?
			//$ Fire:	dark - bright
			//$ Water:	bright - dark, more smoke
		}
		else if ( nSub == "Nub" )
		{
			manaGauge mG = { 10, 50, 300, 600, 10, 50, 10, 50 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );
			
			fourBoolean* siz = new fourBoolean;
			siz->a = 0;	siz->e = 1;	siz->f = 0;	siz->w = 0;
			  newMO->addEffect( SizeX::getInstance(), siz );
			  newMO->addEffect( SizeY::getInstance(), siz );
			  newMO->addEffect( SizeZ::getInstance(), siz );
		}
		else if (nSub == "LevelExit" )
		{
			manaGauge mG = { 50, 100, 50, 100, 50, 100, 50, 100 };
			newMO = new ManaObject( sceneMgr->getSceneNode( name ), &mG );
			
			fourBoolean* end = new fourBoolean;
			end->a = 1;	end->e = 0;	end->f = 1;	end->w = 0;
				newMO->addEffect( NextLvl::getInstance(), end );  
		}
		
		manaMap[name] = newMO;
		return newMO;
	}
}
//
Ogre::RaySceneQueryResultEntry* CameraApp::FirstGeoRayResult( Ogre::Ray R, Ogre::Real cullDist, Ogre::Real& distTo )
{
	rayQuery->setRay( R );
	Ogre::RaySceneQueryResult &QueryResults = rayQuery->execute();
	Ogre::RaySceneQueryResult::iterator ResultIterator = QueryResults.begin();
	
	while ( ResultIterator != QueryResults.end() && ResultIterator->distance <= cullDist ) //there are still results within range
	{
		if ( runGeoCheck( R, *ResultIterator, distTo, cullDist ) )
		{
			if( ++ResultIterator != QueryResults.end() && ResultIterator->distance < distTo )
			{
				Ogre::Real dist2;
				if ( runGeoCheck( R, *ResultIterator, dist2, distTo ) )
				{
					distTo = dist2;
					return &*ResultIterator;
				}
			}

			return &*(--ResultIterator);
		}
		else
		{
			distTo = NULL;
			ResultIterator++;
		}
	}
	
	return NULL;
}

bool CameraApp::runGeoCheck( const Ogre::Ray ray, const Ogre::RaySceneQueryResultEntry Entry, Ogre::Real &distTo, Ogre::Real cullDist )
{
	if ( Entry.movable->getMovableType() == "Entity" )
	{
		Ogre::Vector3 vec = ( Entry.movable->getParentNode()->getPosition() - ray.getOrigin() ).normalisedCopy();
		if ( ray.getDirection().normalisedCopy().directionEquals( vec, Ogre::Radian( 2.094 ) ) )
		{																		//120 * PI / 180
			return ( CheckGeometry( ray, Entry, distTo ) && distTo < cullDist );
		}
		
		//-Ogre::LogManager::getSingleton().logMessage( "skipped " + Entry.movable->getName() );
		return false;
	}
	else if ( Entry.movable->getMovableType() == "OgreTerrainNodeMovable" )
	{
		TerrainGroup::RayResult RR = terrainGrp->rayIntersects( ray, cullDist );

		distTo = ( RR.position - charNode->getPosition() ).length();
		return ( RR.hit && distTo < cullDist );
	}
	else if ( Entry.movable->getMovableType() != "Camera" && ray.getDirection().directionEquals( Entry.movable->getParentNode()->getPosition() - ray.getOrigin(), Ogre::Radian( Math::PI / 2 ) ) )
	{
		Ogre::LogManager::getSingleton().logMessage( "\t*********AABB HIT*********\n" );
		//?investigate if this is relevant anymore 
		distTo = Entry.distance;
		return true;
	}
	
	return false;
}

bool CameraApp::CheckGeometry( const Ogre::Ray ray, const Ogre::RaySceneQueryResultEntry query_result, Ogre::Real& distTo )
{
	Ogre::Real closest_distance = -1.0f;

	// only check this result if its a hit against an entity

	if ( ( query_result.movable != NULL ) && ( query_result.movable->getMovableType() == "Entity"  ) &&	query_result.movable->getName() != "Sinbad" )
	{
		// get the entity to check
		Ogre::Entity *pentity = static_cast<Ogre::Entity*>( query_result.movable );           
 
		// mesh data to retrieve
		std::pair<bool, Ogre::Real> hit;
		size_t index_count;
		size_t vertex_count;
		Ogre::Vector3 *vertices;
		unsigned long *indices;

		// get the mesh information
		getMeshInformation( pentity->getMesh(), vertex_count, vertices, index_count, indices,             
							pentity->getParentNode()->_getDerivedPosition(), 
							pentity->getParentNode()->_getDerivedOrientation(), 
							pentity->getParentNode()->_getDerivedScale() );
		
		// test for hitting individual triangles on the mesh
		//$ find a way to cut this down
		bool new_closest_found = false;
		
		for ( int i = 0; i < static_cast<int>( index_count ); i += 3 )
		{
			// check for a hit against this triangle
			hit = Ogre::Math::intersects( ray, 	vertices[indices[i]], 	vertices[indices[i+1]], 	
												vertices[indices[i+2]], 	true, 	false );
 
			// if it was a hit check if its the closest
			if ( hit.first )
			{
				if ( ( closest_distance < 0.0f ) || ( hit.second < closest_distance ) )
				{
					// this is the closest so far, save it off
					closest_distance = hit.second;
					new_closest_found = true;
				}
			}
		}
 
		// free the verticies and indicies memory
		delete[] vertices;
		delete[] indices;
 
		// if we found a new closest raycast for this object, update the
		// closest_result before moving on to the next object.
		/*if ( new_closest_found )
		{
			closest_result = ray.getPoint( closest_distance );               
		}*/
	}
	

	// return the result
	if ( closest_distance >= 0.0f )
	{								// raycast success
		distTo = closest_distance;
		return ( true );
	}
	else
	{								// raycast failed
		return ( false );
	} 
}


void CameraApp::getMeshInformation( const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices, const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale )
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
 
    vertex_count = index_count = 0;
 
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );
 
        // We only need to add the shared vertices once
        if ( submesh->useSharedVertices )
        {
            if ( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
 
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ( ( !submesh->useSharedVertices )||( submesh->useSharedVertices && !added_shared ) )
        {
            if ( submesh->useSharedVertices )
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic( Ogre::VES_POSITION );
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer( posElem->getSource() );
 
            unsigned char* vertex =
                static_cast<unsigned char*>( vbuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize() )
            {
                posElem->baseVertexPointerToElement( vertex, &pReal );
 
                Ogre::Vector3 pt( pReal[0], pReal[1], pReal[2] );
 
                vertices[current_offset + j] = ( orient * ( pt * scale ) ) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
        if ( ibuf.isNull() ) continue; // need to check if index buffer is valid ( which will be not if the mesh doesn't have triangles like a pointcloud )
 
        bool use32bitindexes = ( ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );
 
        unsigned long*  pLong = static_cast<unsigned long*>( ibuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
        unsigned short* pShort = reinterpret_cast<unsigned short*>( pLong );
 
 
        size_t offset = ( submesh->useSharedVertices )? shared_offset : current_offset;
        size_t index_start = index_data->indexStart;
        size_t last_index = numTris*3 + index_start;
 
        if ( use32bitindexes )
            for ( size_t k = index_start; k < last_index; ++k )
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset );
            }
 
        else
            for ( size_t k = index_start; k < last_index; ++k )
            {
                indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) +
                    static_cast<unsigned long>( offset );
            }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}

void CameraApp::updateGUI()
{
	if( !StoryTime )
	{
		HUDRect[0]->background_image( "none" );
		HUDRect[9]->background_image( "Crosshair" );

		manaGauge* PMG = Player.getGuage();

		int manaRatio[4];																		//GUI currently has 9 frames ( 0-8 )
			manaRatio[0] = ( int )( ( float )PMG->air   / ( float )PMG->aMax * 100.0 / 11.11 ); //100/9 = 11.1111111 -> new frame every 112 mana 
			manaRatio[1] = ( int )( ( float )PMG->earth / ( float )PMG->aMax * 100.0 / 11.11 );
			manaRatio[2] = ( int )( ( float )PMG->fire  / ( float )PMG->aMax * 100.0 / 11.11 );
			manaRatio[3] = ( int )( ( float )PMG->water / ( float )PMG->aMax * 100.0 / 11.11 );
		
		HUDRect[1]->background_image( "AM" + Ogre::StringConverter::toString( manaRatio[0] ) );
		HUDRect[2]->background_image( "EM" + Ogre::StringConverter::toString( manaRatio[1] ) );
		HUDRect[3]->background_image( "FM" + Ogre::StringConverter::toString( manaRatio[2] ) );
		HUDRect[4]->background_image( "WM" + Ogre::StringConverter::toString( manaRatio[3] ) );

		activeMana.a ?  HUDRect[5]->background_image( "Crosshair" ):
						HUDRect[5]->background_image( "none" );
		activeMana.e ?  HUDRect[6]->background_image( "Crosshair" ):
						HUDRect[6]->background_image( "none" );
		activeMana.f ?  HUDRect[7]->background_image( "Crosshair" ):
						HUDRect[7]->background_image( "none" );
		activeMana.w ?  HUDRect[8]->background_image( "Crosshair" ):
						HUDRect[8]->background_image( "none" );

	}
	else
	{
		for ( int g = 1; g < 10; g++ )
		{
			HUDRect[g]->background_image( "none" );
		}
		HUDRect[0]->background_image( "Tutorial" );
	}
}

//
void CameraApp::shutdownApp()
{
	sceneMgr->destroyQuery( rayQuery );
}

void CameraApp::addTimes( float t )
{
	if ( bIdleState->getEnabled() )
	{
		tIdleState->addTime( t );
		bIdleState->addTime( t );
	}
	else if ( bWalkState->getEnabled() )
	{
		tWalkState->addTime( t );
		bWalkState->addTime( t );
	}
	
	if ( lJumpState->getEnabled() )
	{
		lJumpState->addTime( t );
	}
	
	if ( sJumpState->getEnabled() )
	{
		if ( sJumpState->hasEnded() )
		{
			sJumpState->setEnabled( false );
			sJumpState->setTimePosition( 0 );
		}
		else
		{
			sJumpState->addTime( t );
		}
	}
	
	if ( eJumpState->getEnabled() )
	{
		if ( eJumpState->hasEnded() )
		{
			eJumpState->setEnabled( false );
			eJumpState->setTimePosition( 0 );
		}
		else
		{
			eJumpState->addTime( t );
		}
	}
}