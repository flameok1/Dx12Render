#pragma once
#include "FApp.h"
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 從 Windows 標頭排除不常使用的項目
// Windows 標頭檔
#include <windows.h>
// C 執行階段標頭檔
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>


namespace FApp
{

	class FWin32App : public FApp
	{
    public:
        FWin32App();

    private:

		HINSTANCE _hInst;                                // 目前執行個體
        HWND _hWnd;

        std::wstring sTitle;
        std::wstring sWindowClass;


    public:
        bool Init(HINSTANCE hInstance, WCHAR* pTitle, WCHAR* pWindowClass, int nCmdShow);

        int UpdateLoop();

        HWND GetWnd();

    private:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}