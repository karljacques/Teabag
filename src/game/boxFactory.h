#ifndef boxFactory_h__
#define boxFactory_h__

#include "engine/factory/entityPrototype.h"

#include "engine/core/entitySystem.h"

#include "engine/component/physics/physicsManager.h"
#include "engine/component/render/renderManager.h"

#include "engine/component/physics/physicsComponent.h"
#include "engine/component/network/networkComponent.h"
#include "engine/component/render/renderComponent.h"

class BoxFactory
{
public:
	BoxFactory();

	Entity* createBox( float3 pos, EntGUID eGUID );

private:
	EntityPrototype* mPrototype;

};
#endif // boxFactory_h__
