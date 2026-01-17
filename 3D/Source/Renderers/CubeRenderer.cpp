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

void CubeRenderer::drawCube(const glm::vec3& position,
                            const glm::vec3& size,
                            const glm::vec3& color)
{
    glUseProgram(shaderProgram);

    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, size);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
                       1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
                       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));

    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"),
                 1, glm::value_ptr(color));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"),
                 1, glm::value_ptr(cameraPos));
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"),
                0.0f, 5.0f, 5.0f);

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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
