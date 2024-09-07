// Dx12Render.cpp : 定義應用程式的進入點。
//

#include "framework.h"
#include "Dx12Render.h"
#include "FWin32App.h"
#include "Dx12Core.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
    WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX12RENDER, szWindowClass, MAX_LOADSTRING);

    FApp::FWin32App app;

    if (!app.Init(hInstance, szTitle, szWindowClass, nCmdShow))
    {
        return FALSE;
    }

    FDX::Dx12Core dxcore(800, 600, L"GG", app.GetWnd());

    return app.UpdateLoop();
}