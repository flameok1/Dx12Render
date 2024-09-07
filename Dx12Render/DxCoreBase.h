#pragma once

#include <string>
#include <windows.h>

typedef unsigned int UINT;

namespace FDX
{

	class DxCoreBase
	{
	public:
		DxCoreBase(UINT width, UINT height, std::wstring name, HWND wnd);

		~DxCoreBase();
	private:
		// Window title.
		std::wstring _title;

    protected:
		//init end
		bool _init;

        //解析度參數
		UINT _width;
		UINT _height;
        float _aspectRatio;
		HWND _hWnd;

        // 以往初始化device分為硬體和軟體兩種，現在多了一種warp，高效能軟體版
        bool _useWarpDevice;

	public:
		virtual void OnInit() = 0;
	};

}

