/*
  Description:
   
   This is a port of the OgreConsole code presented by PixL in the Ogre Forums then later added
   to the Ogre Wiki.
   
   This is a straight port replacing all the Overlay code with Gorilla code, some changes have
   been added but they are minor and do not add to the actual functionality of the class.
   
   
*/
#include "pch.h"
#include "ogreConsole.h"

#if OGRE_VERSION < 67584 // 1.8.0
template<> OgreConsole* Ogre::Singleton<OgreConsole>::ms_Singleton=0;
#else
template<> OgreConsole* Ogre::Singleton<OgreConsole>::msSingleton=0;
#endif

#define CONSOLE_FONT_INDEX 14

#define CONSOLE_LINE_LENGTH 85
#define CONSOLE_LINE_COUNT 15
static const unsigned char legalchars[]=" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"'#%&/()=?[]\\*-_.:,; ";

OgreConsole::OgreConsole( Engine* eng )
: mIsVisible(true), mIsInitialised(false), mScreen(0), mUpdateConsole(false), mUpdatePrompt(false), mStartline(0), mEngine( eng )
{
	Gorilla::Silverback* silverback = new Gorilla::Silverback();
	silverback->loadAtlas("dejavu");
	Gorilla::Screen* UIScreen = silverback->createScreen( eng->getRenderSystem()->getViewport(),"dejavu" );

	this->init( UIScreen );

	eng->getEventSystem()->registerListener(this);
}
 
OgreConsole::~OgreConsole()
{
 if (mIsInitialised)
  shutdown();
}

void OgreConsole::init(Gorilla::Screen* screen)
{
   
   if(mIsInitialised)
    shutdown();
   
   Ogre::Root::getSingletonPtr()->addFrameListener(this);
   Ogre::LogManager::getSingleton().getDefaultLog()->addListener(this);
   
   // Create gorilla things here.
   mScreen = screen;
   mLayer = mScreen->createLayer(15);
   mGlyphData = mLayer->_getGlyphData(CONSOLE_FONT_INDEX); // Font.CONSOLE_FONT_INDEX

   mConsoleText = mLayer->createMarkupText(CONSOLE_FONT_INDEX,  10,10, Ogre::StringUtil::BLANK);
   mConsoleText->width(mScreen->getWidth() - 10);
   mPromptText = mLayer->createCaption(CONSOLE_FONT_INDEX,  10,10, "> _");
   mDecoration = mLayer->createRectangle(8,8, mScreen->getWidth() - 16, mGlyphData->mLineHeight );
   mDecoration->background_gradient(Gorilla::Gradient_NorthSouth, Gorilla::rgb(128,128,128,128), Gorilla::rgb(64,64,64,128));
   mDecoration->border(2, Gorilla::rgb(128,128,128,128));
   
   mIsInitialised = true;
   
   print("%5Ogre%R%6Console%0 Activated. Press F1 to show/hide.%R");
}

void OgreConsole::shutdown()
{
 if(!mIsInitialised)
    return;
 
 mIsInitialised = false;
 
 Ogre::Root::getSingletonPtr()->removeFrameListener(this);
 Ogre::LogManager::getSingleton().getDefaultLog()->removeListener(this);
 
 mScreen->destroy(mLayer);
 
}


void OgreConsole::handle( Event* arg)
{
 if( arg->getEventType() == EV_CORE_KEY_PRESS )
 {
	 KeyboardEvent* e = static_cast<KeyboardEvent*>(arg);

	 if( e->mKeycode == SDL_SCANCODE_F1 )
	 {
		 setVisible( !mIsVisible );
	 }

	 if(!mIsVisible)
	  return;

	 if (e->mKeycode == SDL_SCANCODE_RETURN )
	 {
  
	  print("%3> " + prompt + "%R");
  
	  //split the parameter list
	  Ogre::StringVector params = Ogre::StringUtil::split(prompt, " ");

	  if (params.size())
	  {
	   std::map<Ogre::String, EngineMethodPtr>::iterator i;
	   for(i=commands.begin();i!=commands.end();i++){
		if((*i).first==params[0]){
		 if((*i).second)
		  (*i).second(params);
		 break;
		}
	   }
	   prompt.clear();
	   mUpdateConsole = true;
	   mUpdatePrompt = true;
	  }
	 }

	 else if (e->mKeycode == SDL_SCANCODE_BACKSPACE )
	 {
	  if (prompt.size())
	  {
	   prompt.erase(prompt.end() - 1); //=prompt.substr(0,prompt.length()-1);
	   mUpdatePrompt = true;
	  }
	 }
	 else if (e->mKeycode == SDL_SCANCODE_PAGEUP )
	 {
		if(mStartline>0)
		   mStartline--;
	  mUpdateConsole = true;
	 }

	 else if (e->mKeycode == SDL_SCANCODE_PAGEDOWN)
	 {
		if(mStartline<lines.size())
		   mStartline++;
	  mUpdateConsole = true;
	 }
 
	 mUpdatePrompt = true;
 }
 if(!mIsVisible)
	 return;
 if( arg->getEventType() == EV_CORE_TEXT_INPUT )
 {
	 KeyboardEvent* e = static_cast<KeyboardEvent*>(arg);
	 for(unsigned int c=0;c<sizeof(legalchars);c++)
	 {
		 prompt+=e->mKey;
		 mUpdatePrompt = true;
		 break;
	 }
 }

}

bool OgreConsole::frameStarted(const Ogre::FrameEvent &evt)
{
   if(mUpdateConsole)
    updateConsole();
    
   if (mUpdatePrompt)
    updatePrompt();
   
   return true;
}

void OgreConsole::updateConsole()
{
 
 mUpdateConsole = false;
 
 std::stringstream text;
 std::list<Ogre::String>::iterator i,start,end;

 //make sure is in range
 if(mStartline>lines.size())
  mStartline=lines.size();

 int lcount=0;
 start=lines.begin();
 for(unsigned int c=0;c<mStartline;c++)
    start++;
 end=start;
 for(unsigned int c=0;c<CONSOLE_LINE_COUNT;c++){
    if(end==lines.end())
       break;
    end++;
 }

 for(i=start;i!=end;i++)
 {
  lcount++;
  text << (*i) << "\n";
 }
 mConsoleText->text(text.str());
 
 // Move prompt downwards.
 mPromptText->top(10 + (lcount * mGlyphData->mLineHeight));
 
 // Change background height so it covers the text and prompt
 mDecoration->height(((lcount+1) * mGlyphData->mLineHeight) + 4);
 
 mConsoleText->width(mScreen->getWidth() - 20);
 mDecoration->width(mScreen->getWidth() - 16);
 mPromptText->width(mScreen->getWidth() - 20);
 
}

void OgreConsole::updatePrompt()
{
 mUpdatePrompt = false;
 std::stringstream text;
 text << "> " << prompt << "_";
 mPromptText->text(text.str());
}

void OgreConsole::print(const Ogre::String &text)
{
   //subdivide it into lines
   const char *str=text.c_str();
   int len=text.length();
   Ogre::String line;
   for(int c=0;c<len;c++){
      if(str[c]=='\n'||line.length()>=CONSOLE_LINE_LENGTH){
         lines.push_back(line);
         line="";
      }
      if(str[c]!='\n')
         line+=str[c];
   }
   if(line.length())
      lines.push_back(line);
   if(lines.size()>CONSOLE_LINE_COUNT)
      mStartline=lines.size()-CONSOLE_LINE_COUNT;
   else
      mStartline=0;
   mUpdateConsole=true;
}

bool OgreConsole::frameEnded(const Ogre::FrameEvent &evt)
{
 return true;
}

void OgreConsole::setVisible(bool isVisible)
{
 mIsVisible = isVisible;
 mLayer->setVisible(mIsVisible);
}

void OgreConsole::addCommand(const Ogre::String &command, EngineMethodPtr func)
{
 commands[command]=func;
}

void OgreConsole::removeCommand(const Ogre::String &command)
{
 commands.erase(commands.find(command));
}

#if OGRE_VERSION_MINOR < 8 && OGRE_VERSION_MAJOR < 2
void OgreConsole::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName )
#else
    // "bool& skip" added in Ogre 1.8
void OgreConsole::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skip )
#endif
{
 print(message);
}

// All Console commands here.
void Console_Net_Connect( Ogre::StringVector& str )
{
	if( str.size() > 1 )
	{
		const char* ip = str[1].c_str();
		OgreConsole::getSingleton().print("Connecting to host @ " + Ogre::String(ip) );

		OgreConsole::getSingleton().getEnginePtr()->SetAsClient( ip );
	}else
	{
		OgreConsole::getSingleton().print( "No IP given." );
	}
}

void Console_Geometry_Spawn( Ogre::StringVector& str )
{
	StaticGeometry* g = OgreConsole::getSingleton().getEnginePtr()->getStaticGeometry();

	// Floor
	g->addGeometry( float3(0,-0.5f,0), float3(70.0,1.0f,70.0f) );

	// Two ramps
	g->addGeometry( float3( 0,5.0f, 0.0f), float3( 5.0f, 0.25f,5.0f ), float3(45.0/57.0f,0,0));
	g->addGeometry( float3( 0,9.0f, 4.0f), float3( 5.0f, 0.25f,5.0f ), float3(-45.0f/57.0f,0,0));
}

void Console_Net_Status(Ogre::StringVector& str)
{
	// Lots of printing to do, shortcut it
	OgreConsole* c = OgreConsole::getSingletonPtr();

	// Get network system
	NetworkSystem* net = OgreConsole::getSingleton().getEnginePtr()->getNetworkSystem();
	int connectedClients = net->getConnectedClients();

	c->print("NETWORK STATUS");
	
	if( net->isHost() )
	{
		c->print("Engine is in SERVER mode.");
		c->print(std::to_string(connectedClients) + " peer(s) connected");

		for( int i=0; i<connectedClients; i++ )
			c->print("Client #"+std::to_string(i+1) + "(" + std::to_string( net->pingPeer(i) ) + "ms)");

	}else
	{
		c->print("Engine is in CLIENT mode.");

		if( connectedClients < 1 )
		{
			c->print("NOT connected to a server.");
		}
		else
		{
			c->print("Ping: " + std::to_string( net->pingPeer(0) ));
		}
	}
	
}
