#include "D3D12Debug.h"

void Engine::D3D12Debug::EnableDebug()
{
    #ifdef ENGINE_DEBUG
    DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug));
    D3D12GetDebugInterface(IID_PPV_ARGS(&m_d3d12Debug));

    m_d3d12Debug->EnableDebugLayer();
    #endif
}

void Engine::D3D12Debug::DisableDebug()
{
    #ifdef ENGINE_DEBUG
    m_d3d12Debug.Release();

    m_dxgiDebug->ReportLiveObjects(
        DXGI_DEBUG_ALL,
        (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_IGNORE_INTERNAL | DXGI_DEBUG_RLO_DETAIL)
    );

    m_dxgiDebug.Release();
    #endif
}
