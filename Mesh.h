//
// Created by sjfochs on 2/12/26.
//

#ifndef CPPRENDERER_MESH_H
#define CPPRENDERER_MESH_H


class Mesh
{
public:
    Mesh();
    ~Mesh();
    void BindVertexArray();
private:
    unsigned int vao_;
    unsigned int vbo_;
};


#endif //CPPRENDERER_MESH_H