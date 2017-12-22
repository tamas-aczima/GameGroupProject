#pragma once

#include "Common.h"
#include <windows.h>
#include <string>
#include "GameClock.h"
#include "GameTime.h"
#include "GameComponent.h"
#include "ServiceContainer.h"
#include <fstream>

using namespace Library;

namespace Library
{
    class Game
    {
    public:
        Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
        ~Game();

        HINSTANCE Instance() const;
        HWND WindowHandle() const;
		HWND WindowGameMenu() const;
        const WNDCLASSEX& Window() const; 
        const std::wstring& WindowClass() const;
        const std::wstring& WindowTitle() const;
        int ScreenWidth() const;
        int ScreenHeight() const;

		ID3D11Device1* Direct3DDevice() const;
		ID3D11DeviceContext1* Direct3DDeviceContext() const;
		bool DepthBufferEnabled() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDesc() const;
		const D3D11_VIEWPORT& Viewport() const;

        virtual int Run();
        virtual void Exit();
        virtual void Initialize();
        virtual void Update(const GameTime& gameTime);
        virtual void Draw(const GameTime& gameTime);

		const std::vector<GameComponent*>& Components() const;
		const ServiceContainer& Services() const;

		HWND mWindowHandle;
		HWND mWindowGameMenu;

		static bool gameStart;
		void initialGame(bool gameStarted);
		static POINT menuPosition;

		static int screenX;
		static int screenY;
		static const UINT DefaultScreenWidth;
		static const UINT DefaultScreenHeight;

    protected:
		virtual void InitializeWindow();
		virtual void Shutdown();
		virtual void InitializeDirectX();

		static const UINT DefaultFrameRate;
		static const UINT DefaultMultiSamplingCount;

        HINSTANCE mInstance;
        std::wstring mWindowClass;
        std::wstring mWindowTitle;
        int mShowCommand;
        
        
        WNDCLASSEX mWindow;		

        UINT mScreenWidth;
        UINT mScreenHeight;

        GameClock mGameClock;
        GameTime mGameTime;

		std::vector<GameComponent*> mComponents;
		ServiceContainer mServices;

		D3D_FEATURE_LEVEL mFeatureLevel;
		ID3D11Device1* mDirect3DDevice;
		ID3D11DeviceContext1* mDirect3DDeviceContext;
		IDXGISwapChain1* mSwapChain;

		UINT mFrameRate;
		bool mIsFullScreen;
		bool mDepthStencilBufferEnabled;
		bool mMultiSamplingEnabled;
		UINT mMultiSamplingCount;
		UINT mMultiSamplingQualityLevels;

		ID3D11Texture2D* mDepthStencilBuffer;
		D3D11_TEXTURE2D_DESC mBackBufferDesc;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT mViewport;

    private:
        Game(const Game& rhs);
        Game& operator=(const Game& rhs);

        POINT CenterWindow(int windowWidth, int windowHeight);

		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		static void GameMenu(HWND hwnd, int option);
		static void LoadImages();
		static void DeleteMenu();
		static void PauseMenu(HWND windowHandle);
    };
}