#ifndef SUN_H
#define SUN_H

#include "Material.h"

class Sun : public Object3D, protected QOpenGLFunctions_3_2_Core
{
public:
    Sun();
    ~Sun();
    Sun(Material* mat);
    Sun(Geometry* geometry, Material* material);

    void initVBO();
    void drawWithVBO();
    void debugDrawWithVBO();
    void update(const QVector3D &positionCamera, const QVector3D &zCamera);

    void setSpeed(float s);

    Light* getLight();
    Geometry* getGeometry();
    Material* getMaterial();

    void castsShadows(bool active);
    bool castsShadows();

    float getScaleRdn();
    void setScaleRdn(float sRdn);
    float getHeightRdn();
    void setHeightRdn(float height);
    float getPourcentage();
    void setPourcentage(float pct);
    float getRangeScale();
    void setRangeScale(float range);

protected :
    Geometry* geometry;
    Material* material;
    Light* light;
    float lightDistance;
    float lightTargetOffset;
    float sunDistance;

    QVector3D localPosition;
    float yaw;
    float speed;

    unsigned int vertices;
    unsigned int indices;
    unsigned int normals;
    unsigned int texcoords;

    bool hasNormals;
    bool hasTexCoords;

    bool castShadows;

    unsigned int locVertices;
    unsigned int locNormals;
    unsigned int locTexCoords;

    float scaleRdn;
    float rangeScale;
    float heightRdn;
    float pourcentage;

};

#endif // SUN_H
