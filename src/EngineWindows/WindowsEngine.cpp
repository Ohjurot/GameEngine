#include "WindowsEngine.h"

#include <EngineWindows/WindowsWindow.h>
#include <EngineWindows/D3D12/D3D12Graphics.h>
#include <EngineWindows/D3D12/D3D12Imgui.h>

void Engine::OSInit()
{
    Get().Window = std::make_shared<WindowsWindow>();
    Get().Graphics = std::make_shared<D3D12Graphics>();
    Get().Imgui = std::make_shared<D3D12Imgui>();
}

void Engine::OSShutdown()
{
    Get().Imgui.reset();
    Get().Graphics.reset();
    Get().Window.reset();
}
