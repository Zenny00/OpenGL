#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

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

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath) {
    std::ifstream stream(filepath);
    
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // vertex
                type = ShaderType::VERTEX;
            } else {
                // fragment
                type = ShaderType::FRAGMENT;
            } 
        } else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() }; // Cast to ShaderProgramSource struct
}

// Notes:
// Vertex shaders find the position of each vertex in the window
// Fragment/pixel shader finds the color value for each pixel

static unsigned int CompileShader(unsigned int type, const std::string &source) {
    // Error handling
    GLCall(unsigned int id = glCreateShader(type)); // Since we don't define a scope in our macro, id is still excessible outside
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLCall(glClearColor(0.50f, 0.50f, 0.50f, 1.0f));
    std::cout << glGetString(GL_VERSION) << std::endl;

    const int NUM_POSITIONS = 6;
    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f, // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const int NUM_BUFFERS = 1;
    const int NUM_TRIANGLES = 2;
    unsigned int buffer;

    // Define a vertex buffer
    GLCall(glGenBuffers(NUM_BUFFERS, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, NUM_POSITIONS * NUM_TRIANGLES * sizeof(float), positions, GL_STATIC_DRAW));
    
    const int INDEX = 0;
    const int NUM_COMPONENTS = 2;
    const int STRIDE = sizeof(float) * NUM_COMPONENTS;
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(INDEX, NUM_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, 0));

    unsigned int ibo; // Index buffer object
    const int NUM_INDEX_BUFFERS = 1;
    const int SIZE_OF_INDICES = 6 * sizeof(unsigned int);

    // Generate an index buffer
    GLCall(glGenBuffers(NUM_INDEX_BUFFERS, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_OF_INDICES, indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("../res/shaders/BasicShader.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    // Find our uniform in the shader | This will return an index of -1 if the uniform doesn't exist or is not used in the shader
    GLCall(int colorUniformIndex = glGetUniformLocation(shader, "u_Color"));
    ASSERT(colorUniformIndex != -1);
    GLCall(glUniform4f(colorUniformIndex, 0.8f, 0.3f, 0.8f, 1.0f)); // Sending 4 floats to the shader via uniform

    float red_channel = 0.0f;
    float green_channel = 0.2f;
    float blue_channel = 0.2f;
    float increment = 0.005f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(colorUniformIndex, red_channel, green_channel, blue_channel, 1.0f));
        // Clear all existing errors/check for thrown errors
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // We can specify nullptr here for the index buffer as it has already been bound

        red_channel += increment;
        if (red_channel > 1.0f) increment = -increment;
        else if (red_channel < 0.0f) increment = -increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}