#include "EngineContext.h"

namespace Engine
{
    void OSInit();
    void OSShutdown();
}

void Engine::Init()
{
    OSInit();

    auto& ctx = Get();

    ctx.Imgui->SetLayerIndex(90);
    ctx.LayerStack.AttacheLayer(ctx.Imgui);
}

void Engine::Shutdown()
{
    Get().LayerStack.ClearLayers();

    OSShutdown();
}

Engine::EngineContext& Engine::Get()
{
    static EngineContext ctx;
    return ctx;
}
