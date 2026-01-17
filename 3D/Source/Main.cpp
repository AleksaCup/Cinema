#include "../Header/glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <chrono>
#include <iostream>

#include "Camera.h"
#include "Renderers/CubeRenderer.h"
#include "Renderers/RoomRenderer.h"

using namespace std::this_thread;
using namespace std::chrono;

// Global
Camera* g_camera = nullptr;
bool g_firstMouse = true;
float g_lastX = 0.0f;
float g_lastY = 0.0f;

// Callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window, float deltaTime);

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(
        mode->width,
        mode->height,
        "Cinema 3D",
        monitor,
        nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    int fbW, fbH;
    glfwGetFramebufferSize(window, &fbW, &fbH);
    glViewport(0, 0, fbW, fbH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    //input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    g_lastX = fbW * 0.5f;
    g_lastY = fbH * 0.5f;

    //camera
    Camera camera(
        glm::vec3(0.0f, 2.0f, 8.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f
    );
    g_camera = &camera;

    //renderers
    CubeRenderer cubeRenderer;
    cubeRenderer.init();

    RoomRenderer roomRenderer;

    //fps limiter
    const double TARGET_FPS = 75.0;
    const double FRAME_TIME = 1.0 / TARGET_FPS;
    double lastFrame = glfwGetTime();

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - lastFrame;

        if (deltaTime < FRAME_TIME) {
            sleep_for(duration<double>(FRAME_TIME - deltaTime));
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
        }
        lastFrame = currentFrame;

        // Input
        processInput(window, (float)deltaTime);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Viewport using framebuffer
        glfwGetFramebufferSize(window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);

        // Matrices
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)fbW / (float)fbH,
            0.1f,
            100.0f
        );

        glm::mat4 view = camera.getViewMatrix();

        cubeRenderer.setCameraPosition(camera.Position);
        cubeRenderer.setMatrices(projection, view);

        // TEST KOCKA (MORA DA SE VIDI)
        cubeRenderer.drawCube(
            glm::vec3(0.0f, 1.0f, -5.0f),
            glm::vec3(1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        // Room
        roomRenderer.draw(projection, view, cubeRenderer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//input

void processInput(GLFWwindow* window, float deltaTime)
{
    if (!g_camera) return;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera->processKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!g_camera) return;

    if (g_firstMouse) {
        g_lastX = (float)xpos;
        g_lastY = (float)ypos;
        g_firstMouse = false;
    }

    float xoffset = (float)xpos - g_lastX;
    float yoffset = g_lastY - (float)ypos;

    g_lastX = (float)xpos;
    g_lastY = (float)ypos;

    g_camera->processMouseMovement(xoffset, yoffset);
}
