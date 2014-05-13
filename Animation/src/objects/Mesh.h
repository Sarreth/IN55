#ifndef MESH_H
#define MESH_H

#include "../renders/Shader.h"
#include "../materials/Material.h"
#include "Instance.h"

class Mesh : public Object3D, protected QOpenGLFunctions_3_2_Core
{
public:
    Mesh();
    Mesh(Geometry* geometry, Material* material);
    ~Mesh();

    void initVBO();
    void drawWithVBO();
    void debugDrawWithVBO();

    Geometry* getGeometry();
    Material* getMaterial();

    Instance* newInstance();
    void removeInstance(int id);
    void clearInstances();
    Instance* getInstance(int id);
    QList<Instance*>& getInstances();
    float getScaleRdn();
    void setScaleRdn(float sRdn);
    float getHeightRdn();
    void setHeightRdn(float height);
    float getPourcentage();
    void setPourcentage(float pct);
    float getRangeScale();
    void setRangeScale(float range);

    void setInstanceType(unsigned int i);
    unsigned int getInstanceType();

    bool isInstance();

    static const unsigned int INSTANCE_NONE;
    static const unsigned int INSTANCE_PALM;
    static const unsigned int INSTANCE_HTREE;
    static const unsigned int INSTANCE_BTREE;
    static const unsigned int INSTANCE_GPALM;
    static const unsigned int INSTANCE_BUSH;
    static const unsigned int INSTANCE_ROCK;

protected :
    Geometry* geometry;
    Material* material;

    unsigned int vertices;
    unsigned int indices;
    unsigned int normals;
    unsigned int texcoords;

    bool hasNormals;
    bool hasTexCoords;

    unsigned int locVertices;
    unsigned int locNormals;
    unsigned int locTexCoords;

    float scaleRdn;
    float rangeScale;
    float heightRdn;
    float pourcentage;
    unsigned int instanceType;

    QList<Instance*> instances;
};


#endif // MESH_H
