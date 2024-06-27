#include "D3D12Graphics.h"

Engine::D3D12Graphics::D3D12Graphics(WindowsWindow& wnd) :
    m_window(wnd),
    m_bufferWidth(m_window.GetWidth()),
    m_bufferHeight(m_window.GetHeight())
{
    m_debug.EnableDebug();

    // === Init DXGI Factory ===
    
    UINT dxgiFactoryFlag = 0;
    #ifdef CONF_Debug
    dxgiFactoryFlag = DXGI_CREATE_FACTORY_DEBUG;
    #endif
    CreateDXGIFactory2(dxgiFactoryFlag, IID_PPV_ARGS(&m_dxgiFactory));

    // === Find GPU ===

    ComPointer<IDXGIAdapter> gpuAdapter;
    m_dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&gpuAdapter));
    DXGI_ADAPTER_DESC gpuDescription;
    gpuAdapter->GetDesc(&gpuDescription);
    m_gpuName.reserve(128);
    WideCharToMultiByte(CP_UTF8, 0, gpuDescription.Description, -1, (LPSTR)m_gpuName.data(), 128, nullptr, nullptr);

    // === Create the device & queue ===

    D3D12CreateDevice(gpuAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_gpuDevice));

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;
    m_gpuDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_directQueue));

    m_gpuDevice->CreateFence(m_directQueueFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_directQueueFence));

    m_directQueueFenceEvent = CreateEventW(nullptr, false, false, nullptr);

    // === Get heap metrics ===

    m_rtvHeapIncrement = m_gpuDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // === Swap Chain ===

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = m_bufferWidth;
    swapChainDesc.Height = m_bufferHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = BufferCount;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
    fullscreenDesc.RefreshRate.Denominator = 0;
    fullscreenDesc.RefreshRate.Numerator = 0;
    fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fullscreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    fullscreenDesc.Windowed = TRUE;

    ComPointer<IDXGISwapChain1> swapChain;
    m_dxgiFactory->CreateSwapChainForHwnd(m_directQueue, m_window.GetWindowHandle(), &swapChainDesc, &fullscreenDesc, nullptr, &swapChain);
    swapChain.QueryInterface(m_swapChain);

    // === Retrive RTV & Buffers ===

    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescriptorHeapDesc.NumDescriptors = BufferCount;
    rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDescriptorHeapDesc.NodeMask = 0;

    m_gpuDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapStartHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    for (size_t i = 0; i < BufferCount; i++)
    {
        m_rtvHandles[i] = { rtvHeapStartHandle.ptr + i * m_rtvHeapIncrement };
    }

    GetBuffers();

    // === Command execution ===

    m_gpuDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocator));
    m_gpuDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator, nullptr, IID_PPV_ARGS(&m_cmdList));

    // === Init viewport and rect ===

    SetupViewportAndRect();
}

Engine::D3D12Graphics::~D3D12Graphics()
{
    FlushQueue(BufferCount);

    ReleaseBuffers();

    m_swapChain.Release();

    CloseHandle(m_directQueueFenceEvent);
    m_directQueueFence.Release();
    m_directQueue.Release();
    m_gpuDevice.Release();

    m_dxgiFactory.Release();

    m_debug.DisableDebug();
}

void Engine::D3D12Graphics::FlushQueue(size_t flushCount /*= 1*/)
{
    for (size_t i = 0; i < flushCount; i++)
    {
        m_directQueue->Signal(m_directQueueFence, ++m_directQueueFenceValue);
        WaitForFence();
    }
}

void Engine::D3D12Graphics::WaitForFence(UINT64 fenceValue /*= -1*/)
{
    if (fenceValue == -1) fenceValue = m_directQueueFenceValue;

    if (m_directQueueFence->GetCompletedValue() < fenceValue)
    {
        if (SUCCEEDED(m_directQueueFence->SetEventOnCompletion(fenceValue, m_directQueueFenceEvent)))
        {
            if (WaitForSingleObject(m_directQueueFenceEvent, 30000) == WAIT_OBJECT_0)
            {
                return;
            }
        }

        // Fallback wait
        while (m_directQueueFence->GetCompletedValue() < fenceValue) Sleep(1);
    }
}

const char* Engine::D3D12Graphics::GetAPIName()
{
    return "D3D12";
}

const char* Engine::D3D12Graphics::GetGPUName()
{
    return m_gpuName.c_str();
}

void Engine::D3D12Graphics::BeginFrame()
{
    // === SwapChain resizing ===

    size_t newWidth = m_window.GetWidth();
    size_t newHeight = m_window.GetHeight();
    if (newWidth != m_bufferWidth || newHeight != m_bufferHeight)
    {
        FlushQueue(BufferCount);

        ReleaseBuffers();
        m_swapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
        GetBuffers();

        m_bufferWidth = newWidth;
        m_bufferHeight = newHeight;

        SetupViewportAndRect();
    }

    // === Get active buffer ===
    m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
    auto backBuffer = m_buffers[m_currentBufferIndex];
    auto rtvHandle = m_rtvHandles[m_currentBufferIndex];

    // === Prepare render target ===

    D3D12_RESOURCE_BARRIER rtSetupBarrier{};
    rtSetupBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    rtSetupBarrier.Transition.pResource = backBuffer;
    rtSetupBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    rtSetupBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    rtSetupBarrier.Transition.Subresource = 0;
    rtSetupBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

    m_cmdList->ResourceBarrier(1, &rtSetupBarrier);

    // === Cleat rtv and prepare for drawing ===

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    m_cmdList->RSSetViewports(1, &m_viewport);
    m_cmdList->RSSetScissorRects(1, &m_fullRect);
    m_cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
}

void Engine::D3D12Graphics::EndFrame()
{
    // === State of back buffer ===

    auto backBuffer = m_buffers[m_currentBufferIndex];

    D3D12_RESOURCE_BARRIER rtSetupBarrier{};
    rtSetupBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    rtSetupBarrier.Transition.pResource = backBuffer;
    rtSetupBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    rtSetupBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    rtSetupBarrier.Transition.Subresource = 0;
    rtSetupBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

    m_cmdList->ResourceBarrier(1, &rtSetupBarrier);

    // === Execute commands ===
    m_cmdList->Close();
    ID3D12CommandList* lists[] = { m_cmdList };
    m_directQueue->ExecuteCommandLists(1, lists);

    // === Wait and present ===
    FlushQueue();
    m_swapChain->Present(1, 0);

    // === Reset cmd allocator and list ===

    m_cmdAllocator->Reset();
    m_cmdList->Reset(m_cmdAllocator, nullptr);
}

size_t Engine::D3D12Graphics::GetCanvasWidth()
{
    return m_bufferWidth;
}

size_t Engine::D3D12Graphics::GetCanvasHight()
{
    return m_bufferHeight;
}

void Engine::D3D12Graphics::GetBuffers()
{
    for (size_t i = 0; i < BufferCount; i++)
    {
        m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i]));
        m_gpuDevice->CreateRenderTargetView(m_buffers[i], nullptr, m_rtvHandles[i]);
    }
}

void Engine::D3D12Graphics::ReleaseBuffers()
{
    for (size_t i = 0; i < BufferCount; i++)
    {
        m_buffers[i].Release();
    }
}

void Engine::D3D12Graphics::SetupViewportAndRect()
{
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = m_bufferWidth;
    m_viewport.Height = m_bufferHeight;
    m_viewport.MinDepth = 1.0f;
    m_viewport.MaxDepth = 0.0;

    m_fullRect.left = 0;
    m_fullRect.right = m_bufferWidth;
    m_fullRect.top = 0;
    m_fullRect.bottom = m_bufferHeight;
}
