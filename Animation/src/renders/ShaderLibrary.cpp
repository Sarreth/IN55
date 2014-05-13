#include "ShaderLibrary.h"

QHash<QString, Shader*> ShaderLibrary::shaders;

Shader* ShaderLibrary::getShader(QString name)
{
    if(shaders.contains(name))
        return shaders.value(name);

}

Shader* ShaderLibrary::addShader(QString name, QString shaderPrefix, QStringList attributes, QStringList uniforms)
{
    if(shaders.contains(name))
        return shaders.value(name);

    Shader* shader = new Shader(name);
    shader->load(shaderPrefix, attributes, uniforms);

    shaders.insert(name, shader);

    return shader;
}

void ShaderLibrary::clear()
{
    foreach(Shader* shader, shaders)
        delete shader;
    shaders.clear();
}
