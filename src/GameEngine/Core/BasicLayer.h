#pragma once

namespace Engine
{
    /*!
     * @brief Interface for an application layer
    */
    class BasicLayer
    {
        public:
            virtual ~BasicLayer() = default;

            /*!
             * @brief Retrieves the layers index
             * @return Index as int
            */
            inline int GetLayerIndex()
            {
                return m_index;
            }

            /*!
             * @brief Sets the layers index
             * @param index Index (0 -> First, inf -> last)
            */
            inline void SetLayerIndex(int index)
            {
                m_index = index;
            }

            /*!
             * @brief Updates the logic of the layer
             * @param dt Delta time in seconds
             * @return True when successfully. Returning false will terminate the application
            */
            virtual bool Update(float dt) = 0;

            /*!
             * @brief Draws content evaluated while update
            */
            virtual void Draw() {};

            /*!
             * @brief Called when the layer is attached to the stack
            */
            virtual void OnAttache() {};

            /*!
             * @brief Called when the layer is detached from the stack
            */
            virtual void OnDettache() {};

        private:
            int m_index = 0;
    };
}
