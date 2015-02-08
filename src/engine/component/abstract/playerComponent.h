#ifndef playerComponent_h__
#define playerComponent_h__

// Component used to identify that a given object belongs to, or is associated with, a given player
struct PlayerComponent
{
	PlayerComponent()
	{
		GUID = 0;
	}

	Component* clone()
	{

	}
	EntID GUID;
};
#endif // playerComponent_h__
