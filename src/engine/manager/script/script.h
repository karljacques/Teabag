#ifndef script_h__
#define script_h__

#include "engine/core/eventSystem.h"
#include "engine/core/event/events/actionEvent.h"

class Script : public EventListener 
{
public:

	virtual void onStart( void );
	virtual void onStop( void );

	virtual void update( double dt ) = 0;

	void		 bind( Entity* e );
	void		 unbind( );

	// WARNING: The event must always be an ActionEvent.
	bool		 doesActionMatchTarget( Event* e );

protected:

	Entity* mTarget;

private:

};
#endif // script_h__
