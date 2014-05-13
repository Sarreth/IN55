#ifndef SUN_H
#define SUN_H

#include "Mesh.h"
#include "../materials/MaterialSun.h"

class Sun : public Mesh
{
public:
    Sun(MaterialSun* mat);

    void update(const QVector3D &positionCamera, const QVector3D &zCamera);

    void setSpeed(float s);
    Light* getLight();

private:
    Light* light;
    float lightDistance;
    float lightTargetOffset;
    float sunDistance;
    QVector3D localPosition;
    float yaw;
    float speed;
};

#endif // SUN_H
