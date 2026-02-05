#ifndef ROOM_RENDERER_H
#define ROOM_RENDERER_H

#include <glm/glm.hpp>
#include "CubeRenderer.h"


class RoomRenderer {
public:
    void draw(const glm::mat4& projection, const glm::mat4& view, CubeRenderer& renderer, const glm::vec3& cameraPos, const glm::vec3& cameraFront, float doorOpen, bool showStaticScreen);

private:
    // void drawSeats(CubeRenderer& renderer, const glm::vec3& cameraPos, const glm::vec3& cameraFront);
    void drawScreen(CubeRenderer& renderer);
    void drawDoors(CubeRenderer& renderer, float doorOpen);
};

#endif
