#ifndef entityPrototype_h__
#define entityPrototype_h__

#include "engine/core/entitySystem.h"

class EntityPrototype
{
public:
	EntityPrototype( Entity* ent );
	Entity* spawn( void );

private:
	Entity* _prototype;
	Entity* _clone( Entity* ent );
	
};
#endif // entityPrototype_h__
