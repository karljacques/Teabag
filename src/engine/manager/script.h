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

	bool		 doesActionMatchTarget( ActionEvent* e );

protected:

	Entity* mTarget;
};
#endif // script_h__
