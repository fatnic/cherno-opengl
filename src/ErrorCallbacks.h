#pragma once

void GLAPIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL ERROR: %s type: 0x%x, severity: 0x%x, message: %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}


void glfwErrorCall(int, const char* err) {
    std::cout << "GLFW error: " << err << std::endl;
}
