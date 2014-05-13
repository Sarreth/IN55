#ifndef MATERIALSUN_H
#define MATERIALSUN_H

#include "Material.h"

class MaterialSun : public Material
{
public:
    MaterialSun(Texture* diffuse, Texture* alpha);
    Shader* bind();

protected:
    Texture* diffuse;
    Texture* alpha;
};

#endif // MATERIALSUN_H
