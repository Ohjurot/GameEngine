#pragma once

#include <EngineCore/IWindow.h>
#include <EngineCore/IGraphics.h>
#include <EngineCore/LayerStackManager.h>

#include <memory>

namespace Engine
{
    /*!
     * @brief Core struct that defines all engines subsystems
    */
    struct EngineContext
    {
        std::shared_ptr<IWindow> Window;
        std::shared_ptr<IGraphics> Graphics;
        std::shared_ptr<BasicLayer> Imgui;
        LayerStackManager LayerStack;
    };

    /*!
     * @brief Init the engines systems
    */
    void Init();

    /*!
     * @brief Shutdowns the engines systems
    */
    void Shutdown();

    /*!
     * @brief Retrieves the engines context
     * @return Reference to context
    */
    EngineContext& Get();
}
