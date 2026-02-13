//
// Created by sjfochs on 2/12/26.
//

#ifndef CPPRENDERER_TEXTURE_H
#define CPPRENDERER_TEXTURE_H
#include <string_view>

class Texture
{
public:
    Texture();
    void LoadFromFile(std::string_view path);
    unsigned int GetId() { return id_; }
private:
    unsigned int id_;

    int width_;
    int height_;
    int num_channels_;
};


#endif //CPPRENDERER_TEXTURE_H