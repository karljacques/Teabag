#pragma once
#include "common.h"
#include "entityManager.h"
#include "engine/core/component/base/positionComponent.h"
#include "engine/core/component/physics/physicsComponent.h"
#include "engine/core/component/render/renderComponent.h"

class StaticGeometry
{
public:
	StaticGeometry( EntityManager* entityManager, RenderSystem* renderSystem, PhysicsManager* physicsManager );
	~StaticGeometry(void);

	// This method will create a new box that acts as static geometry.
	// It needs to be provided a position, a size, and an angle.
	void addGeometry( float3 position, float3 size, float3 angle = float3(0,0,0) );

private:
	// Dependencies
	EntityManager* mEntityManager;
	PhysicsManager* mPhysicsManager;
	RenderSystem* mRenderSystem;

	std::vector<Entity*> mGeometryList;
};

