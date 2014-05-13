#ifndef MATERIAL_H
#define MATERIAL_H

#include "../objects/Object3D.h"
#include "../renders/Shader.h"

class Material
{
public:
    Material(QString shader);
    Material(Shader* shader);

    Shader* getShader();

    virtual Shader* bind(){return shader;}

protected:
    Shader* shader;
};


#endif // MATERIAL_H
