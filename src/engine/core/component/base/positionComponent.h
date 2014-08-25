#ifndef positionComponent_h__
#define positionComponent_h__

#include "component.h"

#include <Math/float3.h>
#include <Math/Quat.h>


class PositionComponent :
	public Component, public EventListener
{
public:
	PositionComponent(void);
	~PositionComponent(void);

	float3 mPosition;
	Quat mOrientation;

	void update(  double dt  );

	void handle(  Event* e );

private:

	// Want everything to go through position first, so instead of dispatching events every loop - just set this flag when events need to be sent out
	bool mUpdated;

};
#endif // positionComponent_h__

