#ifndef CINEMA_CUBERENDERER_H
#define CINEMA_CUBERENDERER_H

#include <glm/glm.hpp>

class CubeRenderer {
private:
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    unsigned int defaultTexture;

    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;

    bool roomLightOn = true;
    bool screenLightOn = false;

    void setupCube();

public:
    CubeRenderer();
    ~CubeRenderer();

    void init();
    void setMatrices(const glm::mat4& proj, const glm::mat4& view);
    void setCameraPosition(const glm::vec3& pos);

    void setRoomLight(bool on);
    void setScreenLight(bool on);

    void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
    void drawCubeRotatedY(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color, float angleRad);
    void drawCubeTextured(const glm::vec3& position, const glm::vec3& scale, unsigned int texture, const glm::vec3& tint = glm::vec3(1.0f));

};

#endif
