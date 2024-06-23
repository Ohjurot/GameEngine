#include "WindowsEngine.h"

#include <EngineWindows/WindowsWindow.h>
#include <EngineWindows/D3D12/D3D12Graphics.h>

void Engine::OSInit()
{
    Get().Window = std::make_unique<WindowsWindow>();
    Get().Graphics = std::make_unique<D3D12Graphics>(*dynamic_cast<WindowsWindow*>(Get().Window.get()));
}

void Engine::OSShutdown()
{
    Get().Graphics.reset();
    Get().Window.reset();
}
