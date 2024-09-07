#include "DxCoreBase.h"


namespace FDX
{
	DxCoreBase::DxCoreBase(UINT width, UINT height, std::wstring name, HWND wnd) :
		_init(false),
		_width(width),
		_height(height),
		_title(name),
		_hWnd(wnd),
		_useWarpDevice(false)
	{
			_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	DxCoreBase::~DxCoreBase()
	{

	}
}