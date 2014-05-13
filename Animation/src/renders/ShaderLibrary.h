#ifndef SHADERLIBRARY_H
#define SHADERLIBRARY_H

#include "Shader.h"

class ShaderLibrary
{
public:
    static Shader* getShader(QString name);
    static Shader* addShader(QString name, QString shaderPrefix, QStringList attributes, QStringList uniforms);
    static void clear();

private:
    static QHash<QString, Shader*> shaders;
};

#endif // SHADERLIBRARY_H
