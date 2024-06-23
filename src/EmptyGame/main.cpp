#include <EngineCore/EngineContext.h>

int main()
{
    Engine::Init();

    auto& wnd = *Engine::Get().Window;

    while (!wnd.CloseRequested())
    {
        wnd.Update();
    }

    Engine::Shutdown();
}
