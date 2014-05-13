#ifndef CAMERA_H
#define CAMERA_H

#include "Object3D.h"


class Camera : public Object3D
{
public:
    Camera();

    void lookAt(const QVector3D &_target);
    void setPerspective(float fov, float ratio, float near, float far);

    Matrix4 &getProjection();

    void update(double groundY, double delta = 1.0);
    void reset();

    void setTarget(const QVector3D& _target);
    QVector3D getTarget();

    void setMouse(int x, int y);
    void setLookAccel(int x, int y);

    void setLeftKey(bool);
    void setRightKey(bool);
    void setUpKey(bool);
    void setDownKey(bool);
    void setControlKey(bool);
    void setSpaceKey(bool);
    void setPositionY(float y);

    void stopMouseEffect();

    void activeLook(bool);

    float getYaw();

private :
    Matrix4 projection;

    QVector3D target;

    bool look;

    bool leftKey;
    bool rightKey;
    bool upKey;
    bool downKey;
    bool spaceKey;
    bool controlKey;

    float mouseXold;
    float mouseYold;
    float mouseX;
    float mouseY;
    float accelX;
    float accelY;

    float lookSpeed;
    float moveSpeed;
    float yaw, pitch;
    float height;
};


#endif // CAMERA_H
