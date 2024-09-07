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

        //�ѪR�װѼ�
		UINT _width;
		UINT _height;
        float _aspectRatio;
		HWND _hWnd;

        // �H����l��device�����w��M�n���ءA�{�b�h�F�@��warp�A���į�n�骩
        bool _useWarpDevice;

	public:
		virtual void OnInit() = 0;
	};

}

