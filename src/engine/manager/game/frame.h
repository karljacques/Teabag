#ifndef gameMode_h__
#define gameMode_h__

#include "engine/core/eventSystem.h"

enum FrameState
{
	FR_ACTIVE,
	FR_BACKGROUND,
	FR_TERMINATED,
	FR_BEGIN_TERMINATION
};

class Engine;

class Frame : public EventListener
{
	friend class Engine;

public:
	virtual FrameState update() = 0;
	virtual FrameState backgroundUpdate() = 0;

	virtual void reset(){};

	virtual void deinit() = 0;

protected:

	FrameState mFrameState;

};
#endif // gameMode_h__
