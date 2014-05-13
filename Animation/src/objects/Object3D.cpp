#include "Object3D.h"

Object3D::Object3D()
{
    matrix.identity();
    rotation = QVector3D(0,0,0);
    scale = QVector3D(1,1,1);
    position = QVector3D(0,0,0);
    needUpdate = false;
    visible = true;
}

void Object3D::setRotation(const QVector3D &rotation)
{
    this->rotation = QVector3D(rotation.x(),rotation.y(),rotation.z());
    this->needUpdate=true;
}

void Object3D::setPosition(const QVector3D &position)
{
    this->position = QVector3D(position.x(),position.y(),position.z());
    this->needUpdate=true;
}

void Object3D::setScale(const QVector3D &scale)
{
    this->scale = QVector3D(scale.x(),scale.y(),scale.z());
    this->needUpdate=true;
}

void Object3D::updateMatrix()
{
    this->matrix.identity();

    QQuaternion q;
    float c = M_PI / 360,
        x = this->rotation.x() * c,
        y = this->rotation.y() * c,
        z = this->rotation.z() * c,

        c1 = std::cos( y  ),
        s1 = std::sin( y  ),
        c2 = std::cos( -z ),
        s2 = std::sin( -z ),
        c3 = std::cos( x  ),
        s3 = std::sin( x  ),

        c1c2 = c1 * c2,
        s1s2 = s1 * s2;

    q.setScalar(c1c2 * c3  - s1s2 * s3);
    q.setX(c1c2 * s3  + s1s2 * c3);
    q.setY(s1 * c2 * c3 + c1 * s2 * s3);
    q.setZ(c1 * s2 * c3 - s1 * c2 * s3);

    this->matrix.setRotationMatrix(q);

    this->matrix.scale(this->scale);

    this->matrix.translate(this->position);

    this->needUpdate=false;
}


void Object3D::rotate(const QVector3D &xyz)
{
    this->rotation += xyz;
    this->needUpdate=true;
}

void Object3D::translate(const QVector3D &xyz)
{
    this->position += xyz;
    this->needUpdate=true;
}

void Object3D::rotateX(float s)
{
    this->rotation.setX(this->rotation.x() + s);
    this->needUpdate=true;
}

void Object3D::rotateY(float s)
{
    this->rotation.setY(this->rotation.y() + s);
    this->needUpdate=true;
}

void Object3D::rotateZ(float s)
{
    this->rotation.setZ(this->rotation.z() + s);
    this->needUpdate=true;
}

void Object3D::translateX(float s)
{
    this->position.setX(this->position.x() + s);
     this->needUpdate=true;
}

void Object3D::translateY(float s)
{
    this->position.setY(this->position.y() + s);
    this->needUpdate=true;
}

void Object3D::translateZ(float s)
{
    this->position.setZ(this->position.z() + s);
    this->needUpdate=true;
}

void Object3D::translateRel(float s, QVector3D axis)
{
    matrix.rotateAxis(axis);
    position += axis * s;
}

void Object3D::translateRelX(float s)
{
    translateRel(s, QVector3D(1,0,0));
}

void Object3D::translateRelY(float s)
{
    translateRel(s, QVector3D(0,1,0));
}

void Object3D::translateRelZ(float s)
{
    translateRel(s, QVector3D(0,0,1));
}

Matrix4& Object3D::getMatrix()
{
    if(needUpdate) updateMatrix();
    return this->matrix;
}

QVector3D& Object3D::getPosition()
{
    return position;
}
