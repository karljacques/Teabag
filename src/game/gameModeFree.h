#ifndef gameModeFree_h__
#define gameModeFree_h__

#include "engine/manager/game/frame.h"
#include "game/SpectatorFactory.h"

#include "engine/core/event/events/spawnEvent.h"

class GameModeFree : public Frame
{
public:

	GameModeFree( void );
	~GameModeFree();

	FrameState update( void );
	FrameState backgroundUpdate( void );

	void handle(  Event* e );

	void reset( void );

	void deinit( void );
private:

	SpectatorFactory mSpectatorFactory;
	std::vector<Entity*> mSpectators;

	Entity* mLocalSpectator;

};
#endif // gameModeFree_h__