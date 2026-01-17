#ifndef CINEMA_CUBERENDERER_H
#define CINEMA_CUBERENDERER_H

#include <glm/glm.hpp>

class CubeRenderer {
private:
    unsigned int VAO, VBO;
    unsigned int shaderProgram;

    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;

    void setupCube();

public:
    CubeRenderer();
    ~CubeRenderer();

    void init();
    void setMatrices(const glm::mat4& proj, const glm::mat4& view);
    void setCameraPosition(const glm::vec3& pos);

    void drawCube(const glm::vec3& position,
                  const glm::vec3& size,
                  const glm::vec3& color);
};

#endif
