#include <signal.h>
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

// Spooky MACROS
# define ASSERT(x) if (!(x)) raise(SIGTRAP);
# define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); // Pass x (use # to convert to string), file, and line to log call

static bool GLLogCall(const char *function, const char *file, int line) {
    // Loop while there are errors remaining
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] " << "(" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

static void GLClearError() {
    // Keep looping until no errors remain
    while (glGetError() != GL_NO_ERROR);
}