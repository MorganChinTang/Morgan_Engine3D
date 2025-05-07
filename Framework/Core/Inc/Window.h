#pragma once

namespace Engine3D::Core
{
    class Window
    {
    public:
        void Initialize(HINSTANCE instance, const std::wstring& appName, uint32_t width, uint32_t height);
        void Terminate();

        void ProcessMessages();

        HWND GetWindowHandle() const;
        bool IsActive() const;

    private:
        HINSTANCE mInstance = nullptr;
        HWND mWindow = nullptr;
        RECT mScreenRect = {};
        std::wstring mAppName;
        bool mIsActive = false;
    };
}