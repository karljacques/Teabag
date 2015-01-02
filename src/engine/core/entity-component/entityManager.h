#pragma once

#include "entity.h"

class EntityManager
{
public:
	EntityManager(void);
	~EntityManager(void);

	Entity* createEntity();
	void removeEntity( unsigned int LUID );

	Entity* getByLUID( unsigned int LUID );
	bool entityExists( unsigned int LUID );

	void update( double dt );

private:

	std::vector<Entity*> mEntities;
	unsigned int mLUIDCount; /* TODO cheap method of finding a LUID. Might have to find a better way 
							if there are likely to be more than 2 million entities created during the
							lifetime of the game. */
};

