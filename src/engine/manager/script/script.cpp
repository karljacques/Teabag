#include "script.h"
#include "engine/component/player/playerComponent.h"

//Matching must be handled differently for player and non-player actions.
// An example of a player action might be moving forward, or shooting. These actions must be relevant to the player, and not affect another player.
// An example of a non-player action might be opening a door. This action is initiated by a single player, but affects all players - thus we should 
// not be checking that the door is owned by a player
bool Script::doesActionMatchTarget( Event* e )
{
	assert( e->getEventType() == EVT_ACTION );
	
	// Get ActionEvent data
	ActionEvent* data = e->getData<ActionEvent>();

	// Does Action concern a particular player?
	if( data->pGUID == 0 )
	{
		// No, it does not
		// Check the Entity GUID is the same as the ActionEvent GUID
		EntID ID = network::getNetworkManager()->getIDByGUID( data->eGUID );
		if( mTarget->ID != ID )
			return false;

		// If it's gotten this far, all checks out. 
		return true;
	}else
	{
		// Yes, it does concern a particular player, and we must match
		// Get the PlayerComponent of our Target - if it has no player component, not a possible match.
		if( mTarget->hasComponent<PlayerComponent>() == false )
			return false;

		PlayerComponent* pComp = mTarget->getComponent<PlayerComponent>();

		// Check the player is the same player our ActionEvent concerns.
		if( pComp->pGUID != data->pGUID )
			return false;

		// Check the Entity GUID is the same as the ActionEvent GUID
		EntID ID = network::getNetworkManager()->getIDByGUID( data->eGUID );
		if( mTarget->ID != ID )
			return false;

		// If it's gotten this far, all checks out. 
		return true;
	}
	
	return false;
}

void Script::bind(Entity* e)
{
	mTarget = e;
}

void Script::unbind()
{
	mTarget = nullptr;
}


