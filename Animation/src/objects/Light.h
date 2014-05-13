#ifndef LIGHT_H
#define LIGHT_H

#include "Object3D.h"

class Light
{
public:
    Light(QVector3D position = QVector3D(0,1,0),QVector3D direction = QVector3D(0,-1,0), QVector3D intensity = QVector3D(1,1,1));

    void setDirection(const QVector3D &direction);
    QVector3D& getDirection();
    void lookAt(const QVector3D &target);

    QVector3D& getIntensity();
    void setIntensity(const QVector3D &intensity);

    QVector3D& getPosition();
    void setPosition(const QVector3D &position);

    Matrix4& getProjMatrix();
    Matrix4& getProjMatrixCascade();
    Matrix4& getViewMatrix();
    Matrix4& getTextureProjMatrix(const Matrix4& modelMatrix);
    Matrix4& getTextureProjMatrixCascade(const Matrix4& modelMatrix);

    void resize(float ratio);
    void setSize(float s);

private:
    QVector3D intensity;
    QVector3D position;
    QVector3D direction;

    float size, sizeCascade;

    Matrix4 viewMatrix;
    Matrix4 projMatrix;
    Matrix4 biasMatrix;
    Matrix4 textureProjMatrix;
    Matrix4 projMatrixCascade;
    Matrix4 textureProjMatrixCascade;


};

#endif // LIGHT_H
