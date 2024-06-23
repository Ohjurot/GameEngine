#include <EngineCore/EngineContext.h>

int main()
{
    Engine::Init();

    auto& wnd = *Engine::Get().Window;
    auto& gfx = *Engine::Get().Graphics;

    while (!wnd.CloseRequested())
    {
        wnd.Update();

        gfx.BeginFrame();
        // TODO: Rendering
        gfx.EndFrame();
    }

    Engine::Shutdown();
}
