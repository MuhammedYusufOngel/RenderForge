#include "glwindow.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace graf{

    void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        void* ptr = glfwGetWindowUserPointer(window);
        GLWindow* windowPtr = (GLWindow*)ptr;
        windowPtr->m_keyboardFunction(key, action);
    }
    
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        void * ptr = glfwGetWindowUserPointer(window);
        GLWindow* glWindow = (GLWindow*)ptr;
        glWindow->m_mouseButtonFunction(button,action,mods);     
    }

    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        void* ptr = glfwGetWindowUserPointer(window);
        GLWindow* windowPtr = (GLWindow*)ptr;

        double dx = xpos - windowPtr->m_oldMouseX;
        double dy = ypos - windowPtr->m_oldMouseY;
        windowPtr->m_mouseFunction(dx, dy);

        windowPtr->m_oldMouseX = xpos;
        windowPtr->m_oldMouseY = ypos;
    }

    void GLWindow::initializeImgui()
    {
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void GLWindow::renderImgui()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_guiFunction();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
    }

    void GLWindow::create(int width, int height)
    {
        if(!glfwInit())
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSetErrorCallback(error_callback);

        m_window = glfwCreateWindow(width, height, "Deneme", NULL, NULL);
        if (!m_window)
        {
            // Window or OpenGL context creation failed
        }
    

        glfwMakeContextCurrent(m_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }

        glfwSetKeyCallback(m_window, key_callback);
        glfwGetCursorPos(m_window, &m_oldMouseX, &m_oldMouseY);
        glfwSetCursorPosCallback(m_window, cursor_position_callback);
        glfwSetWindowUserPointer(m_window, this);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);
        initializeImgui();
    }

    GLFWwindow* GLWindow::getWindow(){
        return m_window;
    }

    void GLWindow::setRenderFunction(RenderFunction function){
        m_renderFunction = function;
    }

    void GLWindow::setMouseMoveFunction(MouseMoveFunction function)
    {
        m_mouseFunction = function;
    }

    bool GLWindow::isMouseRightButtonPressed(){
        int state = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);

        if(state == GLFW_PRESS){
            return true;
        }

        return false;
    }
    void GLWindow::setGuiFunction(RenderFunction function)
    {
        m_guiFunction = function;
    }

    void GLWindow::setKeyboardFunction(KeyboardFunction function)
    {
        m_keyboardFunction = function;
    }

    void GLWindow::begin(){
        while (!glfwWindowShouldClose(m_window))
        {            
            m_renderFunction();
         
            renderImgui();
            
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}