#ifndef PERSON_RENDERER_H
#define PERSON_RENDERER_H

#include <glm/glm.hpp>

class CubeRenderer;

class PersonRenderer {
public:
    static void draw(CubeRenderer& renderer, const glm::vec3& basePos, unsigned int textureId);
};

#endif
