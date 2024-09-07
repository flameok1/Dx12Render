#include "Dx12Core.h"
#include <windows.h>




namespace FDX
{
    Dx12Core::Dx12Core(UINT width, UINT height, std::wstring name, HWND wnd)
        : DxCoreBase(width, height, name, wnd)
    {

    }

	void Dx12Core::InitDevice()
	{
        _init = false;
        UINT dxgiFactoryFlags = 0;
        
#if defined(_DEBUG)
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        // NOTE: �����bdevice�غc�e����A�_�h�|��device����
        {
            ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            {
                debugController->EnableDebugLayer();

                // Enable additional debug layers.
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif
        
        ComPtr<IDXGIFactory4> factory;
        if (FAILED( CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory))) )
        {
            return;
        }

        if (_useWarpDevice)
        {
            ComPtr<IDXGIAdapter> warpAdapter;
            if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))))
            {
                return;
            }

            //��device�I�s�A�U�������ܹ�
            if (FAILED(D3D12CreateDevice(
                                        warpAdapter.Get(),
                                        D3D_FEATURE_LEVEL_11_0,
                                        IID_PPV_ARGS(&_device)
                                    ))
                )
            {
                return;
            }
        }
        else
        {
            ComPtr<IDXGIAdapter1> hardwareAdapter;
            GetHardwareAdapter(factory.Get(), &hardwareAdapter);

            if (FAILED(D3D12CreateDevice(
                hardwareAdapter.Get(),
                D3D_FEATURE_LEVEL_11_0,
                IID_PPV_ARGS(&_device)
            )))
            {
                return;
            }
        }

        
        // �H�e�i��O������Device�U���O��DeviceContext�U���O�AD12�s�WCommandQueue
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if( FAILED(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue))) )
        {
            return;
        }

        // swap chain�򥻴N�Oback buff�a...dx11�N�X�{�F�Adx10�S�ιL�����D...dx9�S��
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = FrameCount;
        swapChainDesc.Width = _width;
        swapChainDesc.Height = _height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> swapChain;
        if (FAILED(factory->CreateSwapChainForHwnd(
            _commandQueue.Get(),
            _hWnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain
        )))
        {
            return;
        }

        
        // alt+enter�O�������ù����ֳt��A�������
        if (FAILED(factory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER)))
        {
            return;
        }
        
        //�����ഫIDXGISwapChain1�MIDXGISwapChain3�����O�~�����Y
        if (FAILED(swapChain.As(&_swapChain)))
        {
            return;
        }

        _frameIndex = _swapChain->GetCurrentBackBufferIndex();

        
        // Create descriptor heaps.
        {
            // Describe and create a render target view (RTV) descriptor heap. ���ӴN�OID3D11RenderTargetView�o�ӡA�u�O�令�o�Φ�
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            if (FAILED(_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap))))
            {
                return;
            }

            _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }
        
        // Create frame resources.
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

            // Create a RTV for each frame.
            for (UINT n = 0; n < FrameCount; n++)
            {
                if (FAILED(_swapChain->GetBuffer(n, IID_PPV_ARGS(&_renderTargets[n]))))
                {
                    return;
                }

                _device->CreateRenderTargetView(_renderTargets[n].Get(), nullptr, rtvHandle); //��������dx11��ClearRenderTargetView?
                rtvHandle.Offset(1, _rtvDescriptorSize);
            }
        }
        
        if (FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator))))
        {
            return;
        }
        

        _init = true;
	}


	void Dx12Core::OnInit()
	{
		InitDevice();
	}


    _Use_decl_annotations_
    void Dx12Core::GetHardwareAdapter(
            IDXGIFactory1* pFactory,
            IDXGIAdapter1** ppAdapter,
            bool requestHighPerformanceAdapter)
    {
        *ppAdapter = nullptr;

        //�M���eDX�����@�ˡA�|�q�w��=>�n��MD3D_FEATURE_LEVEL���w�U���ի�Ddevice
        //�U�����]�O���ի�Device�A���O�ܦ��O�ϥ�factory�h�C�|�U�س]�w�A���ի�Device

        ComPtr<IDXGIAdapter1> adapter;

        ComPtr<IDXGIFactory6> factory6;
        if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            for (
                UINT adapterIndex = 0;
                SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                    adapterIndex,
                    requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                    IID_PPV_ARGS(&adapter)));
                    ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    //���Lsoftware �����A�p�u�ݭnpass in "/warp" on the command line.
                    continue;
                }

                // ����ppDevice�ΥH�T�{�O�_�䴩�����إ�
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        if (adapter.Get() == nullptr)
        {
            for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    //���Lsoftware �����A�p�u�ݭnpass in "/warp" on the command line.
                    continue;
                }

                // ����ppDevice�ΥH�T�{�O�_�䴩�����إ�
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        *ppAdapter = adapter.Detach();
    }
}
