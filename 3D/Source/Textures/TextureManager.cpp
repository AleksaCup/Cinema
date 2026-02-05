#include "../Header/Textures/TextureManager.h"
#include "../Header/glad/glad.h"
#include "../Header/Util.h"
#include <vector>

static unsigned int createCheckerTexture(unsigned char r, unsigned char g, unsigned char b)
{
    constexpr int W = 32;
    constexpr int H = 32;
    std::vector<unsigned char> data(W * H * 4);

    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            int idx = (y * W + x) * 4;
            bool dark = (((x / 4) + (y / 4)) % 2) == 0;
            float f = dark ? 0.72f : 1.0f;
            data[idx + 0] = static_cast<unsigned char>(r * f);
            data[idx + 1] = static_cast<unsigned char>(g * f);
            data[idx + 2] = static_cast<unsigned char>(b * f);
            data[idx + 3] = 255;
        }
    }

    unsigned int tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

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
    textures["person1"]  = createCheckerTexture(180,  50,  60);
    textures["person2"]  = createCheckerTexture( 60, 120, 200);
    textures["person3"]  = createCheckerTexture( 40, 160, 120);
    textures["person4"]  = createCheckerTexture(200, 120,  40);
    textures["person5"]  = createCheckerTexture(150,  80, 180);
    textures["person6"]  = createCheckerTexture(220,  90, 120);
    textures["person7"]  = createCheckerTexture(120, 120, 120);
    textures["person8"]  = createCheckerTexture( 80,  70, 150);
    textures["person9"]  = createCheckerTexture(200, 170,  60);
    textures["person10"] = createCheckerTexture( 70, 170, 200);
    textures["person11"] = createCheckerTexture(210, 100,  70);
    textures["person12"] = createCheckerTexture(100, 180,  80);
    textures["person13"] = createCheckerTexture(150, 110,  70);
    textures["person14"] = createCheckerTexture(180,  60, 140);
    textures["person15"] = createCheckerTexture( 60,  90, 200);

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
    for (auto& kv : textures)
        glDeleteTextures(1, &kv.second);

    textures.clear();
}
