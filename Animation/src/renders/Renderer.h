#ifndef RENDERER_H
#define RENDERER_H

#include "../objects/Scene.h"
#include "PostComposer.h"
#include <QElapsedTimer>
#include <QStack>
#include <QMatrix3x3>

class Renderer
{
public:
    Renderer();

    void setScene(Scene* s);

    void init(Scene* _scene, unsigned int w, unsigned int h);
    void resize(unsigned int w, unsigned int h);
    void loadShaders();

    void start();
    void stop();
    void render();

    void drawSky();
    void drawSun();
    void drawGround();
    void drawMeshes();

    PostComposer* getPostComposer();

    void setRatio(float r);

private:

    void pushMatrix(const Matrix4& mat);
    void popMatrix();

    bool cull(Instance *m, float offset = 0.0);

    void calcMVP();
    void transmitMVP();
    void transmitNormalMatrix();
    void transmitViewMatrix();

    QElapsedTimer timer;
    float delta;
    bool active;

    Scene* scene;
    unsigned int width, height;
    unsigned int finalWidth, finalHeight;
    float ratio;

    FrameBuffer* mainFBO;
    PostComposer* postComposer;

    QStack<Matrix4> xform;
    Matrix4 matrixProj,
            matrixView,
            matrixModel;

    // Buffers
    Camera* __camera;
    Material* __material;
    Shader* __shader;
    SkyBox* __skybox;
    Sun* __sun;
    Ground* __ground;
    Matrix4 __mvp,
            __mv;
    QVector3D __lightDir;
    unsigned int
        __locPosition,
        __locColor,
        __locNormal,
        __locTexcoord,
        __locMVP;
};


#endif // RENDERER_H
