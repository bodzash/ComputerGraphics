#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

// Needs to be singleton
class Window
{
public:
    Window()
    {
        Init();
    }

    void Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(800, 600, "Renderer", NULL, NULL);
    }

    void Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    inline static HWND GetNativeWindowHandle() { return glfwGetWin32Window(m_Window); }
    inline int GetWidth() { return m_Width; }
    inline int GetHeight() { return m_Height; }

private:
    static GLFWwindow* m_Window;
    int m_Width = 800;
    int m_Height = 600;
};