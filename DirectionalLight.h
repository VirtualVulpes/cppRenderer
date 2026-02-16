#ifndef CPPRENDERER_LIGHT_H
#define CPPRENDERER_LIGHT_H

#include "GameObject.h"

class DirectionalLight : public GameObject
{
public:
    DirectionalLight(const Transform& transform, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s, glm::vec3 tint = {1.0, 1.0, 1.0}, float strength = 1.0);
    void Draw() const override;
    void SetColor(glm::vec3 color) { tint_ = color; }

private:
    float strength_;
};


#endif