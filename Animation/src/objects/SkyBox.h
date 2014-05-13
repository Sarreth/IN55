#ifndef SKYBOX_H
#define SKYBOX_H

#include "Mesh.h"
#include "../materials/MaterialSkyBox.h"

class SkyBox : public Mesh
{
public:
    SkyBox(MaterialSkyBox* mat);
};

#endif // SKYBOX_H
