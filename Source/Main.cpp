#include "../Header/glad/glad.h"
#include <GLFW/glfw3.h>
#include <__thread/this_thread.h>
#include "../Header/Util.h"

using namespace std::this_thread;
using namespace std::chrono;

unsigned int signatureVAO;
unsigned int signatureVBO;
unsigned int signatureShader;
unsigned int signatureTex;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Cinema Main", monitor, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return endProgram("GLAD nije uspeo da se inicijalizuje.");

    signatureTex = loadImageToTexture("../Resources/signature.png");

    GLFWcursor* cameraCursor = loadImageToCursor("../Resources/camera-cursor-24.png");
    if (cameraCursor != nullptr) glfwSetCursor(window, cameraCursor);

    // OpenGL setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.8f, 0.6f, 1.0f);

    // FPS limiter
    const double TARGET_FPS = 75.0;
    const double FRAME_TIME = 1.0 / TARGET_FPS;
    double lastFrame = glfwGetTime();

    signatureShader = createShader("../Shaders/signature.vert", "../Shaders/signature.frag");
    glUseProgram(signatureShader);
    glUniform1i(glGetUniformLocation(signatureShader, "uTex"), 0);


    int sigW = 500;
    int sigH = 128;
    int margin = 20;

    float x1 = mode->width  - sigW - margin;
    float y1 = mode->height - sigH - margin;
    float x2 = x1 + sigW;
    float y2 = y1 + sigH;

    float x1N = (x1 / mode->width)  * 2.0f - 1.0f;
    float x2N = (x2 / mode->width)  * 2.0f - 1.0f;
    float y1N = -((y1 / mode->height) * 2.0f - 1.0f);
    float y2N = -((y2 / mode->height) * 2.0f - 1.0f);

    float signatureVertices[] = {
        // pos        // uv
        x1N, y2N,     0.0f, 0.0f,   // top-left
        x1N, y1N,     0.0f, 1.0f,   // bottom-left
        x2N, y1N,     1.0f, 1.0f,   // bottom-right
        x2N, y2N,     1.0f, 0.0f    // top-right
    };


    glGenVertexArrays(1, &signatureVAO);
    glGenBuffers(1, &signatureVBO);

    glBindVertexArray(signatureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, signatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(signatureVertices), signatureVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        //escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //fps limit
        double now = glfwGetTime();
        double delta = now - lastFrame;

        if (delta < FRAME_TIME) {
            sleep_for(duration<double>(FRAME_TIME - delta));
            continue;
        }

        lastFrame = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(signatureShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, signatureTex);
        glBindVertexArray(signatureVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
