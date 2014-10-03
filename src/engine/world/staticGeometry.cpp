#include "pch.h"

#include "common.h"
#include "staticGeometry.h"


StaticGeometry::StaticGeometry( Engine* engine )
{
	mEngine = engine;
}


StaticGeometry::~StaticGeometry(void)
{
}

Entity* StaticGeometry::addGeometry( float3 position, float3 size, float3 angle )
{
	// Create the components needed
	PositionComponent* posComp = new PositionComponent();
	RenderComponent* rComp = new RenderComponent( mEngine->getRenderSystem(), posComp ); 
	PhysicsComponent* phyComp = new PhysicsComponent( mEngine->getPhysicsManager(),posComp );


	// Create entity and register components
	Entity* ent = new Entity();
	ent->addComponent( rComp );
	ent->addComponent( phyComp );
	ent->addComponent( posComp );

	// Register components to listen to one another
	posComp->registerListener( rComp );
	phyComp->registerListener( posComp );

	// Set up physics
	btBoxShape* box = new btBoxShape( size/2 );
	phyComp->initialise( box, 0, position, (Quat::RotateZ(angle.z)*Quat::RotateY(angle.y)*Quat::RotateX(angle.x)) );
	posComp->setPosition(position);
	posComp->setOrientation((Quat::RotateZ(angle.z)*Quat::RotateY(angle.y)*Quat::RotateX(angle.x)));

	// Set up graphics
	rComp->setAsBox( size.x,size.y,size.z );

	// Tell managers about new entity
	mEngine->getEntityManager()->addEntity( ent );
	mGeometryList.push_back(ent);

	mEngine->getEntityManager()->update( 0 );

	// If host, send event so that clients create geometry too.
	if( mEngine->getNetworkSystem()->isHost() )
	{
		TransformEvent* te = new TransformEvent( EV_WORLD_CREATE_STATIC_BOX );
		te->mPosition = position;
		te->mOrientation = Quat::FromEulerXYX( angle.x,angle.y,angle.z );
		te->mSecondaryFloat = size;

		mEngine->getEventSystem()->dispatchEvent( te );
	}

	return ent;
}

void StaticGeometry::handle( Event* e )
{
	if( e->getEventType() == EV_WORLD_CREATE_STATIC_BOX )
	{
		TransformEvent* te = static_cast<TransformEvent*>(e);

		Entity* ent = this->addGeometry( te->mPosition, te->mSecondaryFloat, te->mOrientation.ToEulerXYX() );
		ent->addComponent( mNetworkSystem->getNetworkComponent(te->mGUID ));
	}
}