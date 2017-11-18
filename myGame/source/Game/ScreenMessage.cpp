#include "ScreenMessage.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Game.h"


namespace Rendering
{

	std::vector<string> ScreenMessage::Msg;

	void ScreenMessage::PushMessage(string msg)
	{
		Msg.push_back(msg);
	}

	void ScreenMessage::ClearMessages()
	{
		
		Sleep(1000);
		if (Msg.size() > 0)
		{
			Msg.erase(Msg.begin());
		}
	}

	std::vector<string> ScreenMessage::Allmessages()
	{
		return Msg;
	}




}