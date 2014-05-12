#include "Object3D.h"

Object3D::Object3D()
{
    matrix.setToIdentity();
    rotation=QVector3D(0,0,0);
    scale=QVector3D(1,1,1);
    position=QVector3D(0,0,0);
    needUpdate = false;
    visible = true;
}

void Object3D::setRotation(const QVector3D &rotation)
{
    this->rotation=QVector3D(rotation.x(),rotation.y(),rotation.z());
    this->needUpdate=true;
}

void Object3D::setPosition(const QVector3D &position)
{
    this->position=QVector3D(position.x(),position.y(),position.z());
    this->needUpdate=true;
}

void Object3D::setScale(const QVector3D &scale)
{
    this->scale=QVector3D(scale.x(),scale.y(),scale.z());
    this->needUpdate=true;
}

void Object3D::updateMatrix()
{
    this->matrix.setToIdentity();

    QQuaternion q;
    float c = M_PI / 360;
    q.setVector(rotation.x()*c,rotation.y()*c,rotation.z()*c);

    float
        c1 = std::cos( q.y()  ),
        s1 = std::sin( q.y()  ),
        c2 = std::cos( -q.z() ),
        s2 = std::sin( -q.z() ),
        c3 = std::cos( q.x()  ),
        s3 = std::sin( q.x()  ),

        c1c2 = c1 * c2,
        s1s2 = s1 * s2;

    q.setScalar(c1c2 * c3  - s1s2 * s3);
    q.setX(c1c2 * s3  + s1s2 * c3);
    q.setY(s1 * c2 * c3 + c1 * s2 * s3);
    q.setZ(c1 * s2 * c3 - s1 * c2 * s3);

    float x = q.x(),
          y = q.y(),
          z = q.z(),
          w = q.scalar(),

        x2 = x + x,     y2 = y + y,     z2 = z + z,
        xx = x * x2,    xy = x * y2,    xz = x * z2,
        yy = y * y2,    yz = y * z2,    zz = z * z2,
        wx = w * x2,    wy = w * y2,    wz = w * z2;

    this->matrix.setRow(0,QVector4D(1 - ( yy + zz ),xy - wz,xz + wy,0));
    this->matrix.setRow(1,QVector4D(xy + wz,1 - ( xx + zz ),yz - wx,0));
    this->matrix.setRow(2,QVector4D(xz - wy,yz + wx,1 - ( xx + yy ),0));

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
    this->rotation.setX(this->position.x()+s);
    this->needUpdate=true;
}

void Object3D::rotateY(float s)
{
     this->rotation.setY(this->position.y()+s);
    this->needUpdate=true;
}

void Object3D::rotateZ(float s)
{
    this->rotation.setZ(this->position.z()+s);
    this->needUpdate=true;
}

void Object3D::translateX(float s)
{
     this->position.setX(this->position.x()+s);
     this->needUpdate=true;
}

void Object3D::translateY(float s)
{
    this->position.setY(this->position.y()+s);
    this->needUpdate=true;
}

void Object3D::translateZ(float s)
{
    this->position.setZ(this->position.z()+s);
    this->needUpdate=true;
}

void Object3D::translateRel(float s, QVector3D axis)
{
//    matrix.rotateAxis(axis);
    matrix.rotate(0,axis);
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

QMatrix4x4& Object3D::getMatrix()
{
    if(needUpdate) updateMatrix();
    return this->matrix;
}

QVector3D& Object3D::getPosition()
{
    return position;
}
