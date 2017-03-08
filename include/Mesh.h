#ifndef MESH_H
#define MESH_H

#include <iostream>

#include "PhysicsDemo.h"

class Mesh
{
public:
    /* Draw the mesh with the supplied texture */
    virtual void Draw(Shader shader) = 0;
};

#endif // MESH_H
