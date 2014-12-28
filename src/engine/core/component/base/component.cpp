#include "pch.h"
#include "component.h"
#include "engine/core/event/eventSystem.h"

Component::Component(void)
{

}


Component::~Component(void)
{
}

void Component::registerListener( EventListener* comp )
{
	mListeners.push_back(comp);
}


void Component::deregisterListener( EventListener* comp )
{
	for (auto i = mListeners.begin(); i != mListeners.end(); i++ ) {

		if( comp == (*i) )
		{
			mListeners.erase(i);
			break;
		}

	}
}

void Component::dispatch( Event* e )
{

	for( auto i=mListeners.begin(); i!=mListeners.end(); i++ )
		(*i)->handle( e );

	EventSystem::getSingletonPtr()->releaseEvent(e);

}

void Component::update( double dt )
{

}
