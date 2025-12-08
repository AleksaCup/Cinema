#pragma once
#include <glad/glad.h>

void initSignatureRendering(unsigned int& shader, unsigned int& vao, unsigned int& tex, int w, int h);

void drawSignatureRendering(unsigned int shader, unsigned int vao, unsigned int tex);
