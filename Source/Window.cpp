#include "Window.h"

GLFWwindow* Window::m_Window = glfwCreateWindow(800, 600, "ComputerGraphics", nullptr, nullptr);

Window::Window()
{
    Init();
}

void Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void Window::Shutdown()
{
    glfwDestroyWindow(Window::m_Window);
    glfwTerminate();
}