#include <signal.h>
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

#ifndef ERROR_CHECKER_H
#define ERROR_CHECKER_H

// Spooky MACROS
# define ASSERT(x) if (!(x)) raise(SIGTRAP);
# define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); // Pass x (use # to convert to string), file, and line to log call

bool GLLogCall(const char *function, const char *file, int line);

void GLClearError();

#endif