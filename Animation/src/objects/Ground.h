#ifndef GROUND_H
#define GROUND_H

#include "Mesh.h"
#include "../materials/MaterialGround.h"

class Ground : public Mesh
{

private:
    int numberVerticesX;
    int numberVerticesZ;
    QVector3D * normals;
//    Path * path;
    QList<QVector3D*> rockPos;

public:
    Ground(QString path, MaterialGround* mat);
    ~Ground();

    void setVectorToFloat(QVector3D normals, float * normalsF, int i);
    void firstLineNormals(float * normalsF);
    void middleGridNormals(float * normalsF);
    void firstVertexNormal(float *normalsF, int lineNumber);
    void lastLineNormals(float * normalsF);
    float getY(float x, float z);
    float getYApprox(float x, float z, float prevY);
    QVector3D randomMapPos(double height = 0, bool isRock = false);
    bool containInBoundingBox(double x, double z);
    QList<QVector3D*> getRockPos();
    void removeRockPos();

    static const float MAP_SIZE;
    static float MAP_MIN;
    static float MAP_MAX;
    static const float MAX_HEIGHT;
};

#endif // GROUND_H
