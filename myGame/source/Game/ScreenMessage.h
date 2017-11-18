#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>


using namespace std;

namespace Rendering
{
	class ScreenMessage
	{

	public:
		//static string Message;
		
		//static void pushMessage(string msg);

		/*struct A{
			static std::vector<std::string> allMessages;
		};*/

		static void PushMessage(string msg);
		static void ClearMessages();
		static std::vector<string> Allmessages();

	private:		
		
		static std::vector<string> Msg;

	};

}