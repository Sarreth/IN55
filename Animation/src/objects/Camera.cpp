#include "camera.h"
#include <QDebug>
#include <cmath>

#define DEG_TO_RAD M_PI/180.0


Camera::Camera():Object3D()
{
    leftKey = false;
    rightKey = false;;
    upKey = false;
    downKey = false;
    spaceKey = false;
    controlKey = false;

    setPerspective(90.0f, 1.33, 0.1f, 500.0f);
    target = QVector3D(0,0,0);

    lookSpeed = 0.5;
    moveSpeed = 5;

    yaw = 0.0;
    pitch = 0.0;

    mouseXold = 0.0;
    mouseYold = 0.0;
    mouseX = 0.0;
    mouseY = 0.0;
    accelX = 0.0;
    accelY = 0.0;

    look = false;

    height = 2.2;
}

void Camera::reset()
{
    yaw = 0.0;
    pitch = 0.0;
    position = QVector3D(0,6,-1);
    qDebug()<<"Reset Camera";
}

void Camera::setPerspective(float ffov, float fratio, float fnear, float ffar)
{
    projection.setPerspective(ffov,fratio,fnear,ffar);
}

Matrix4& Camera::getProjection()
{
    return projection;
}

void Camera::setLeftKey(bool a)
{
    leftKey = a;
}

void Camera::setRightKey(bool a)
{
    rightKey = a;
}

void Camera::setUpKey(bool a)
{
    upKey = a;
}

void Camera::setDownKey(bool a)
{
    downKey = a;
}

void Camera::setSpaceKey(bool a)
{
    spaceKey = a;
}

void Camera::setControlKey(bool a)
{
    controlKey = a;
}

void Camera::update(double groundY, double delta)
{
    // http://www.lloydgoodall.com/tutorials/first-person-camera-control-with-lwjgl/

//    position.y = groundY + height;

    if(look)
    {
        yaw += accelX*lookSpeed;
        pitch += accelY*lookSpeed;
    }

    if(leftKey == true)
    {
        position.setX(position.x() - moveSpeed * std::sin((yaw+90)*DEG_TO_RAD) * delta);
        position.setZ(position.z() + moveSpeed * std::cos((yaw+90)*DEG_TO_RAD) * delta);
    }
    if(rightKey == true)
    {
        position.setX(position.x() - moveSpeed * std::sin((yaw-90)*DEG_TO_RAD) * delta);
        position.setZ(position.z() + moveSpeed * std::cos((yaw-90)*DEG_TO_RAD) * delta);
    }
    if(upKey == true)
    {
        position.setX(position.x() + moveSpeed * std::sin(yaw*DEG_TO_RAD) * delta);
        position.setZ(position.z() - moveSpeed * std::cos(yaw*DEG_TO_RAD) * delta);
    }
    if(downKey == true)
    {
        position.setX(position.x() - moveSpeed * std::sin(yaw*DEG_TO_RAD) * delta);
        position.setZ(position.z() + moveSpeed * std::cos(yaw*DEG_TO_RAD) * delta);
    }
    if(spaceKey == true)
    {
        position.setY(position.y() + moveSpeed * std::sin(yaw*DEG_TO_RAD) * delta);
//        position.z += moveSpeed * std::cos(yaw*DEG_TO_RAD) * delta;
    }
    if(controlKey == true)
    {
        position.setY(position.y() - moveSpeed * std::sin(yaw*DEG_TO_RAD) * delta);
//        position.z += moveSpeed * std::cos(yaw*DEG_TO_RAD) * delta;
    }

    matrix.identity()
            .translate(position*-1)
            .rotate(yaw, QVector3D(0,1,0))
            .rotate(pitch, QVector3D(1,0,0));
}

void Camera::setTarget(const QVector3D &_target)
{
    this->target = _target ;
}

QVector3D Camera::getTarget()
{
    return target;
}

void Camera::setMouse(int x, int y)
{
    mouseXold = mouseX;
    mouseYold = mouseY;
    mouseX = (float)x;
    mouseY = (float)y;
    accelX = mouseX - mouseXold;
    accelY = mouseY - mouseYold;
}

void Camera::setLookAccel(int x, int y)
{
    accelX = (float)x;
    accelY = (float)y;
}

void Camera::activeLook(bool active)
{
    look = active;
}

float Camera::getYaw()
{
    return yaw*DEG_TO_RAD;
}

void Camera::setPositionY(float y){
    this->position.setY(y);
}
