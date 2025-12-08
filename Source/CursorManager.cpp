#include "../Header/CursorManager.h"
#include "../Header/stb_image.h"
#include <iostream>

GLFWcursor* CursorManager::loadCursor(const std::string& filePath, float hotspotFactorX, float hotspotFactorY)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (!imageData) {
        std::cout << "Cursor nije ucitan! Putanja: " << filePath << std::endl;
        return nullptr;
    }

    GLFWimage img;
    img.width = width;
    img.height = height;
    img.pixels = imageData;

    int hotspotX = (int)(width * hotspotFactorX);
    int hotspotY = (int)(height * hotspotFactorY);

    GLFWcursor* cursor = glfwCreateCursor(&img, hotspotX, hotspotY);

    stbi_image_free(imageData);
    return cursor;
}