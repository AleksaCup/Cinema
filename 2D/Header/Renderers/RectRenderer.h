#pragma once
#include "../../Header/glad/glad.h"

class RectRenderer {
public:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int shader = 0;

    void init();
    void draw(float x, float y, float w, float h, float r, float g, float b, float a);
};
