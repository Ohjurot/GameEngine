#include "WindowsEngine.h"

#include <EngineWindows/WindowsWindow.h>

void Engine::OSInit()
{
    Get().Window = std::make_unique<WindowsWindow>();
}

void Engine::OSShutdown()
{
    Get().Window.reset();
}
