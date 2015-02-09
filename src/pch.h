#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#include <Ogre.h>

#include <SDL.h>
#include <SDL_syswm.h>

#include <stdint.h>

#include <iostream>
#include <queue>

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <RakNetTypes.h>
#include <BitStream.h>
#include <GetTime.h>
#include <RakNetVersion.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>