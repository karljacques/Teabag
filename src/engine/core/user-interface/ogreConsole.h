#ifndef OgreConsoleForGorilla_h__
#define OgreConsoleForGorilla_h__

/*
  Description:
   
   This is a port of the OgreConsole code presented by PixL in the Ogre Forums then later added
   to the Ogre Wiki[1].
   
   This is a straight port replacing all the Overlay code with Gorilla code, some changes have
   been added but they are minor and do not add to the actual functionality of the class.
   
  Port Author:
   
   Betajaen.
   
  Usage:
   
   
   
  References:
   
   [1] http://www.ogre3d.org/tikiwiki/OgreConsole&structure=Cookbook
   
*/

#include "engine/core/event/eventListener.h"
#include "../render/UI/Gorilla.h"
#include "engine/engine.h"

typedef void ( *EngineMethodPtr ) (Ogre::StringVector& );
class Engine;

class OgreConsole : public Ogre::Singleton<OgreConsole>, Ogre::FrameListener, Ogre::LogListener, public EventListener
{
    
 public:
    
    OgreConsole( Engine* eng );
    
   ~OgreConsole();
    
    void   init(Gorilla::Screen* screen_to_use);
    void   shutdown();
    
    void   setVisible(bool mIsVisible);
    bool   isVisible(){return mIsVisible;}
    
    void   print(const Ogre::String &text);
    
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameEnded(const Ogre::FrameEvent &evt);
    
    void handle( Event* arg);
    
    void addCommand(const Ogre::String &command, EngineMethodPtr func);
    void removeCommand(const Ogre::String &command);

	Engine* getEnginePtr( ){ return mEngine; };
 
    //log
#if OGRE_VERSION_MINOR < 8 && OGRE_VERSION_MAJOR < 2
    void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );
#else
    // "bool& skip" added in Ogre 1.8
    void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skip );
#endif
 private:
    
    void  updateConsole();
    void  updatePrompt();
    
    bool                 mIsVisible;
    bool                 mIsInitialised;
    Gorilla::Screen*     mScreen;
    Gorilla::Layer*      mLayer;
    Gorilla::Caption*    mPromptText;
    Gorilla::MarkupText* mConsoleText;
    Gorilla::Rectangle*  mDecoration;
    Gorilla::GlyphData*  mGlyphData;
    
    bool                 mUpdateConsole;
    bool                 mUpdatePrompt;

	bool				 mKeyboardActive;
			
    unsigned int         mStartline;
    std::list<Ogre::String>      lines;
    Ogre::String            prompt;
    std::map<Ogre::String, EngineMethodPtr>  commands;

	Engine* mEngine;

	unsigned int mConsoleWidth;
	unsigned int mConsoleHeight;

	bool mEnableKeyGuard;
	int mLineCount;
};

// Console Commands
void Console_Net_Connect(Ogre::StringVector& str );
void Console_Geometry_Spawn( Ogre::StringVector& str );
void Console_Net_Status(Ogre::StringVector& str );


#endif // OgreConsoleForGorilla_h__
