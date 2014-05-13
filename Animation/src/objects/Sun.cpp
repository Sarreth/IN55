#include "Sun.h"

#define SUN_ANGLE_STEP 0.001
#define SUN_YAW_OFFSET M_PI/10.0
#define SUN_YAW_START -SUN_YAW_OFFSET
#define SUN_YAW_END M_PI+SUN_YAW_OFFSET

Sun::Sun(MaterialSun *mat) : Mesh()
{
    float size = 2.0;

    float vertices[] = {
      -size,  size,  0.0,
      -size, -size,  0.0,
       size, -size,  0.0,
       size,  size,  0.0
    };

    float texcoords[] = {
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0
    };

    int indices[] = {0, 1, 2, 3};

    Geometry* g = new Geometry(vertices, indices, 12, 4);
    g->setTexCoords(texcoords);
    g->setPrimitive(GL_QUADS);

    geometry=g;
    material = mat;
    light = new Light();
    lightDistance = 150.0;
    lightTargetOffset = 13;
    sunDistance = 6.0;
    yaw = M_PI/3.0;
    speed = 0.0;
}

void Sun::update(const QVector3D &positionCamera, const QVector3D &zCamera)
{
    yaw += SUN_ANGLE_STEP*speed;
    if(yaw > SUN_YAW_END) yaw = SUN_YAW_START;

    localPosition = QVector3D(std::cos(yaw), std::sin(yaw), -std::sin(yaw)/3.0);
    position = QVector3D(positionCamera + localPosition*sunDistance);

    matrix.lookAt(position, positionCamera, QVector3D(0,1,0));

    QVector3D offsetCamera = positionCamera + zCamera*lightTargetOffset;
    light->setPosition(offsetCamera + localPosition*lightDistance);
    light->lookAt(offsetCamera);
}

void Sun::setSpeed(float s)
{
    speed = s;
}

Light* Sun::getLight()
{
    return light;
}
