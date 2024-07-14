#include "D3D12Imgui.h"

Engine::D3D12Imgui::D3D12Imgui() :
    m_wnd(*dynamic_cast<WindowsWindow*>(Get().Window.get())),
    m_gfx(*dynamic_cast<D3D12Graphics*>(Get().Graphics.get()))
{
    // === Imgui core ===

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // === Win32 ===

    ImGui_ImplWin32_Init(m_wnd.GetWindowHandle());

    // === D3D12 Init ===

    D3D12_DESCRIPTOR_HEAP_DESC imguiDescHepDesc{};
    imguiDescHepDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    imguiDescHepDesc.NumDescriptors = 1;
    imguiDescHepDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    imguiDescHepDesc.NodeMask = 0;

    m_gfx.GetDevice()->CreateDescriptorHeap(&imguiDescHepDesc, IID_PPV_ARGS(&m_imguiDescHeap));

    ImGui_ImplDX12_Init(
        m_gfx.GetDevice(),
        D3D12Graphics::BufferCount,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        m_imguiDescHeap,
        m_imguiDescHeap->GetCPUDescriptorHandleForHeapStart(),
        m_imguiDescHeap->GetGPUDescriptorHandleForHeapStart()
    );
}

Engine::D3D12Imgui::~D3D12Imgui()
{
    // === D3D12 Shutdown ===

    ImGui_ImplDX12_Shutdown();
    m_imguiDescHeap.Release();

    // === Win32 ===

    ImGui_ImplWin32_Shutdown();

    // === ImGui core ===

    ImGui::DestroyContext();
}

bool Engine::D3D12Imgui::Update(float dt)
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    return true;
}

void Engine::D3D12Imgui::Draw()
{
    ImGui::Render();

    auto commandList = m_gfx.SetupAndGetCommandList();

    commandList->SetDescriptorHeaps(1, &m_imguiDescHeap);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

    m_gfx.DrawCommandList();
}
