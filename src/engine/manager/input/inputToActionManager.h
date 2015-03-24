#ifndef actionMapping_h__
#define actionMapping_h__

#include "engine/core/eventSystem.h"

class InputToActionManager : public EventListener
{
public:
	void handle( Event* e );
private:
};
#endif // actionMapping_h__
