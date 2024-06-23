#pragma once

#include <EngineCore/IWindow.h>

#include <memory>

namespace Engine
{
    /*!
     * @brief Core struct that defines all engines subsystems
    */
    struct EngineContext
    {
        std::unique_ptr<IWindow> Window;
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
