#include <fstream>
#include <string>
#include <sstream>

#include "ErrorChecker.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

// Notes:
// Vertex shaders find the position of each vertex in the window
// Fragment/pixel shader finds the color value for each pixel

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        const int NUM_VERTICES = 4;
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
        const int NUM_INDEX_BUFFERS = 1;
        const int INDICES_COUNT  = 6;

        // Vertex array
        // Vertex buffer is linked to vertex array object
        VertexArray va;

        // Define a vertex buffer
        // Automatically bound by Vertex array object
        VertexBuffer vb(positions, NUM_VERTICES * NUM_TRIANGLES * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Generate an index buffer
        IndexBuffer ib(indices, INDICES_COUNT);

        Shader shader("/home/zenny/Desktop/OpenGL/res/shaders/BasicShader.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        
        float red_channel = 0.1f;
        float green_channel = 0.2f;
        float blue_channel = 0.8f;
        float increment = 0.05f;

        // Unbind vertex array, shader, buffer, and index buffer
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", red_channel, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            red_channel += increment;
            if (red_channel > 1.0f) increment = -increment;
            else if (red_channel < 0.0f) increment = -increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}