#pragma once

class PersonRenderer {
public:
    unsigned int shader = 0;
    unsigned int vao    = 0;
    unsigned int vbo    = 0;

    void init();
    void draw(float x, float y, float size, unsigned int texID, bool flipY);

};
