#include "FWin32App.h"
#include "resource.h"

namespace FApp
{
    FWin32App::FWin32App()
        :_hInst(nullptr), _hWnd(nullptr)
    {

    }

    bool FWin32App::Init(HINSTANCE hInstance, WCHAR* pTitle, WCHAR* pWindowClass, int nCmdShow)
    {
        sTitle = pTitle;
        sWindowClass = pWindowClass;

        WNDCLASSEXW wcex = { 0 };

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = this->WndProc;
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

    int FWin32App::UpdateLoop()
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

    HWND FWin32App::GetWnd()
    {
        return _hWnd;
    }

    int count = 0;

    LRESULT CALLBACK FWin32App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // ��R�\���������:
            switch (wmId)
            {
            case IDM_ABOUT:
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            wchar_t text[256];
            wsprintf(text, L"GG %d\n", count);
            RECT rec = { 0,0,200,100 };
            count++;

            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �b���s�W����ϥ� hdc ��ø�ϵ{���X...
            DrawText(hdc, text, -1, &rec, DT_LEFT);
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
}