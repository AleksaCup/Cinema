#include "../Header/glad/glad.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

#include "SeatGrid.h"
#include "Simulation.h"
#include "../Header/Util.h"
#include "../Header/CursorManager.h"
#include "Renderers/RectRenderer.h"
#include "Renderers/SeatRenderer.h"
#include "Renderers/SignatureRenderer.h"

using namespace std::this_thread;
using namespace std::chrono;

SeatGrid* g_grid = nullptr;
RectRenderer rectRenderer;
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);

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

    GLFWcursor* cameraCursor = CursorManager::loadCursor("../Resources/camera-cursor-24.png");
    if (cameraCursor) glfwSetCursor(window, cameraCursor);

    // OpenGL setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.8f, 0.6f, 1.0f);

    // FPS limiter
    const double TARGET_FPS = 75.0;
    const double FRAME_TIME = 1.0 / TARGET_FPS;
    double lastFrame = glfwGetTime();

    rectRenderer.init();

    //signature
    unsigned int sigShader, sigVAO, sigTex;
    initSignatureRendering(sigShader, sigVAO, sigTex, mode->width, mode->height);

    //seats
    unsigned int seatShader, seatVAO;
    initSeatRendering(seatShader, seatVAO);

    SeatGrid grid(10, 12, 0.035f);
    g_grid = &grid;
    glfwSetMouseButtonCallback(window, mouseClickCallback);
    Simulation sim(&grid, &rectRenderer);

    bool keyHandled[10] = { false }; //ovo mi treba da resim bug za kupovinu vise sedista
    while (!glfwWindowShouldClose(window))
    {
        //escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //kupovina
        for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; key++)
        {
            int idx = key - GLFW_KEY_0;

            if (glfwGetKey(window, key) == GLFW_PRESS)
            {
                if (!keyHandled[idx])
                {
                    keyHandled[idx] = true;

                    int count = idx;
                    auto seatsToBuy = grid.findContiguousFreeSeats(count);
                    if (!seatsToBuy.empty())
                        grid.markBought(seatsToBuy);
                }
            }
            else
            {
                keyHandled[idx] = false;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            sim.start();


        //fps limit
        double now = glfwGetTime();
        double delta = now - lastFrame;

        if (delta < FRAME_TIME) {
            sleep_for(duration<double>(FRAME_TIME - delta));
            continue;
        }

        lastFrame = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT);

        sim.update(delta);

        glUseProgram(seatShader);
        grid.draw(seatShader, seatVAO);

        sim.draw();

        drawSignatureRendering(sigShader, sigVAO, sigTex);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    //refernca na globalni grid
    if (!g_grid) return;

    //pozicija kursora
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    //dimenzije windowa
    int w, h;
    glfwGetWindowSize(window, &w, &h);

    // piksel kordinate u NDC
    float ndcX = (mx / w) * 2.0f - 1.0f;
    float ndcY = -((my / h) * 2.0f - 1.0f);

    g_grid->toggleSeat(ndcX, ndcY);
}

