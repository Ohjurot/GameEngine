#pragma once

#include <EngineCore/IGraphics.h>

#include <EngineWindows/WinInclude.h>
#include <EngineWindows/WindowsWindow.h>
#include <EngineWindows/D3D12/D3D12Debug.h>

#include <string>

namespace Engine
{
    /*!
     * @brief Implements d3d12 drawing
    */
    class D3D12Graphics : public IGraphics
    {
        public:
            static const size_t BufferCount = 2;

        public:
            D3D12Graphics(WindowsWindow& wnd);
            ~D3D12Graphics();

            /*!
             * @brief Flushes the GPU queue
             * @param flushCount Number of flushes to occur
            */
            void FlushQueue(size_t flushCount = 1);

            /*!
             * @brief Waits for the fence for a given value
             * @param fenceValue Value to wait for. When -1 waits for the current max fence value
            */
            void WaitForFence(UINT64 fenceValue = -1);

        public:
            const char* GetAPIName() override;
            const char* GetGPUName() override;
            void BeginFrame() override;
            void EndFrame() override;
            size_t GetCanvasWidth() override;
            size_t GetCanvasHight() override;

        private:
            void GetBuffers();
            void ReleaseBuffers();
            void SetupViewportAndRect();

        private:
            D3D12Debug m_debug;

            WindowsWindow& m_window;
            D3D12_VIEWPORT m_viewport;
            D3D12_RECT m_fullRect;

            std::string m_gpuName;
            UINT64 m_rtvHeapIncrement = 0;

            ComPointer<IDXGIFactory6> m_dxgiFactory;

            ComPointer<ID3D12Device> m_gpuDevice;
            ComPointer<ID3D12CommandQueue> m_directQueue;
            ComPointer<ID3D12Fence> m_directQueueFence;
            UINT64 m_directQueueFenceValue = 0;
            HANDLE m_directQueueFenceEvent = nullptr;

            ComPointer<IDXGISwapChain3> m_swapChain;
            ComPointer<ID3D12Resource> m_buffers[BufferCount];
            ComPointer<ID3D12DescriptorHeap> m_rtvHeap;
            D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandles[BufferCount];
            size_t m_bufferWidth;
            size_t m_bufferHeight;

            ComPointer<ID3D12CommandAllocator> m_cmdAllocator;
            ComPointer<ID3D12GraphicsCommandList> m_cmdList;

            UINT m_currentBufferIndex = -1;
    };
}
