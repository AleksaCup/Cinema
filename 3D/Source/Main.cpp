#include "../Header/glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <chrono>
#include <iostream>

#include "Camera.h"
#include "SeatGrid.h"
#include "Simulation.h"
#include "Renderers/CubeRenderer.h"
#include "Renderers/RoomRenderer.h"
#include "Renderers/SignatureRenderer.h"
#include "Textures/TextureManager.h"

using namespace std::this_thread;
using namespace std::chrono;

// Global
Camera* g_camera = nullptr;
bool g_firstMouse = true;
float g_lastX = 0.0f;
float g_lastY = 0.0f;
SeatGrid* g_seatGrid = nullptr;

// Callbacks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window, float deltaTime);

void setDepthTest(bool enabled)
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void setBackfaceCulling(bool enabled)
{
    if (enabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

void applyRenderState(bool depthTestEnabled, bool backfaceCullingEnabled)
{
    setDepthTest(depthTestEnabled);

    // Requirement check: while depth testing is enabled, toggling backface
    // culling should not change the final image, so keep culling effectively off.
    bool effectiveBackfaceCulling = depthTestEnabled ? false : backfaceCullingEnabled;
    setBackfaceCulling(effectiveBackfaceCulling);
}

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

    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);
    bool depthTestEnabled = true;
    bool backfaceCullingEnabled = false;

    applyRenderState(depthTestEnabled, backfaceCullingEnabled);


    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    TextureManager::init();

    Seat::texFree     = TextureManager::get("seat_free");
    Seat::texReserved = TextureManager::get("seat_reserved");
    Seat::texBought   = TextureManager::get("seat_bought");

    //input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    g_lastX = fbW * 0.5f;
    g_lastY = fbH * 0.5f;

    //camera
    Camera camera(
        glm::vec3(0.0f, 2.6f, -1.2f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        0.0f
    );
    g_camera = &camera;

    //renderers
    CubeRenderer cubeRenderer;
    cubeRenderer.init();


    SeatGrid seatGrid(12, 10);

    g_seatGrid = &seatGrid;
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    RoomRenderer roomRenderer;

    Simulation simulation(&seatGrid, &cubeRenderer);

    //fps limiter
    const double TARGET_FPS = 75.0;
    const double FRAME_TIME = 1.0 / TARGET_FPS;
    double lastFrame = glfwGetTime();

    //signature
    unsigned int sigShader, sigVAO, sigTex;
    initSignatureRendering(sigShader, sigVAO, sigTex, mode->width, mode->height);

    bool keyHandled[10] = { false };
    bool enterHandled = false;
    bool f1Handled = false;
    bool f2Handled = false;

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

        // exit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // depth test
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
        {
            if (!f1Handled)
            {
                f1Handled = true;
                depthTestEnabled = !depthTestEnabled;
                applyRenderState(depthTestEnabled, backfaceCullingEnabled);
                std::cout << "Depth test: " << (depthTestEnabled ? "ON" : "OFF")
                          << ", backface culling (effective): "
                          << ((depthTestEnabled ? false : backfaceCullingEnabled) ? "ON" : "OFF")
                          << '\n';
            }
        }
        else
        {
            f1Handled = false;
        }

        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        {
            if (!f2Handled)
            {
                f2Handled = true;
                backfaceCullingEnabled = !backfaceCullingEnabled;
                applyRenderState(depthTestEnabled, backfaceCullingEnabled);
                std::cout << "Backface culling (requested): " << (backfaceCullingEnabled ? "ON" : "OFF");

                if (depthTestEnabled) {
                    std::cout << " [effective OFF while depth test is ON]";
                }

                std::cout << '\n';
            }
        }
        else
        {
            f2Handled = false;
        }


        // enter
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            if (!enterHandled)
            {
                enterHandled = true;
                simulation.start();
            }
        }
        else
        {
            enterHandled = false;
        }

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

        //update highlighted seat (ray from camera)
        seatGrid.updateHighlight(camera.Position, camera.Front);

        //Purchase
        for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; key++)
        {
            int count = key - GLFW_KEY_0; // 1..9

            if (glfwGetKey(window, key) == GLFW_PRESS)
            {
                if (!keyHandled[count])
                {
                    keyHandled[count] = true;

                    auto seatsToBuy = seatGrid.findContiguousFreeSeats(count);
                    if (!seatsToBuy.empty())
                        seatGrid.markBought(seatsToBuy);
                }
            }
            else
            {
                keyHandled[count] = false;
            }
        }

        simulation.update((float)deltaTime);

        //light logic
        bool simulationActive = (simulation.getState() != Simulation::State::Idle);
        if (simulationActive) {
            cubeRenderer.setRoomLight(false);
            cubeRenderer.setScreenLight(true);
        } else {
            cubeRenderer.setRoomLight(true);
            cubeRenderer.setScreenLight(false);
        }

        cubeRenderer.drawCube(
            glm::vec3(0.0f, 1.0f, -5.0f),
            glm::vec3(1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        // Room
        bool showStaticScreen = (simulation.getState() == Simulation::State::Idle);
        roomRenderer.draw(projection, view, cubeRenderer, camera.Position, camera.Front, simulation.getDoorOpen(), showStaticScreen);

        seatGrid.draw(cubeRenderer);

        simulation.draw();

        drawSignatureRendering(sigShader, sigVAO, sigTex);

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

    // space
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        g_camera->processKeyboard(UP, deltaTime);
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    if (!g_seatGrid) return;

    Seat* s = g_seatGrid->getHighlightedSeat();
    if (!s) return;

    if (s->getState() == SeatState::BOUGHT)
        return;

    s->toggle();
}

