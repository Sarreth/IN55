#include "Light.h"

QMatrix4x4 Light::setOrthogonal(float left, float right, float top, float bottom, float fnear, float ffar)
{
    float x, y, z, w, h, p;

    w = right - left;
    h = top - bottom;
    p = ffar - fnear;

    x = ( right + left ) / w;
    y = ( top + bottom ) / h;
    z = ( ffar + fnear ) / p;

    QMatrix4x4 mat;
    mat.setColumn(0,QVector4D(2/w,0,0,0));
    mat.setColumn(1,QVector4D(0,2/h,0,0));
    mat.setColumn(2,QVector4D(0,0,-2/p,0));
    mat.setColumn(3,QVector4D(-x,-y,-z,1));

    return mat;
}

QMatrix4x4 Light::lookAt2(const QVector3D &eye, const QVector3D &target, const QVector3D &up)
{
    QVector3D x, y, z;
    QMatrix4x4 mat;

    z = (target - eye);
    z.normalize();
    if(z.isNull()) z.setZ(0.1);

    x.crossProduct(z, up).normalize();
    y.crossProduct(x, z).normalize();

    mat.setToIdentity();

    mat.setColumn(0,QVector4D(x.x(),y.x(),-z.x(),0));
    mat.setColumn(1,QVector4D(x.y(),y.y(),-z.y(),0));
    mat.setColumn(2,QVector4D(x.z(),y.z(),-z.z(),0));
    mat.setColumn(3,QVector4D(QVector3D::dotProduct(-x,eye),QVector3D::dotProduct(-y,eye),QVector3D::dotProduct(z,eye),1));

    return mat;
}

Light::Light(QVector3D position, QVector3D direction,QVector3D intensity)
{
    this->position = position;
    this->direction = direction;
    this->intensity = intensity;

    size = 20;
    sizeCascade = 120;

    projMatrix=setOrthogonal(-size,size,size,-size,1,200);
    projMatrixCascade=setOrthogonal(-sizeCascade,sizeCascade,sizeCascade,-sizeCascade,1,200);

    biasMatrix.setRow(0,QVector4D(0.5,0.0,0.0,0.5));
    biasMatrix.setRow(1,QVector4D(0.0,0.5,0.0,0.5));
    biasMatrix.setRow(2,QVector4D(0.0,0.0,0.5,0.5));
    biasMatrix.setRow(3,QVector4D(0.0,0.0,0.0,0.1));
}

void Light::setSize(float s)
{
    size = s;
}

void Light::resize(float ratio)
{
    projMatrix=setOrthogonal(-size,size,size/ratio,-size/ratio,60.0,200.0);
    projMatrixCascade=setOrthogonal(-sizeCascade,sizeCascade,sizeCascade/ratio,-sizeCascade/ratio,40.0,300.0);
}

void Light::setDirection(const QVector3D &direction)
{
    this->direction=direction;
}

QVector3D& Light::getDirection()
{
    return direction;
}

void Light::lookAt(const QVector3D &target)
{
   direction = target - position;
   direction.normalize();

   viewMatrix=lookAt2(position, target, QVector3D(0,1,0));
}

QVector3D& Light::getIntensity()
{
    return intensity;
}

void Light::setIntensity(const QVector3D &intensity)
{
    this->intensity=intensity;
}

QVector3D& Light::getPosition()
{
    return position;
}

void Light::setPosition(const QVector3D &position)
{
    this->position=position;
}

QMatrix4x4& Light::getProjMatrix()
{
    return projMatrix;
}

QMatrix4x4& Light::getProjMatrixCascade()
{
    return projMatrixCascade;
}

QMatrix4x4& Light::getViewMatrix()
{
    return viewMatrix;
}

QMatrix4x4& Light::getTextureProjMatrix(const QMatrix4x4& modelMatrix)
{
    textureProjMatrix = biasMatrix * projMatrix * viewMatrix * modelMatrix;
    return textureProjMatrix;
}

QMatrix4x4& Light::getTextureProjMatrixCascade(const QMatrix4x4& modelMatrix)
{
    textureProjMatrixCascade = biasMatrix * projMatrixCascade * viewMatrix * modelMatrix;
    return textureProjMatrixCascade;
}
