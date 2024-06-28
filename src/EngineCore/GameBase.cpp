#include "GameBase.h"

Engine::GameBase::GameBase()
{
    Engine::Init();
    SetLayerIndex(100);
}

Engine::GameBase::~GameBase()
{
    Engine::Shutdown();
}

int Engine::GameBase::Run()
{
    Engine::Get().LayerStack.AttacheLayer(shared_from_this());

    auto& wnd = *Engine::Get().Window;
    auto& gfx = *Engine::Get().Graphics;
    auto& stack = Engine::Get().LayerStack;

    bool shouldExit = false;
    while (!shouldExit)
    {
        wnd.Update();
        shouldExit |= wnd.CloseRequested();

        gfx.BeginFrame();
        shouldExit |= !stack.UpdateLayers(0.0f);
        gfx.EndFrame();
    }

    return 0;
}

int Engine::GameBase::Run(int argc, char** argv)
{
    // TODO: Parse arguments

    return Run();
}
