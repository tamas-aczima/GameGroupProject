﻿#include <memory>
#include "GameException.h"
#include "RenderingGame.h"


//memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace Library;
using namespace Rendering;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{

	//may remove the code here
	//if enabled, memory leaks are listed in Visual studio's output panel after the program exits.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::unique_ptr<Game> game(new RenderingGame(instance, L"RenderingClass", L"Maze", showCommand));

    try
    {
        game->Run();
    }
    catch (GameException ex)
    {
        MessageBox(game->WindowHandle(), ex.whatw().c_str(), game->WindowTitle().c_str(), MB_ABORTRETRYIGNORE);
    }

	//end of remove here
    return 0;
}