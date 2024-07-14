#pragma once

namespace Engine
{
    /*!
     * @brief Defines the interface to the core graphics api
    */
    class IGraphics
    {
        public:
            virtual ~IGraphics() = default;

            /*!
             * @brief Retrieve the name of the underlaying rendering api
             * @return ASCII String
            */
            virtual const char* GetAPIName() = 0;

            /*!
             * @brief Retrieves the name of the GPU handling the drawing
             * @return ASCII String
            */
            virtual const char* GetGPUName() = 0;

            /*!
             * @brief Function to be called before any drawing happens
            */
            virtual void BeginFrame() = 0;

            /*!
             * @brief Function to be called after all drawing happened (renders and displays)
            */
            virtual void EndFrame() = 0;

            /*!
             * @brief Retrieves the drawing canvas width
             * @return Width in pixel
            */
            virtual size_t GetCanvasWidth() = 0;

            /*!
             * @brief Retrieves the drawing canvas height
             * @return Hight in pixel
            */
            virtual size_t GetCanvasHight() = 0;
    };
}
