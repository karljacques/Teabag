#ifndef positionComponent_h__
#define positionComponent_h__

#include "component.h"

#include <Math/float3.h>
#include <Math/Quat.h>


class PositionComponent :public Component
{
public:
	PositionComponent(void);
	~PositionComponent(void);

	float3 mPosition;
	Quat mOrientation;

};
#endif // positionComponent_h__

