#include "../Header/Textures/TextureManager.h"
#include "../Header/Util.h"

std::unordered_map<std::string, unsigned int> TextureManager::textures;

void TextureManager::init()
{
    //seats
    textures["seat_free"]     = loadImageToTexture("../Resources/Seat/seat_texture_green.png");
    textures["seat_reserved"] = loadImageToTexture("../Resources/Seat/seat_texture_yellow.png");
    textures["seat_bought"]   = loadImageToTexture("../Resources/Seat/seat_texture_red.png");

    // screens
    textures["screen1"]  = loadImageToTexture("../Resources/Screen/1.png");
    textures["screen2"]  = loadImageToTexture("../Resources/Screen/2.png");
    textures["screen3"]  = loadImageToTexture("../Resources/Screen/3.png");
    textures["screen4"]  = loadImageToTexture("../Resources/Screen/4.png");
    textures["screen5"]  = loadImageToTexture("../Resources/Screen/5.png");
    textures["screen6"]  = loadImageToTexture("../Resources/Screen/6.png");
    textures["screen7"]  = loadImageToTexture("../Resources/Screen/7.png");
    textures["screen8"]  = loadImageToTexture("../Resources/Screen/8.png");
    textures["screen9"]  = loadImageToTexture("../Resources/Screen/9.png");
    textures["screen10"] = loadImageToTexture("../Resources/Screen/10.png");
    textures["screen11"] = loadImageToTexture("../Resources/Screen/11.png");
    textures["screen12"] = loadImageToTexture("../Resources/Screen/12.png");
    textures["screen13"] = loadImageToTexture("../Resources/Screen/13.png");
    textures["screen14"] = loadImageToTexture("../Resources/Screen/14.png");
    textures["screen15"] = loadImageToTexture("../Resources/Screen/15.png");
    textures["screen16"] = loadImageToTexture("../Resources/Screen/16.png");
    textures["screen17"] = loadImageToTexture("../Resources/Screen/17.png");
    textures["screen18"] = loadImageToTexture("../Resources/Screen/18.png");
    textures["screen19"] = loadImageToTexture("../Resources/Screen/19.png");
    textures["screen20"] = loadImageToTexture("../Resources/Screen/20.png");

    // people
    // textures["person1"]  = loadImageToTexture("../Resources/Person/1.png");
    // textures["person2"]  = loadImageToTexture("../Resources/Person/2.png");
    // textures["person3"]  = loadImageToTexture("../Resources/Person/3.png");
    // textures["person4"]  = loadImageToTexture("../Resources/Person/4.png");
    // textures["person5"]  = loadImageToTexture("../Resources/Person/5.png");
    // textures["person6"]  = loadImageToTexture("../Resources/Person/6.png");
    // textures["person7"]  = loadImageToTexture("../Resources/Person/7.png");
    // textures["person8"]  = loadImageToTexture("../Resources/Person/8.png");
    // textures["person9"]  = loadImageToTexture("../Resources/Person/9.png");
    // textures["person10"] = loadImageToTexture("../Resources/Person/10.png");
    // textures["person11"] = loadImageToTexture("../Resources/Person/11.png");
    // textures["person12"] = loadImageToTexture("../Resources/Person/12.png");
    // textures["person13"] = loadImageToTexture("../Resources/Person/13.png");
    // textures["person14"] = loadImageToTexture("../Resources/Person/14.png");
    // textures["person15"] = loadImageToTexture("../Resources/Person/15.png");

}

unsigned int TextureManager::get(const std::string& name)
{
    return textures.at(name);
}

void TextureManager::cleanup()
{
    for (auto& [_, tex] : textures)
        glDeleteTextures(1, &tex);

    textures.clear();
}
