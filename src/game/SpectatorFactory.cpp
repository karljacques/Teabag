#include "pch.h"
#include "SpectatorFactory.h"

SpectatorFactory::SpectatorFactory( )
{
	// Create a spectator
	EntID ent = entitysys::createEntity();

	TransformComponent* trans = entitysys::createComponent<TransformComponent>(ent);
	entitysys::getByID(ent)->addComponent(trans);

	MotionComponent* motion = entitysys::createComponent<MotionComponent>(ent);
	entitysys::getByID(ent)->addComponent(motion);

	SpectatorComponent* spec = entitysys::createComponent<SpectatorComponent>(ent);
	entitysys::getByID(ent)->addComponent(spec);

	PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(ent);
	dynamic_cast<PhysicsManager*>(entitysys::getComponentManager(phys))->initComponent(phys,new btSphereShape( 0.25f ) , 10);
	phys->body->setFriction(0);
	phys->body->setGravity(float3(0,0,0)); // Disable gravity on a spectator
	phys->body->setAngularFactor(float3(1.0f,1.0f,1.0f));
	entitysys::getByID(ent)->addComponent(phys);
	phys->body->setDamping( 0.95f, 1.0f );

	mPrototype = new EntityPrototype( entitysys::getByID(ent) );
	entitysys::destroyEntity(entitysys::getByID(ent));
}

Entity* SpectatorFactory::createRemoteSpectator(PlayerGUID pGUID, EntGUID eGUID)
{
	Entity* spectator = mPrototype->spawn();

	RenderComponent* rend = entitysys::createComponent<RenderComponent>(spectator->ID);
	RenderManager* renderManager = dynamic_cast<RenderManager*>(entitysys::getComponentManager(rend));
	assert( renderManager!= nullptr );
	renderManager->initComponent( rend );
	renderManager->setAsSphere( rend, 0.25f );
	spectator->addComponent(rend);

	_attachNetworkingComponents( spectator->ID, pGUID, eGUID );

	return spectator;
}

Entity* SpectatorFactory::createLocalSpectator( EntGUID eGUID )
{
	Entity* spectator = mPrototype->spawn();

	// Create a camera for local spectator
	CameraComponent* comp = entitysys::createComponent<CameraComponent>(spectator->ID);
	CameraManager* cameraManager = dynamic_cast<CameraManager*>(entitysys::getComponentManager(comp));
	assert( cameraManager!= nullptr );
	cameraManager->getCamera( comp );
	spectator->addComponent(comp);

	cameraManager->lookAt( comp, float3( 0,0,0 ));

	// PlayerManager holds local pGUID
	PlayerManager* playerMgr = dynamic_cast<PlayerManager*>(entitysys::getComponentManager<PlayerComponent>() );
	assert( playerMgr!= nullptr );

	_attachNetworkingComponents(spectator->ID, playerMgr->getLocalGUID(), eGUID );

	return spectator;
}

void SpectatorFactory::_attachNetworkingComponents (EntID ID, PlayerGUID pGUID, EntGUID eGUID)
{
	// Create player component
	PlayerComponent* pcomp = entitysys::createComponent<PlayerComponent>( ID );
	pcomp->pGUID = pGUID;
	entitysys::getByID(ID)->addComponent(pcomp);

	// Create network component
	NetworkComponent* ncomp = entitysys::createComponent<NetworkComponent>( ID );
	ncomp->eGUID = eGUID;
	entitysys::getByID(ID)->addComponent(ncomp);
}

