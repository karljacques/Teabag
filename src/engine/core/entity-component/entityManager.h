#pragma once

#include "entity.h"

typedef unsigned int EntID;

class EntityManager
{
public:
	EntityManager(void);
	~EntityManager(void);

	EntID createEntity();
	void removeEntity( EntID ID );

	Entity* getByID( EntID ID );
	bool entityExists( EntID ID );

	void update( double dt );

private:

	std::vector<Entity*> mEntities;
	EntID mIDCount; /* TODO cheap method of finding a ID. Might have to find a better way 
							if there are likely to be more than 2 million entities created during the
							lifetime of the game. */
};

