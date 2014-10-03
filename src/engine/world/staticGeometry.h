#pragma once
#include "common.h"
#include "entityManager.h"
#include "engine/core/component/base/positionComponent.h"
#include "engine/core/component/physics/physicsComponent.h"
#include "engine/core/component/render/renderComponent.h"

#include "engine/engine.h"

class StaticGeometry :
	public EventListener
{
public:
	StaticGeometry( Engine* engine );
	~StaticGeometry(void);

	// This method will create a new box that acts as static geometry.
	// It needs to be provided a position, a size, and an angle.
	Entity* addGeometry( float3 position, float3 size, float3 angle = float3(0,0,0) );

	void handle( Event* e );
private:
	// Dependencies
	Engine* mEngine;

	std::vector<Entity*> mGeometryList;
};

