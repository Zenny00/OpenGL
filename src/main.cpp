#include <fstream>
#include <string>
#include <sstream>

#include "ErrorChecker.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

// Notes:
// Vertex shaders find the position of each vertex in the window
// Fragment/pixel shader finds the color value for each pixel

int main() {
    const int NUM_BUFFERS = 1;
    const int NUM_FLOATS = 4;
    const int NUM_INDEX_BUFFERS = 1;
    const int INDICES_COUNT  = 6;
    const int NUM_VERTICES = 4;

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
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        float red_channel = 0.1f;
        float green_channel = 0.2f;
        float blue_channel = 0.8f;
        float increment = 0.05f;

        Renderer renderer;

        /* Loop until the user closes the window */
        float x = 0.7f;
        float y = 0.7f;
        float x_increment = 0.005f;
        float y_increment = 0.005f;

        float positions[] = {
                -x, -y, 0.0f, 0.0f, // 0
                 x, -y, 1.0f, 0.0f, // 1
                 x,  y, 1.0f, 1.0f, // 2
                -x,  y, 0.0f, 1.0f  // 3
            };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        Texture texture("../res/textures/rock_wall.png");
        texture.Bind();
        while (!glfwWindowShouldClose(window))
        {
            // Vertex array
            // Vertex buffer is linked to vertex array object
            VertexArray va;

            // Define a vertex buffer
            // Automatically bound by Vertex array object
            VertexBuffer vb(positions, NUM_VERTICES * NUM_FLOATS * sizeof(float));
            
            VertexBufferLayout layout;
            layout.Push<float>(2);
            layout.Push<float>(2);
            va.AddBuffer(vb, layout);

            // Generate an index buffer
            IndexBuffer ib(indices, INDICES_COUNT);


            Shader shader("/home/zenny/Desktop/OpenGL/res/shaders/BasicShader.shader");
            shader.Bind();
            shader.SetUniform1i("u_Texture", 0);

            /* Render here */
            renderer.Clear();
            renderer.Draw(va, ib, shader);

            // Unbind vertex array, shader, buffer, and index buffer
            va.Unbind();
            shader.Unbind();
            vb.Unbind();
            ib.Unbind();

            /*
            // Update values
            red_channel += increment;
            if (red_channel > 1.0f || red_channel < 0.0f) increment = -increment;
            */

            for (int i = 0; i < 16; i+=4) {
                positions[i] += x_increment;
                if (positions[i] >= 1.0f || positions[i] <= -1.0f) x_increment = -x_increment;
            }

            /*
            y += y_increment;
            if (y > 1.0f || y < 0.0f) y_increment = -y_increment;
            */

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}