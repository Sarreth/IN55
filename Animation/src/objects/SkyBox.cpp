#include "SkyBox.h"

SkyBox::SkyBox(MaterialSkyBox* mat) : Mesh()
{
    float size = 1.0;
    float vertices[] = {
      -size,  size,  size,
      -size, -size,  size,
       size, -size,  size,
       size,  size,  size,
      -size,  size, -size,
      -size, -size, -size,
       size, -size, -size,
       size,  size, -size,
    };

     int indices[] = {
        0, 1, 2, 3,
        3, 2, 6, 7,
        7, 6, 5, 4,
        4, 5, 1, 0,
        0, 3, 7, 4,
        1, 2, 6, 5,
    };

    geometry = new Geometry(vertices, indices, 24, 24);
    geometry->setPrimitive(GL_QUADS);
    material = mat;
}
