#pragma once

#include <GameEngine/Core/EngineContext.h>

#include <memory>

#define ENGINE_DEFINE_GAME_CLASS(type) \
int main(int argc, char** argv) \
{ \
    return Engine::RunGame<type>(argc, argv); \
}

namespace Engine
{
    /*!
     * @brief Implements the base for a game
    */
    class GameBase : 
        public Engine::BasicLayer, 
        public std::enable_shared_from_this<GameBase>
    {
        public:
            GameBase();
            ~GameBase();

            /*!
             * @brief Runs the game without / or already parsed arguments
             * @return Application return code
            */
            int Run();

            /*!
             * @brief Run the application for "normale" C++ application arguments
             * @param argc C++ argc
             * @param argv C++ argv
             * @return Application return code
            */
            int Run(int argc, char** argv);

            bool Update(float dt) override { return true; }

        private:

    };

    /*!
     * @brief Creates and runs a game
     * @tparam T Type of the game
     * @tparam ...Args Type of arguments of the games run function
     * @param ...args Arguments values of the run function
     * @return Games return code
    */
    template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<GameBase, T>>>
    int RunGame(Args&&... args)
    {
        return std::make_shared<T>()->Run(std::forward<Args>(args)...);
    }
}
