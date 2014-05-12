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

    QMatrix4x4& getProjMatrix();
    QMatrix4x4& getProjMatrixCascade();
    QMatrix4x4& getViewMatrix();
    QMatrix4x4& getTextureProjMatrix(const QMatrix4x4& modelMatrix);
    QMatrix4x4& getTextureProjMatrixCascade(const QMatrix4x4& modelMatrix);

    QMatrix4x4 setOrthogonal(float left, float right, float top, float bottom, float fnear, float ffar);
    QMatrix4x4 lookAt2(const QVector3D &eye, const QVector3D &target, const QVector3D &up);
    void resize(float ratio);
    void setSize(float s);

private:
    QVector3D intensity;
    QVector3D position;
    QVector3D direction;

    float size, sizeCascade;

    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 biasMatrix;
    QMatrix4x4 textureProjMatrix;
    QMatrix4x4 projMatrixCascade;
    QMatrix4x4 textureProjMatrixCascade;


};

#endif // LIGHT_H
