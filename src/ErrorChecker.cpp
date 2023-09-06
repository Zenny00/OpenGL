#include "ErrorChecker.h"

bool GLLogCall(const char *function, const char *file, int line) {
    // Loop while there are errors remaining
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] " << "(" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void GLClearError() {
    // Keep looping until no errors remain
    while (glGetError() != GL_NO_ERROR);
}