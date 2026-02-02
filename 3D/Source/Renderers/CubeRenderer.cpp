#include "Renderers/CubeRenderer.h"
#include "../Header/glad/glad.h"
#include "../Header/Util.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CubeRenderer::CubeRenderer() : VAO(0), VBO(0), shaderProgram(0) {}

CubeRenderer::~CubeRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void CubeRenderer::init() {
    setupCube();
    shaderProgram = createShader(
        "../Shaders/cube.vert",
        "../Shaders/cube.frag"
    );
}

void CubeRenderer::setMatrices(const glm::mat4& proj, const glm::mat4& v) {
    projection = proj;
    view = v;
}

void CubeRenderer::setCameraPosition(const glm::vec3& pos) {
    cameraPos = pos;
}

void CubeRenderer::setRoomLight(bool on)
{
    roomLightOn = on;
}

void CubeRenderer::setScreenLight(bool on)
{
    screenLightOn = on;
}

void CubeRenderer::drawCube(const glm::vec3& position,
                            const glm::vec3& size,
                            const glm::vec3& color)
{
    glUseProgram(shaderProgram);

    //Model matrix
    glm::mat4 uM(1.0f);
    uM = glm::translate(uM, position);
    uM = glm::scale(uM, size);

    //Matrices
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uM"),
                       1, GL_FALSE, glm::value_ptr(uM));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uV"),
                       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uP"),
                       1, GL_FALSE, glm::value_ptr(projection));

    //Camera / view pos
    glUniform3fv(glGetUniformLocation(shaderProgram, "uViewPos"),
                 1, glm::value_ptr(cameraPos));

    //Light toggles
    glUniform1i(glGetUniformLocation(shaderProgram, "uRoomLightOn"),
                roomLightOn ? 1 : 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "uScreenLightOn"),
                screenLightOn ? 1 : 0);

    //Room light
    glUniform3f(glGetUniformLocation(shaderProgram, "uRoomLight.pos"),
                0.0f, 6.0f, 6.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uRoomLight.kA"),
                0.20f, 0.20f, 0.20f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uRoomLight.kD"),
                1.00f, 1.00f, 1.00f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uRoomLight.kS"),
                1.00f, 1.00f, 1.00f);

    //Screen light
    glUniform3f(glGetUniformLocation(shaderProgram, "uScreenLight.pos"),
                0.0f, 2.6f, -1.8f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uScreenLight.kA"),
                0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uScreenLight.kD"),
                1.00f, 1.00f, 1.00f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uScreenLight.kS"),
                1.00f, 1.00f, 1.00f);

    //Material
    glUniform3f(glGetUniformLocation(shaderProgram, "uMaterial.kA"),
                color.r, color.g, color.b);
    glUniform3f(glGetUniformLocation(shaderProgram, "uMaterial.kD"),
                color.r, color.g, color.b);
    glUniform3f(glGetUniformLocation(shaderProgram, "uMaterial.kS"),
                1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "uMaterial.shine"),
                32.0f);

    //Draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void CubeRenderer::setupCube() {
    float vertices[] = {
        // positions          // normals
        // Back
        -0.5f,-0.5f,-0.5f,  0,0,-1,  0.5f,-0.5f,-0.5f,  0,0,-1,  0.5f, 0.5f,-0.5f,  0,0,-1,
         0.5f, 0.5f,-0.5f,  0,0,-1, -0.5f, 0.5f,-0.5f,  0,0,-1, -0.5f,-0.5f,-0.5f,  0,0,-1,

        // Front
        -0.5f,-0.5f, 0.5f,  0,0, 1,  0.5f,-0.5f, 0.5f,  0,0, 1,  0.5f, 0.5f, 0.5f,  0,0, 1,
         0.5f, 0.5f, 0.5f,  0,0, 1, -0.5f, 0.5f, 0.5f,  0,0, 1, -0.5f,-0.5f, 0.5f,  0,0, 1,

        // Left
        -0.5f, 0.5f, 0.5f, -1,0,0, -0.5f, 0.5f,-0.5f, -1,0,0, -0.5f,-0.5f,-0.5f, -1,0,0,
        -0.5f,-0.5f,-0.5f, -1,0,0, -0.5f,-0.5f, 0.5f, -1,0,0, -0.5f, 0.5f, 0.5f, -1,0,0,

        // Right
         0.5f, 0.5f, 0.5f,  1,0,0,  0.5f, 0.5f,-0.5f,  1,0,0,  0.5f,-0.5f,-0.5f,  1,0,0,
         0.5f,-0.5f,-0.5f,  1,0,0,  0.5f,-0.5f, 0.5f,  1,0,0,  0.5f, 0.5f, 0.5f,  1,0,0,

        // Bottom
        -0.5f,-0.5f,-0.5f,  0,-1,0,  0.5f,-0.5f,-0.5f,  0,-1,0,  0.5f,-0.5f, 0.5f,  0,-1,0,
         0.5f,-0.5f, 0.5f,  0,-1,0, -0.5f,-0.5f, 0.5f,  0,-1,0, -0.5f,-0.5f,-0.5f,  0,-1,0,

        // Top
        -0.5f, 0.5f,-0.5f,  0,1,0,   0.5f, 0.5f,-0.5f,  0,1,0,   0.5f, 0.5f, 0.5f,  0,1,0,
         0.5f, 0.5f, 0.5f,  0,1,0,  -0.5f, 0.5f, 0.5f,  0,1,0,  -0.5f, 0.5f,-0.5f,  0,1,0
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
