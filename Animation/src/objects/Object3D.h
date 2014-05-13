#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "../../Objets.h"
#include "../utils/Geometry.h"
#include "../utils/Matrix4.h"

class Object3D
{
public:

    Object3D();

    void setRotation(const QVector3D&);
    void setPosition(const QVector3D&);
    void setScale(const QVector3D&);
    void updateMatrix();

    void rotate(const QVector3D&);
    void rotateX(float);
    void rotateY(float);
    void rotateZ(float);
    void translate(const QVector3D&);
    void translateX(float);
    void translateY(float);
    void translateZ(float);
    void translateRel(float, QVector3D);
    void translateRelX(float);
    void translateRelY(float);
    void translateRelZ(float);

    Matrix4& getMatrix();
    QVector3D& getPosition();


protected :
    Matrix4 matrix;
    QVector3D rotation;
    QVector3D position;
    QVector3D scale;

    bool needUpdate;
    bool visible;
};

#endif // OBJECT3D_H
