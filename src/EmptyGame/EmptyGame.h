#pragma once

#include <EngineCore/GameBase.h>

#include <imgui.h>

/*!
 * @brief Simple empty game to demonstrate the engines use case
*/
class EmptyGame : public Engine::GameBase
{
    public:
        bool Update(float dt) override;
};
