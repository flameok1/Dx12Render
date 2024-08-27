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
    private:

		HINSTANCE _hInst;                                // 目前執行個體
        HWND _hWnd;

        std::wstring sTitle;
        std::wstring sWindowClass;

    public:
        bool Init(HINSTANCE hInstance, WCHAR *pTitle, WCHAR *pWindowClass, int nCmdShow, WNDPROC WndProc)
        {
            sTitle = pTitle;
            sWindowClass = pWindowClass;

            WNDCLASSEXW wcex = { 0 };

            wcex.cbSize = sizeof(WNDCLASSEX);

            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = WndProc;
            wcex.hInstance = hInstance;
            wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wcex.lpszClassName = sWindowClass.c_str();

            RegisterClassExW(&wcex);

            _hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

            _hWnd = CreateWindowW(
                sWindowClass.c_str(), 
                sTitle.c_str(), 
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, 
                0, 
                CW_USEDEFAULT, 
                0, 
                nullptr, 
                nullptr, 
                hInstance, 
                nullptr);

            if (!_hWnd)
            {
                return false;
            }

            ShowWindow(_hWnd, nCmdShow);
            UpdateWindow(_hWnd);

            return true;
        }

        int UpdateLoop()
        {
            MSG msg = {};
            while (msg.message != WM_QUIT)
            {
                // 不同於GetMessage會卡到有Message才會回傳，PeekMessage就算沒Message也會回傳
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            return (int)msg.wParam;
        }
	};
}