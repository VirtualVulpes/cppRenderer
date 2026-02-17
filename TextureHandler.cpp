#include "TextureHandler.h"

#include <filesystem>

void TextureHandler::Load(std::string_view name, std::string_view path)
{
    name_to_id_[name.data()] = ++id_;
    Texture texture{path};
    id_to_texture_[id_] = std::make_unique<Texture>(path);
}

void TextureHandler::LoadFromFolder(std::string_view path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        Load(path);
    }
}

Texture* TextureHandler::Get(std::string_view name)
{
    uint32_t id = name_to_id_[name.data()];
    return id_to_texture_[id].get();
}
