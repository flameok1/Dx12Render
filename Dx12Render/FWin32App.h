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
    public:
        FWin32App();

    private:

		HINSTANCE _hInst;                                // �ثe�������
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