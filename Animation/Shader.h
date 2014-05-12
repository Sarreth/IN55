#ifndef SHADER_H
#define SHADER_H

#include "Texture.h"

class Shader : protected QOpenGLFunctions_3_2_Core
{
public:
    Shader(QString name);
    ~Shader();

    Shader& load(QString shader, QStringList attributes, QStringList uniforms);
    Shader& bind();
    Shader& setup();
    Shader& setupLocations(QStringList _attributes, QStringList _uniforms);

    QString& getName();

    unsigned int attribute(QString name);
    unsigned int uniform(QString name);

    bool hasAttribute(QString name);
    bool hasUniform(QString name);

    void transmitUniform(QString name, const Texture* tex);
    void transmitUniform(QString name, int i);
    void transmitUniform(QString name, float f);
    void transmitUniform(QString name, float f1, float f2);
    void transmitUniform(QString name, float f1, float f2, float f3);
    void transmitUniform(QString name, const QVector3D &vec3);
    void transmitUniform(QString name, const QMatrix3x3 &mat3);
    void transmitUniform(QString name, const QMatrix4x4 &mat4);
    void transmitUniform(QString name, bool b);

    char* getFileContent(QString path);

private:
    QString name;
    unsigned int uid;
    const char* vertex;
    const char* fragment;

    QHash<QString, unsigned int> attributes;
    QHash<QString, unsigned int> uniforms;
};

#endif // SHADER_H
