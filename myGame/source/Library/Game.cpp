#include "Game.h"
#include "GameException.h"
#include "DrawableGameComponent.h"

#include <windows.h>
#include <fstream>
#include <cwchar>
#include <string>
#include <iostream>
#include <algorithm>

namespace Library
{
	const UINT Game::DefaultScreenWidth = 800;//1920; 
	const UINT Game::DefaultScreenHeight = 600;//1080; 
	const UINT Game::DefaultFrameRate = 60;
	const UINT Game::DefaultMultiSamplingCount = 4;
	int Game::screenX = 0;
	int Game::screenY = 0;

	bool Game::gameStart = false;
	POINT Game::menuPosition;
	//HWND mGameLogo;
	HBITMAP mGameLogoImage;
	HWND mGameLogo, mGameStart, mStartButton, mHighScore, mHelp, mCredits, mExit;
	wchar_t buff[4096];
	LPCWSTR highScoreText;

	void readRecordsFromFile();
	//static void Game:: initialGame(bool gameStarted);

    Game::Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
        : mInstance(instance), mWindowClass(windowClass), mWindowTitle(windowTitle), mShowCommand(showCommand),
          mWindowHandle(), mWindow(), mWindowGameMenu(),
          mScreenWidth(DefaultScreenWidth), mScreenHeight(DefaultScreenHeight),
          mGameClock(), mGameTime(),
		  mFeatureLevel(D3D_FEATURE_LEVEL_9_1),
		  mDirect3DDevice(nullptr),
		  mDirect3DDeviceContext(nullptr), mSwapChain(nullptr),
		  mFrameRate(DefaultFrameRate), mIsFullScreen(false),
		  mDepthStencilBufferEnabled(false),
		  mMultiSamplingEnabled(false),
		  mMultiSamplingCount(DefaultMultiSamplingCount),
		  mMultiSamplingQualityLevels(0),
		  mDepthStencilBuffer(nullptr), mRenderTargetView(nullptr),
		  mDepthStencilView(nullptr), mViewport(),
		  mComponents(), mServices()
    {
    }


	void readRecordsFromFile() {

		HANDLE hFile, hAppend;
		DWORD dwBytesRead, dwBytesWritten, dwPos;

		LPCWSTR fname = L"Content\\HighScore.txt";
		LPCWSTR fname2 = L"Content\\HighScore2.txt";
		//char buff[4096];


		//Open the existing file
		hFile = CreateFile(fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Could Not Open!");
		}
		else
		{
			printf("Opened successfully.");
		}

		hAppend = CreateFile(fname2, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


		do
		{
			if (ReadFile(hFile, buff, 4096, &dwBytesRead, NULL))
			{
				dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);

				if (LockFile(hAppend, dwPos, 0, dwBytesRead, 0) != 0)
					WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
			}
		} while (dwBytesRead == 4096);

			//Close file
			if (CloseHandle(hFile) != 0)
				printf("File closed!");
			if (CloseHandle(hAppend) != 0)
				printf("File 2 closed!");



			highScoreText = (LPCWSTR)buff;

			system("pause");

		//LPCWSTR 

		//ReadFile()

		//ifstream file("zombie.txt");


		//if (!file.is_open())
		//{
		//	cout << "Error opening file." << endl;

		//	return;
		//}
		//if (!file.good())
		//{
		//	cout << "not a good file." << endl;
		//	return;
		//}

		//while (!file.eof())
		//{
		//	map<string, int> answerTable;
		//	string roomName; //for room name
		//	vector<string> roomDescription;
		//	AdventureRoom *room = new AdventureRoom();
		//	MotionTableEntry *motionTable = new MotionTableEntry();
		//	string strRoomNo;
		//	int roomNo;
		//	getline(file, strRoomNo);

		//	roomNo = atoi(strRoomNo.c_str());
		//	room->setRoomNumber(roomNo);
		//	motionTable->setDestinationRoom(roomNo);

		//	while (true)
		//	{
		//		string line;
		//		getline(file, line);
		//		if (line != "*****")
		//		{
		//			roomName = line;
		//		}
		//		else
		//		{
		//			break;
		//		}
		//	}

		//	room->setRoomName(roomName);

		//	string line, line2;
		//	getline(file, line);

		//	while (line != "=====")
		//	{
		//		line2 = line2 + line + "\n";
		//		motionTable->setDirection(line2);

		//		getline(file, line);
		//	}

		//	while (true)
		//	{
		//		string line;
		//		getline(file, line);
		//		if (line != "-----")
		//		{
		//			roomDescription.push_back(line);
		//		}
		//		else
		//		{
		//			break;
		//		}

		//	}

		//	room->setDescription(roomDescription);

		//	while (true)
		//	{
		//		string line;
		//		getline(file, line);
		//		if (line == "+++++")
		//		{
		//			break;
		//		}
		//		else
		//		{
		//			size_t sep = line.find(" ");
		//			size_t sep2 = line.find(" ", sep + 1);
		//			size_t sep3 = line.find(" ", sep2 + 1);
		//			size_t sep4 = line.find(" ", sep3 + 1);
		//			string name = line.substr(0, sep);
		//			int space = atoi((line.substr(sep + 1)).c_str());
		//			int type = atoi((line.substr(sep2 + 1)).c_str());
		//			int usable = atoi((line.substr(sep3 + 1)).c_str());
		//			string objectDesc = line.substr(sep4 + 1);
		//			if (type == 1)
		//			{
		//				Key* newObject = new Key(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 2)
		//			{
		//				Lock* newObject = new Lock(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 3)
		//			{
		//				Container* newObject = new Container(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 4)
		//			{
		//				BackPack* newObject = new BackPack(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 5)
		//			{
		//				Food* newObject = new Food(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 6)
		//			{
		//				Drink* newObject = new Drink(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 7)
		//			{
		//				Clothes* newObject = new Clothes(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 8)
		//			{
		//				Weapon* newObject = new Weapon(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//			if (type == 9)
		//			{
		//				Shoes* newObject = new Shoes(name, space, usable, objectDesc);

		//				room->setObjectsInRoom(newObject);
		//			}
		//		}
		//	}

		//	while (true)
		//	{
		//		string lineQ;
		//		getline(file, lineQ);
		//		if (lineQ.length() == 0)
		//		{
		//			break;
		//		}
		//		else
		//		{
		//			size_t sep = lineQ.find(" ");
		//			string key = lineQ.substr(0, sep);
		//			if (lineQ.find("/") != string::npos)
		//			{
		//				size_t sep2 = lineQ.find("/");
		//				string keyName = lineQ.substr(sep2 + 1);
		//				motionTable->setDirectioninfo(key);
		//				motionTable->setKeyName(keyName);
		//			}
		//			int value = atoi((lineQ.substr(sep + 1)).c_str());
		//			answerTable[key] = value;
		//			room->setAnswerTable(answerTable);
		//		}
		//	}
		//	mapQuiz[roomNo] = room;

		//	motionTableVec.push_back(motionTable);
		//}
	}



    Game::~Game()
    {
    }

    HINSTANCE Game::Instance() const
    {
        return mInstance;
    }

    HWND Game::WindowHandle() const
    {
        return mWindowHandle;
    }

	HWND Game::WindowGameMenu() const
	{
		return mWindowGameMenu;
	}

    const WNDCLASSEX& Game::Window() const
    {
        return mWindow;
    }

    const std::wstring& Game::WindowClass() const
    {
        return mWindowClass;
    }

    const std::wstring& Game::WindowTitle() const
    {
        return mWindowTitle;
    }

    int Game::ScreenWidth() const
    {
        return mScreenWidth;
    }

    int Game::ScreenHeight() const
    {
        return mScreenHeight;
    }

	ID3D11Device1* Game::Direct3DDevice() const
	{
		return mDirect3DDevice;
	}

	ID3D11DeviceContext1* Game::Direct3DDeviceContext() const
	{
		return mDirect3DDeviceContext;
	}

	float Game::AspectRatio() const
	{
		return static_cast<float>(mScreenWidth) / mScreenHeight;
	}

	bool Game::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	const D3D11_TEXTURE2D_DESC& Game::BackBufferDesc() const
	{
		return mBackBufferDesc;
	}

	const D3D11_VIEWPORT& Game::Viewport() const
	{
		return mViewport;
	}

	const std::vector<GameComponent*>& Game::Components() const
	{
		return mComponents;
	}

	const ServiceContainer& Game::Services() const
	{
		return mServices;
	}
        
    int Game::Run()
    {
		//InitializeGameMenu();
		//InitializeWindow();
		//InitializeDirectX();
        //Initialize();



	
		InitializeWindow();
		InitializeDirectX();
		Initialize();



		//initialGame(gameStart);

		MSG message = {0}; //New code
        ZeroMemory(&message, sizeof(message));
        
        mGameClock.Reset();		

        while(message.message != WM_QUIT)
        {
            if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))//nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            else
            {
				if (gameStart)
				{
					mGameClock.UpdateGameTime(mGameTime);
					Update(mGameTime);
					Draw(mGameTime);
				}
				
            }
        }
		return (int)message.wParam;

		Shutdown();
    }

	void Game::initialGame(bool gameStarted)
	{
		if (gameStarted)
		{
			InitializeDirectX();
			Initialize();
		}
		else
		{
			return;
		}
	}

    void Game::Exit()
    {
        PostQuitMessage(0);
    }

    void Game::Initialize()
    {
		for (GameComponent* component : mComponents)
		{
			component->Initialize();
		}
    }

    void Game::Update(const GameTime& gameTime)
    {
		for (GameComponent* component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameTime);
			}
		}
    }

    void Game::Draw(const GameTime& gameTime)
    {
		for (GameComponent* component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
			{
				drawableGameComponent->Draw(gameTime);
			}
		}
    }

	void Game::InitializeGameMenu()
	{
		ZeroMemory(&mWindow, sizeof(mWindow));
		mWindow.cbSize = sizeof(WNDCLASSEX);
		mWindow.style = CS_CLASSDC;
		//mWindow.lpfnWndProc = WndProcGameMenu;
		mWindow.hInstance = mInstance;
		mWindow.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		mWindow.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
		mWindow.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		mWindow.lpszClassName = mWindowClass.c_str();

		RECT windowRectangle = { 0, 0, mScreenWidth, mScreenHeight };
		AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&mWindow);
		POINT center = CenterWindow(mScreenWidth, mScreenHeight);
		mWindowGameMenu = CreateWindow(mWindowClass.c_str(), mWindowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, mInstance, nullptr);


		ShowWindow(mWindowGameMenu, mShowCommand);
		UpdateWindow(mWindowGameMenu);

	}

    void Game::InitializeWindow()
    {
        ZeroMemory(&mWindow, sizeof(mWindow));
        mWindow.cbSize = sizeof(WNDCLASSEX);
        mWindow.style = CS_CLASSDC;
        mWindow.lpfnWndProc = WndProc;
        mWindow.hInstance = mInstance;
        mWindow.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        mWindow.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
        mWindow.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
        mWindow.lpszClassName = mWindowClass.c_str();		

        RECT windowRectangle = { 0, 0, mScreenWidth, mScreenHeight };
        AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

        RegisterClassEx(&mWindow);
        POINT center = CenterWindow(mScreenWidth, mScreenHeight);
		menuPosition = center;
        mWindowHandle = CreateWindow(mWindowClass.c_str(), mWindowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, mInstance, nullptr);

        ShowWindow(mWindowHandle, mShowCommand);
        UpdateWindow(mWindowHandle);
    }

	void Game::InitializeDirectX()
	{
		HRESULT hr;
		UINT createDeviceFlags = 0;
/*

#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
*/

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		//Step 1: Create the Direct3D device and device contect interfaces

		ID3D11Device* direct3DDevice = nullptr;
		ID3D11DeviceContext* direct3DDeviceContext = nullptr;

		if (FAILED(hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels,
			ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &mFeatureLevel, &direct3DDeviceContext)))
		{
			throw GameException("D3D11CreateDevice() failed", hr);
		}

		if (FAILED(hr = direct3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mDirect3DDevice))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", hr);
		}

		if (FAILED(hr = direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mDirect3DDeviceContext))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", hr);
		}

		ReleaseObject(direct3DDevice);
		ReleaseObject(direct3DDeviceContext);

		//Step 2: Check for multisampling support
		
		mDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMultiSamplingCount, &mMultiSamplingQualityLevels);
		
		if (mMultiSamplingQualityLevels == 0)
		{
			throw GameException("Unsupported multi-sampling quality");
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.Width = mScreenWidth;
		swapChainDesc.Height = mScreenHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allowed alt+Enter to enter full screen.

		if (mMultiSamplingEnabled)
		{
			swapChainDesc.SampleDesc.Count = mMultiSamplingCount;
			swapChainDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
		}
		else 
		{
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//Step 3: Create the swap chain

		IDXGIDevice* dxgiDevice = nullptr;

		if (FAILED(hr = mDirect3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", hr);
		}

		IDXGIAdapter *dxgiAdapter = nullptr;

		if (FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
		{
			ReleaseObject(dxgiDevice);
			throw GameException("IDXGIDEVICE::GetParent() failed retrieving adapter.", hr);
		}

		IDXGIFactory2* dxgiFactory = nullptr;

		if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			throw GameException("IDXGIAdapter::GetParent() failed retrieving factory.", hr);
		}

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
		fullScreenDesc.RefreshRate.Numerator = mFrameRate;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = !mIsFullScreen;

		if (FAILED(hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, mWindowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &mSwapChain)))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			ReleaseObject(dxgiFactory);
			throw GameException("IDXGIDevice::CreateSwapChainForHwnd() failed.", hr);
		}

		ReleaseObject(dxgiDevice);
		ReleaseObject(dxgiAdapter);
		ReleaseObject(dxgiFactory);

		//Step 4: Create the render target view

		ID3D11Texture2D* backBuffer;

		if (FAILED(hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
		{
			throw GameException("IDXGISwapChain::GetBuffer() failed.", hr);
		}

		backBuffer->GetDesc(&mBackBufferDesc);

		if (FAILED(hr = mDirect3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView)))
		{
			ReleaseObject(backBuffer);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", hr);
		}

		ReleaseObject(backBuffer);

		//Step 5: Create the depth-stencil view

		if (mDepthStencilBufferEnabled)
		{
			D3D11_TEXTURE2D_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
			depthStencilDesc.Width = mScreenWidth;
			depthStencilDesc.Height = mScreenHeight;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

			if (mMultiSamplingEnabled)
			{
				depthStencilDesc.SampleDesc.Count = mMultiSamplingCount;
				depthStencilDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
			}
			else
			{
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
			}

			if (FAILED(hr = mDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer)))
			{
				throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
			}

			if (FAILED(hr = mDirect3DDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView)))
			{
				throw GameException("IDXGIDevice::CreateDepthStencilView() failed.", hr);
			}
		}

		//Step 6: Bind the render target and depth-stencil views to OM stage

		mDirect3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.Width = static_cast<float>(mScreenWidth);
		mViewport.Height = static_cast<float>(mScreenHeight);
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		//Step 7: Set the viewport

		mDirect3DDeviceContext->RSSetViewports(1, &mViewport);
	}

	void Game::Shutdown()
	{
		ReleaseObject(mRenderTargetView);
		ReleaseObject(mDepthStencilView);
		ReleaseObject(mSwapChain);
		ReleaseObject(mDepthStencilBuffer);

		if (mDirect3DDeviceContext != nullptr)
		{
			mDirect3DDeviceContext->ClearState();
		}

		ReleaseObject(mDirect3DDeviceContext);
		ReleaseObject(mDirect3DDevice);

		UnregisterClass(mWindowClass.c_str(), mWindow.hInstance);
	}

	void Game::GameMenu(HWND hwnd, int option)
	{
		DeleteMenu();

		mGameLogo = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP , 0, 0, 1920, 1080, hwnd, NULL, NULL, NULL);
		SendMessageW(mGameLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) mGameLogoImage);
		if (option == 1)
		{
			mGameStart = CreateWindowW(TEXT("Button"), TEXT("Start"), WS_VISIBLE | WS_CHILD | WS_BORDER, 100, menuPosition.y, 200, 50, hwnd, (HMENU("ID_BUTTON")), NULL, NULL);
		}
		else
		{
			mGameStart = CreateWindowW(TEXT("Button"), TEXT("Resume"), WS_VISIBLE | WS_CHILD | WS_BORDER, 100, menuPosition.y, 200, 50, hwnd, (HMENU("ID_BUTTON")), NULL, NULL);
		}
		//HWND mGameStart = CreateWindowW(TEXT("Button"), NULL,  WS_VISIBLE | WS_CHILD | WS_BORDER | BS_BITMAP , menuPosition.x, 150, 700, 250, hwnd, (HMENU("ID_BUTTON")), NULL, NULL);
		//SendMessageW(mGameStart, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)mGameLogoImage);
		mHighScore = CreateWindow(TEXT("Button"), TEXT("High Scores"), WS_CHILD | WS_VISIBLE | WS_BORDER , 100, menuPosition.y + 100, 200, 50, hwnd, (HMENU("HS_BUTTON")), NULL, NULL);
		
		mHelp = CreateWindow(TEXT("Button"), TEXT("Help"), WS_CHILD | WS_VISIBLE | WS_BORDER, 100, menuPosition.y + 200, 200, 50, hwnd, (HMENU("HP_BUTTON")), NULL, NULL);
		mCredits = CreateWindow(TEXT("Button"), TEXT("Credits"), WS_CHILD | WS_VISIBLE | WS_BORDER, 100, menuPosition.y + 300, 200, 50, hwnd, (HMENU("CB_BUTTON")), NULL, NULL);
		mExit = CreateWindow(TEXT("Button"), TEXT("Exit"), WS_CHILD | WS_VISIBLE | WS_BORDER , 100, menuPosition.y + 400, 200, 50, hwnd, (HMENU("EX_BUTTON")), NULL, NULL);
	}

	void Game::DeleteMenu()
	{
		DestroyWindow(mGameLogo);
		DestroyWindow(mGameStart);
		DestroyWindow(mHighScore);
		DestroyWindow(mHelp);
		DestroyWindow(mCredits);
		DestroyWindow(mExit);
	}
	void Game::PauseMenu(HWND windowHandle)
	{

		if (gameStart)
		{
			GameMenu(windowHandle, 0);

			Game::gameStart = false;
		}
		else if (!gameStart)
		{

			Game::gameStart = true;

			DeleteMenu();
		}

	
	}

	void Game::LoadImages()
	{
		mGameLogoImage = (HBITMAP)LoadImageW(NULL, L"Content\\ROL.bmp", IMAGE_BITMAP, 1920, 1080, LR_LOADFROMFILE);
	}


	LRESULT CALLBACK Game::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch(message)
        {
		case WM_CREATE:  //WM_RBUTTONDOWN://WM_CREATE: 

						 /*HMENU hMenubar = CreateMenu();
						 HMENU hCredits = CreateMenu();
						 HMENU hExit = CreateMenu();

						 AppendMenu(hMenubar, MF_POPUP, NULL, L"Start");
						 AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hCredits, L"Credits");
						 AppendMenu(hMenubar, MF_POPUP, NULL, L"Exit");

						 AppendMenu(hCredits, MF_POPUP, (UINT_PTR)hCredits, L"Yong");
						 AppendMenu(hCredits, MF_POPUP, (UINT_PTR)hCredits, L"Khai");

						 SetMenu(windowHandle, hMenubar);*/
			LoadImages();

			readRecordsFromFile();

			GameMenu(windowHandle, 1);
			return 0;

		case WM_COMMAND:


			if (LOWORD(wParam) == (LOWORD("ID_BUTTON")))
			{
				Game::gameStart = true;

				DeleteMenu();
			}

			if (LOWORD(wParam) == (LOWORD("HP_BUTTON")))
			{
				MessageBox(windowHandle, L"Guide to play Ray of Light: \n Step 1:  ", L"Help", MB_OK);

			}


			if (LOWORD(wParam) == (LOWORD("CB_BUTTON")))
			{
				MessageBox(windowHandle, L"Team No. 3 \nTeam Leader / Game Programmer - Khai \nLead Programmer / Asset Manager - Tamas \nAI & Animator / Game Programmer - Martin \nGame Designer / Game Programmer - Ivan ", L"Credits", MB_OK);
			}


			if (LOWORD(wParam) == (LOWORD("EX_BUTTON")))
			{
					int confirmation = MessageBox(0, L"Do you want to quit?", L"Quit", MB_ICONASTERISK | MB_YESNO);

					if (confirmation == IDYES)
					{
						PostQuitMessage(0);
					}
			}

			return 0;


		case WM_CLOSE:
		{	
			int confirmation = MessageBox(0, L"Do you want to quit?", L"Quit", MB_ICONASTERISK | MB_YESNO);

			if (confirmation == IDYES)
			{
				PostQuitMessage(0);
			}
		}
			return 0;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{	
				if (gameStart)
				{
					GameMenu(windowHandle, 0);

					Game::gameStart = false;
				}
				else
				{
					Game::gameStart = true;

					DeleteMenu();

					SendMessage(windowHandle, WM_ACTIVATE,0,0);
				}
			}
			return 0;

		case WM_DESTROY:				
                PostQuitMessage(0);
                return 0;
			case WM_LBUTTONDOWN:
				Game::screenX = ((int)(short)LOWORD(lParam));
				Game::screenY = ((int)(short)HIWORD(lParam));
			case WM_RBUTTONDOWN:
				Game::screenX = ((int)(short)LOWORD(lParam));
				Game::screenY = ((int)(short)HIWORD(lParam));
        }

        return DefWindowProc(windowHandle, message, wParam, lParam);
    }

	//LRESULT WINAPI Game:: WndProcGameMenu(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	//{
	//	switch (message)
	//	{
	//	case WM_CREATE:  //WM_RBUTTONDOWN://WM_CREATE: 

	//					 /*HMENU hMenubar = CreateMenu();
	//					 HMENU hCredits = CreateMenu();
	//					 HMENU hExit = CreateMenu();

	//					 AppendMenu(hMenubar, MF_POPUP, NULL, L"Start");
	//					 AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hCredits, L"Credits");
	//					 AppendMenu(hMenubar, MF_POPUP, NULL, L"Exit");

	//					 AppendMenu(hCredits, MF_POPUP, (UINT_PTR)hCredits, L"Yong");
	//					 AppendMenu(hCredits, MF_POPUP, (UINT_PTR)hCredits, L"Khai");

	//					 SetMenu(windowHandle, hMenubar);*/
	//		
	//		GameMenu(windowHandle, 1);

	//		//CreateWindow(TEXT("Button"), TEXT("Button"), WS_CHILD | WS_VISIBLE , 300, 300, 100, 30, windowHandle, (HMENU("ID_BUTTON")), NULL, NULL);

	//		return 0;
	//		//break;


	//	case WM_COMMAND:


	//		if (LOWORD(wParam) == (LOWORD("ID_BUTTON")))
	//		{
	//			//MessageBox(windowHandle, L"Button Pressed", L"OMG!", MB_ICONINFORMATION);

	//			//ShowWindow(mWindowHandle, mShowCommand);

	//			Game::gameStart = true;

	//			//InitializeWindow();
	//			//InitializeDirectX();
	//			//Initialize();

	//		}

	//		return 0;


	//	case WM_CLOSE:
	//	{
	//		int confirmation = MessageBox(0, L"Do you want to quit?", L"Quit", MB_ICONASTERISK | MB_YESNO);

	//		if (confirmation == IDYES)
	//		{
	//			PostQuitMessage(0);
	//		}
	//	}
	//	return 0;

	//	case WM_KEYDOWN:
	//		if (wParam == VK_ESCAPE)
	//		{
	//			int confirmation = MessageBox(0, L"Do you want to quit?", L"Quit", MB_ICONASTERISK | MB_YESNO);

	//			if (confirmation == IDYES)
	//			{
	//				PostQuitMessage(0);
	//			}
	//		}
	//		return 0;

	//	case WM_DESTROY:
	//		PostQuitMessage(0);
	//		return 0;
	//	}

	//	return DefWindowProc(windowHandle, message, wParam, lParam);
	//}


    POINT Game::CenterWindow(int windowWidth, int windowHeight)
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        POINT center;
        center.x = (screenWidth - windowWidth) / 2;
        center.y = (screenHeight - windowHeight) / 2;

        return center;
    }	

	

}