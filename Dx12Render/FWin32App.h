#pragma once
#include "FApp.h"
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // �q Windows ���Y�ư����`�ϥΪ�����
// Windows ���Y��
#include <windows.h>
// C ���涥�q���Y��
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

		HINSTANCE _hInst;                                // �ثe�������
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

            _hInst = hInstance; // �N������鱱��N�X�x�s�b�����ܼƤ�

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
                // ���P��GetMessage�|�d�즳Message�~�|�^�ǡAPeekMessage�N��SMessage�]�|�^��
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