#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Needs to be singleton
class Window
{
public:
    Window();
    void Init();
    void Shutdown();

    inline static HWND GetNativeWindowHandle() { return glfwGetWin32Window(m_Window); }
    inline static int GetWidth() { return m_Width; }
    inline static int GetHeight() { return m_Height; }

private:
    static GLFWwindow* m_Window;
    static const int m_Width = 800;
    static const int m_Height = 600;
};