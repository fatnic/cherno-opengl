#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ErrorCallbacks.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int main()
{
    glfwSetErrorCallback(glfwErrorCall);

     GLFWwindow* window;

     if (!glfwInit())
         return -1;

     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     window = glfwCreateWindow(960, 540, "GLFWWindow", nullptr, nullptr);
     if (!window) {
         glfwTerminate();
         return -1;
     }

     glfwMakeContextCurrent(window);

     GLenum err = glewInit();
     if (GLEW_OK != err) {
         std::cout << glewGetErrorString(err) << std::endl;
         return -1;
     }

     glEnable(GL_DEBUG_OUTPUT);
     glDebugMessageCallback(GLErrorCallback, nullptr);

     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

     float positions[] = {
         -50.f, -50.f,      0.0f, 0.0f,
          50.f, -50.f,      1.0f, 0.0f,
          50.f,  50.f,      1.0f, 1.0f,
         -50.f,  50.f,      0.0f, 1.0f
     };

     unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
     };

     unsigned int vao;
     glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     VertexArray va;
     VertexBuffer vb(positions, sizeof(positions));
     VertexBufferLayout layout;
     layout.push_float(2);
     layout.push_float(2);
     va.addBuffer(vb, layout);

     IndexBuffer ib(indices, 6);

     Shader shader("res/shaders/basic.shader");

     glm::mat4 proj  = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
     glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


     Texture texture("res/textures/jess2.png");
     texture.bind(0);
     shader.setUniform1i("u_Texture", 0);

     Renderer renderer;

     ImGui::CreateContext();
     ImGui_ImplGlfw_InitForOpenGL(window, true);
     ImGui_ImplOpenGL3_Init("#version 330 core");
     ImGui::StyleColorsDark();

     glm::vec3 translation(200, 200, 0);

     while(!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

         glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
         glm::mat4 mvp   = proj * view * model;
         shader.setUniformMat4f("u_MVP", mvp);

         renderer.clear();
         renderer.draw(va, ib, shader);

        {
            ImGui::Begin("Image");
            ImGui::SliderFloat("x", &translation.x, 0.0f, 960.0f);
            ImGui::SliderFloat("y", &translation.y, 0.0f, 540.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }


         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         glfwSwapBuffers(window);
         glfwPollEvents();
     }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
