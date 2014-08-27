#include "pch.h"

#include "staticGeometry.h"


StaticGeometry::StaticGeometry(EntityManager* entityManager, RenderSystem* renderSystem, PhysicsManager* physicsManager )
{
	mEntityManager = entityManager;
	mRenderSystem = renderSystem;
	mPhysicsManager = physicsManager;
}


StaticGeometry::~StaticGeometry(void)
{
}

void StaticGeometry::addGeometry( float3 position, float3 size, float3 angle )
{
	// Create the components needed
	RenderComponent* rComp = new RenderComponent( mRenderSystem ); 
	PhysicsComponent* phyComp = new PhysicsComponent( mPhysicsManager );
	PositionComponent* posComp = new PositionComponent();

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
	phyComp->initialise( box, 0, position );

	// Set up graphics
	rComp->setAsBox( size.x,size.y,size.z );

	// Tell managers about new entity
	mEntityManager->addEntity( ent );
	mGeometryList.push_back(ent);

	mEntityManager->update( 0 );
}
