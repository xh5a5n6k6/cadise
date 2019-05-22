#pragma once

namespace cadise {

class Path;

class ImageIO {
public:
    static void save(Path path, int width, int height, unsigned char* data);

private:
    //static bool saveJPG();
    static bool _savePNG(Path path, int width, int height, unsigned char* data);
    //static bool saveBMP();
    static bool _savePPM(Path path, int width, int height, unsigned char* data);
};

} // namespace cadise