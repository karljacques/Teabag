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

	// Setters
	void setPosition( float3 pos );
	void setOrientation( Quat orientation );

	// Getters
	float3 getPosition();
	Quat getOrientation();

	// Component functions
	void update(  double dt  );
	void handle(  Event* e );

	// These functions prevent a nasty loop occuring with the physics engine
	void _setPosition( float3 pos );
	void _setOrientation( Quat rot );

private:

	// Want everything to go through position first, so instead of dispatching events every loop - just set this flag when events need to be sent out
	bool mUpdated;

	float3 mPosition;
	Quat mOrientation;



};
#endif // positionComponent_h__

