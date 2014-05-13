#ifndef MATRIX4_H
#define MATRIX4_H

#include <QQuaternion>

class Matrix4
{
public:
    union
    {
        float data[4][4];
        float array[16];
    };

    Matrix4();
    Matrix4(const Matrix4 &mat);

    Matrix4& operator=(const Matrix4 &mat);

    Matrix4& set(float,float,float,float,
                 float,float,float,float,
                 float,float,float,float,
                 float,float,float,float);

    Matrix4& identity();
    Matrix4& zero();
    Matrix4& copy(const Matrix4 &mat);
    Matrix4& transpose();
    Matrix4& lookAt(const QVector3D &eye, const QVector3D &target, const QVector3D &up);
    Matrix4& lookAt2(const QVector3D &eye, const QVector3D &target, const QVector3D &up);
    Matrix4& setPerspective(float fov, float ratio, float near, float far);
    Matrix4& setOrthogonal(float left, float right, float top, float bottom, float near, float far);
    Matrix4& setRotationMatrix(float angle, const QVector3D &xyz);
    Matrix4& setRotationMatrix(const QQuaternion &q);

    QVector3D& rotateAxis(QVector3D &axis);

    Matrix4& setPosition(const QVector3D &xyz);

    Matrix4& translate(const QVector3D &xyz);
    Matrix4& rotate(float angle, const QVector3D &xyz);
    Matrix4& scale(const QVector3D &xyz);

    void log();

    float determinant() const;
    Matrix4 inverse() const;

    Matrix4 operator*(const Matrix4 &mat) const;
    void operator*=(const Matrix4 &mat);

    Matrix4 operator*(const float scalar) const;
    void operator*=(const float scalar);
    QVector3D operator *(const QVector3D vec3) const;
};

#endif // MATRIX4_H
