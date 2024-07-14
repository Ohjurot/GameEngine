#pragma once

#include <GameEngine/Windows/WinInclude.h>

namespace Engine
{
    class D3D12Debug
    {
        public:
            void EnableDebug();
            void DisableDebug();

        private:
            #ifdef ENGINE_DEBUG
            ComPointer<ID3D12Debug> m_d3d12Debug;
            ComPointer<IDXGIDebug1> m_dxgiDebug;
            #endif
    };
}
