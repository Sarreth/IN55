#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

class Material
{
public:
    Material(Shader* shader);
    Material(Texture* diffuse, Texture* alpha);

    Shader* getShader();

    Shader* bind();

protected:
    Shader* shader;
    Texture* diffuse;
    Texture* alpha;
};

#endif // MATERIAL_H
