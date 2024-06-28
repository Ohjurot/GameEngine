#pragma once

#include <EngineCore/BasicLayer.h>
#include <EngineCore/EngineContext.h>

#include <EngineWindows/WinInclude.h>
#include <EngineWindows/WindowsWindow.h>
#include <EngineWindows/D3D12/D3D12Graphics.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_dx12.h>

namespace Engine
{
    /*!
     * @brief Implements imgui for D3D12
    */
    class D3D12Imgui : public BasicLayer
    {
        public:
            D3D12Imgui();
            ~D3D12Imgui();

            bool Update(float dt) override;
            void Draw() override;

        private:
            WindowsWindow& m_wnd;
            D3D12Graphics& m_gfx;
            ComPointer<ID3D12DescriptorHeap> m_imguiDescHeap;
    };
}
