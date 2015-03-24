#include "pch.h"
#include "boxFactory.h"

BoxFactory::BoxFactory()
{
	EntID ent = entitysys::createEntity();

	TransformComponent* trans = entitysys::createComponent<TransformComponent>(ent);
	entitysys::getByID(ent)->addComponent(trans);

	MotionComponent* motion = entitysys::createComponent<MotionComponent>(ent);
	entitysys::getByID(ent)->addComponent(motion);

	PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(ent);
	dynamic_cast<PhysicsManager*>(entitysys::getComponentManager(phys))->initComponent(phys,new btBoxShape( float3(0.5f)/2.0f ) , 5.0f);
	entitysys::getByID(ent)->addComponent(phys);

	RenderComponent* rend = entitysys::createComponent<RenderComponent>(ent);
	RenderManager* rendMgr = dynamic_cast<RenderManager*>(entitysys::getComponentManager<RenderComponent>());
	rendMgr->initComponent(rend);
	rendMgr->setAsBox( rend, float3(0.5f) );
	entitysys::getByID(ent)->addComponent(rend);

	mPrototype = new EntityPrototype( entitysys::getByID(ent) );
	entitysys::destroyEntity(entitysys::getByID(ent));
}

Entity* BoxFactory::createBox(float3 pos, EntGUID eGUID)
{
	Entity* box = mPrototype->spawn();
	NetworkComponent* net = entitysys::createComponent<NetworkComponent>(box->ID);
	net->eGUID = eGUID;
	box->addComponent(net);

	ENT_SET_TRANSFORM( box->ID, pos, Quat(0,0,0,1) );
	return box;
}

