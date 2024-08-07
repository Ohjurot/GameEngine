#pragma once

#include <GameEngine/Core/IWindow.h>
#include <GameEngine/Windows/WinInclude.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine
{
    class WindowsWindow : public IWindow
    {
        public:
            WindowsWindow();
            ~WindowsWindow();

            /*!
             * @brief Exposes the window handle
             * @return Native win32 handle
            */
            inline HWND GetWindowHandle() { return m_wnd; }

        public:
            void Update() override;
            bool CloseRequested() override;
            size_t GetWidth() override;
            size_t GetHeight() override;

        private:
            static LRESULT WindowProc_Setup(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
            static LRESULT WindowProc_Redirect(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

            LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

        private:
            HINSTANCE m_hInstance = nullptr;
            ATOM m_windowClass = 0;
            HWND m_wnd = nullptr;

            size_t m_width = 1920;
            size_t m_height = 1080;
            bool m_closeRequest = false;
    };
}
