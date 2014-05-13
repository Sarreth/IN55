#include "Light.h"

Light::Light(QVector3D position, QVector3D direction,QVector3D intensity)
{
    this->position = position;
    this->direction = direction;
    this->intensity = intensity;

    size = 20;
    sizeCascade = 120;

    projMatrix.setOrthogonal(-size,size,size,-size,1,200);
    projMatrixCascade.setOrthogonal(-sizeCascade,sizeCascade,sizeCascade,-sizeCascade,1,200);

    biasMatrix.set(0.5,0.0,0.0,0.5,
                    0.0,0.5,0.0,0.5,
                    0.0,0.0,0.5,0.5,
                    0.0,0.0,0.0,0.1);
}

void Light::setSize(float s)
{
    size = s;
}

void Light::resize(float ratio)
{
    projMatrix.setOrthogonal(-size,size,size/ratio,-size/ratio,60.0,200.0);
    projMatrixCascade.setOrthogonal(-sizeCascade,sizeCascade,sizeCascade/ratio,-sizeCascade/ratio,40.0,300.0);
}

void Light::setDirection(const QVector3D &direction)
{
    this->direction = direction;
}

QVector3D& Light::getDirection()
{
    return direction;
}

void Light::lookAt(const QVector3D &target)
{
   direction = target - position;
   direction.normalize();

   viewMatrix.lookAt2(position, target, QVector3D(0,1,0));
}

QVector3D& Light::getIntensity()
{
    return intensity;
}

void Light::setIntensity(const QVector3D &intensity)
{
    this->intensity = intensity;
}

QVector3D& Light::getPosition()
{
    return position;
}

void Light::setPosition(const QVector3D &position)
{
    this->position = position;
}

Matrix4& Light::getProjMatrix()
{
    return projMatrix;
}

Matrix4& Light::getProjMatrixCascade()
{
    return projMatrixCascade;
}

Matrix4& Light::getViewMatrix()
{
    return viewMatrix;
}

Matrix4& Light::getTextureProjMatrix(const Matrix4& modelMatrix)
{
    textureProjMatrix = biasMatrix * projMatrix * viewMatrix * modelMatrix;
    return textureProjMatrix;
}

Matrix4& Light::getTextureProjMatrixCascade(const Matrix4& modelMatrix)
{
    textureProjMatrixCascade = biasMatrix * projMatrixCascade * viewMatrix * modelMatrix;
    return textureProjMatrixCascade;
}
