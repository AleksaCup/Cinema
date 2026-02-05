#pragma once
#include <unordered_map>
#include <string>

class TextureManager {
public:
    static void init();
    static unsigned int get(const std::string& name);
    static void cleanup();

private:
    static std::unordered_map<std::string, unsigned int> textures;
};
