#include "Material.h"

Material::Material(Shader* shader)
{
    this->shader = shader;
}

Shader* Material::getShader()
{
    return this->shader;
}

Material::Material(Texture *diffuse, Texture *alpha)
{

    Shader* tmp = new Shader("rt_sun");
    tmp->load( "shaders/rt_sun", QStringList()<<"position"<<"texcoord",QStringList()<<"mvp"<<"texDiffuse"<<"texAlpha");
    this->shader = tmp;
    this->diffuse = diffuse;
    this->alpha = alpha;
}

Shader* Material::bind()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->bind();

    diffuse->bind();
    shader->transmitUniform("texDiffuse", diffuse);

    alpha->bind();
    shader->transmitUniform("texAlpha", alpha);

    return shader;
}
