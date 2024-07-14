#pragma once

namespace Engine
{
    /*!
     * @brief Represent the window the game can run in
    */
    class IWindow
    {
        public:
            virtual ~IWindow() = default;

            /*!
             * @brief Updates the window
            */
            virtual void Update() = 0;

            /*!
             * @brief Returns true for one time after the user requested the window to close
             * @return True when close was request
            */
            virtual bool CloseRequested() = 0;

            /*!
             * @brief Retrieve the client width of the window
             * @return Width in pixel
            */
            virtual size_t GetWidth() = 0;
            
            /*!
             * @brief Retrieve the client height of the window
             * @return Height in pixel
            */
            virtual size_t GetHeight() = 0;
    };
}
