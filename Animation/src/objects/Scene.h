#ifndef SCENE_H
#define SCENE_H

#include "Sun.h"
#include "SkyBox.h"
#include "Camera.h"
#include "Ground.h"

class Scene : public Object3D
{
public:
    Scene();

    QList<Mesh*> getMeshes();
    Sun* getSun();
    QList<Camera*> getCameras();
    SkyBox* getSky();
    Ground* getGround();

    Mesh* getMeshAt(int index);
    Camera* getCameraAt(int index);

    Camera* getCurrentCamera();
    void setCurrentCamera(Camera *camera);

    void addMesh(Mesh* mesh);
    void setSun(Sun* sun);
    void addCamera(Camera* camera);
    void setSky(SkyBox* sky);
    void setGround(Ground* ground);

    void removeMesh(Mesh* mesh);
    void removeCamera(Camera* camera);

    void removeMeshAt(int index);
    void removeCameraAt(int index);

    void update(float delta);
    void init();

    bool getRenderAble();
    void setRenderAble(bool renderAble);

    const QVector3D& getCameraZ();


private :
    QList<Mesh*> meshes;
    Sun* sun;
    QList<Camera*> cameras;
    SkyBox* sky;
    Ground* ground;

    Camera* currentCamera;
    QVector3D cameraZ;
    bool renderAble;
};

#endif // SCENE_H
