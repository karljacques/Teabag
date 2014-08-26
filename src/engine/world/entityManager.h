#pragma once

#include "engine/core/component/base/entity.h"

class EntityManager
{
public:
	EntityManager(void);
	~EntityManager(void);

	void addEntity( Entity* e );
	void removeEntity( Entity* e );

	void update( double dt );

private:

	std::vector<Entity*> mEntities;
};

