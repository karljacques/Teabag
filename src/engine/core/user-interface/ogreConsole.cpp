/*
  Description:
   
   This is a port of the OgreConsole code presented by PixL in the Ogre Forums then later added
   to the Ogre Wiki.
   
   This is a straight port replacing all the Overlay code with Gorilla code, some changes have
   been added but they are minor and do not add to the actual functionality of the class.
   
   
*/
#include "pch.h"
#include "ogreConsole.h"
#include "..\event\eventSystem.h"

#if OGRE_VERSION < 67584 // 1.8.0
template<> OgreConsole* Ogre::Singleton<OgreConsole>::ms_Singleton=0;
#else
template<> OgreConsole* Ogre::Singleton<OgreConsole>::msSingleton=0;
#endif

#define CONSOLE_FONT_INDEX 9

#define CONSOLE_LINE_LENGTH 85
#define CONSOLE_LINE_COUNT 15
static const unsigned char legalchars[]=" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"'#%&/()=?[]\\*-_.:,; ";

OgreConsole::OgreConsole( CameraManager* cameraManager )
{
	mCameraManager = cameraManager;
	mUpdatePrompt = false;
	mUpdateConsole = false;
	mScreen = 0;
	mIsInitialised= 0;
	mIsVisible = true;
	mStartline = 0;

	Gorilla::Silverback* silverback = new Gorilla::Silverback();
	silverback->loadAtlas("dejavu");
	Gorilla::Screen* UIScreen = silverback->createScreen( mCameraManager->getViewport(),"dejavu" );

	this->init( UIScreen );

	mKeyboardActive = false;
	mEnableKeyGuard = false;
}

void OgreConsole::init( Gorilla::Screen* UIScreen )
{

   if(mIsInitialised)
    shutdown();
   
   Ogre::Root::getSingletonPtr()->addFrameListener(this);
   Ogre::LogManager::getSingleton().getDefaultLog()->addListener(this);
   
   // Create gorilla things here.
   mScreen = UIScreen;
   mLayer = mScreen->createLayer(15);
   mGlyphData = mLayer->_getGlyphData(CONSOLE_FONT_INDEX); // Font.CONSOLE_FONT_INDEX
   
   mConsoleWidth = 500.0f;
   mConsoleHeight = 200.0f;

   mConsoleText = mLayer->createMarkupText(CONSOLE_FONT_INDEX,  0,0, Ogre::StringUtil::BLANK);
   mConsoleText->width(mConsoleWidth);
   
   mPromptText = mLayer->createCaption(CONSOLE_FONT_INDEX,  0,0, "> _");
   mDecoration = mLayer->createRectangle(0,0, mConsoleWidth , mGlyphData->mLineHeight );
   mDecoration->background_gradient(Gorilla::Gradient_NorthSouth, Gorilla::rgb(0,0,0,128), Gorilla::rgb(0,0,0,128));
   
   mIsInitialised = true;

   mLineCount = (mConsoleHeight/18);
   
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
	if( arg->getEventType() == EV_CORE_CHAT_MESSAGE )
	{
		MessageEvent* me = arg->getData<MessageEvent>();
		this->print( "CHAT:" + me->message );
	}

	if( arg->getEventType() == EV_CORE_PRINT_CONSOLE )
	{
		MessageEvent* me = arg->getData<MessageEvent>();
		this->print( me->message );
	}

	if( arg->getEventType() == EV_CORE_TEXT_INPUT )
	{
		if( mKeyboardActive )
		{
			KeyboardEvent* e = arg->getData<KeyboardEvent>();
			// Check it's not the enable key guard
			if( mEnableKeyGuard == false )
			{
				// TODO check legality of character
				prompt+=e->mKey;
				mUpdatePrompt = true;
			}
			mEnableKeyGuard = false;
		}
	}

	if( arg->getEventType() == EV_CORE_KEY_PRESS )
	{
		KeyboardEvent* e = arg->getData<KeyboardEvent>();

		if( e->mKeycode == SDL_SCANCODE_F1 )
		{
			setVisible( !mIsVisible );
		}

		if( e->mKeycode == SDL_SCANCODE_T )
		{
			if( mKeyboardActive == false )
			{
				mKeyboardActive = true;
				mEnableKeyGuard = true;
			}
		}

		if( e->mKeycode == SDL_SCANCODE_SLASH )
		{
			if(!mKeyboardActive)
			{
				mKeyboardActive = true;
				mUpdatePrompt=true;
			}
		}

		if(!mIsVisible)
			return;



		if (e->mKeycode == SDL_SCANCODE_RETURN )
		{
			print("%3> " + prompt + "%R");

			// See if it's a command or chat
			if( prompt[0] == ("/")[0])
			{
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
					
				}
			}else
			{
				if( prompt.length() > 0 )
				{
					// It's chat - we need to send this off!
					Event* e = eventsys::get(EV_CORE_CHAT_MESSAGE, 0, this );
					MessageEvent* me = e->createEventData<MessageEvent>();
					me->message = prompt;
					eventsys::dispatch(e);
				}
				
			}

			mKeyboardActive = false;
			prompt.clear();
			mUpdateConsole = true;
			mUpdatePrompt = true;
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
 for(unsigned int c=0;c<(mConsoleHeight/18)+1;c++){
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
 mPromptText->top(  std::min( ((lcount) * mGlyphData->mLineHeight), (float)mConsoleHeight ));
 
 // Change background height so it covers the text and prompt
 mDecoration->height( std::min( ((lcount+1) * mGlyphData->mLineHeight), (float)mConsoleHeight ) + 10.0f );
 
 mConsoleText->width(mConsoleWidth);
 mDecoration->width(mConsoleWidth);
 mPromptText->width(mConsoleWidth);
 
}

void OgreConsole::updatePrompt()
{
	
 mUpdatePrompt = false;
 std::stringstream text;
 if( mKeyboardActive == false )
 {
	 text << "";
 }else
 {
	  text << "> " << prompt << "_";
 }

 mPromptText->text(text.str());
}

void OgreConsole::print(const Ogre::String &text)
{
	if( text.length()>0 )
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

	   if( lines.size() > mLineCount )
	   {
		   lines.pop_front();
	   }

	   if(line.length())
		  lines.push_back(line);
	   if(lines.size()>mLineCount)
		  mStartline=lines.size()-mLineCount;
	   else
		  mStartline=0;
	   mUpdateConsole=true;
	}
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
		// dispatch network system modifier
		eventsys::dispatch( eventsys::get(EV_NETWORK_MOD_CLIENT) );

		// Tell network system to connect to this IP:
		Event* e = eventsys::get(EV_NETWORK_MOD_CONNECT);
		MessageEvent* msg = e->createEventData<MessageEvent>();
		msg->message = str[1];
		eventsys::dispatch(e);
	}else
	{
		printm( "No IP given." );
	}
}

void Console_Set_Username( Ogre::StringVector& str )
{
	if( str.size() > 1 )
	{
		// Dispatch an event to change player name
		Event* e = eventsys::get( EV_PLAYER_SET_NAME );
		MessageEvent* msg = e->createEventData<MessageEvent>();
		msg->message = str[1];
		eventsys::dispatch( e );
	}

}

void printm(std::string message)
{
	Event* e = eventsys::get(EV_CORE_PRINT_CONSOLE);
	MessageEvent* msg = e->createEventData<MessageEvent>();

	msg->message = message;

	eventsys::dispatch(e);
}
