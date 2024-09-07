#pragma once

#include "DxCoreBase.h"
//使用ComPtr必備
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include "include/d3dx12/d3dx12.h"
#include <dxgi1_6.h>
#include <d3dcompiler.h>

namespace FDX
{
	class DxCoreBase;

	/// <summary>
	/// use DirectX 12
	/// </summary>
	class Dx12Core : DxCoreBase
	{
	public:
		Dx12Core(UINT width, UINT height, std::wstring name, HWND wnd);
	private:
		//ComPtr vs shared_ptr ...主要是ComPtr會呼叫release()...

		static const UINT FrameCount = 2;

		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;
		ComPtr<IDXGISwapChain3> _swapChain;
		ComPtr<ID3D12Device> _device;
		ComPtr<ID3D12Resource> _renderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator> _commandAllocator;
		ComPtr<ID3D12CommandQueue> _commandQueue;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> _rtvHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		UINT _rtvDescriptorSize;

		

		/// <summary>
		/// 目前swapchain的index
		/// </summary>
		UINT _frameIndex;
	protected:
	public:
		virtual void OnInit();
	private:
		/// <summary>
		/// 初始化device，Dx9只有Device，Dx11和Dx12多少有改變如SwapChain等，統一叫Device
		/// </summary>
		void InitDevice();

		void GetHardwareAdapter(
			_In_ IDXGIFactory1* pFactory,
			_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter = false);
	};
}