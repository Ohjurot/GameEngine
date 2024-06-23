#include "EngineContext.h"

namespace Engine
{
    void OSInit();
    void OSShutdown();
}

void Engine::Init()
{
    OSInit();

    // TODO: OS Independent init
}

void Engine::Shutdown()
{
    // TODO: OS Independent shutdown

    OSShutdown();
}

Engine::EngineContext& Engine::Get()
{
    static EngineContext ctx;
    return ctx;
}
