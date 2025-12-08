#pragma once
#include <GLFW/glfw3.h>
#include <string>

class CursorManager {
public:
    static GLFWcursor* loadCursor(const std::string& filePath, float hotspotFactorX = 0.2f, float hotspotFactorY = 0.2f);
};
