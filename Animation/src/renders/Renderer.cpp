#include "Renderer.h"
#include "ShaderLibrary.h"

#define MAX_SHADOW_WIDTH_1 2048
#define MAX_SHADOW_WIDTH_0 2048

Renderer::Renderer()
{
    active = false;
    delta = 0;

    width = 1024;
    height = 768;
    finalWidth = width;
    finalHeight = height;
    ratio = 1.0;

    mainFBO = 0;
    postComposer = 0;

    __camera = 0;
    __material = 0;
    __shader = 0;
    __locPosition = 0;
    __locColor = 0;
    __locNormal = 0;
    __locTexcoord = 0;
    __locMVP = 0;

}

void Renderer::init(Scene* _scene, unsigned int w, unsigned int h)
{

    width = w;
    height = h;

    mainFBO = new FrameBuffer(QStringList()<<"texScene"<<"texThreshold", w, h, true);
    mainFBO->init(0, true);

    postComposer = new PostComposer(mainFBO);

    scene = _scene;
    __camera = scene->getCurrentCamera();
    __skybox = scene->getSky();
    __sun = scene->getSun();
    __ground = scene->getGround();

    matrixProj = __camera->getProjection();
    matrixView = __camera->getMatrix();
    matrixModel.identity();
    xform.push(matrixModel);
}

void Renderer::start()
{
    qDebug() << "+ Renderer start" <<endl;
    active = true;
    timer.start();
}

void Renderer::stop()
{
    active = false;
}

void Renderer::render()
{
    delta = timer.restart()/32;

    scene->update(delta);
    __lightDir = __sun->getLight()->getDirection();

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth(1.0f);

    mainFBO->bind();
    mainFBO->resizeViewport();
    mainFBO->clear();

    matrixProj = __camera->getProjection();
    matrixView = __camera->getMatrix();

    drawSky();
    drawSun();

    pushMatrix(scene->getMatrix());

    drawGround();
    drawMeshes();

    popMatrix();

    mainFBO->unbind();

    postComposer->render();

    Texture::needsUpdate = false;

}

void Renderer::drawSky()
{
    pushMatrix(__skybox->getMatrix());

    __material = __skybox->getMaterial();
    __shader = __material->bind();

    calcMVP();
    transmitMVP();

    __skybox->drawWithVBO();

    popMatrix();
}

void Renderer::drawSun()
{
    pushMatrix(__sun->getMatrix());

    __material = __sun->getMaterial();
    __shader = __material->bind();

    calcMVP();
    transmitMVP();

    postComposer->setLightPosScreen(
                QVector3D(__mvp.data[0][3], __mvp.data[1][3], __mvp.data[2][3])
                /__mvp.data[3][3]
                * 0.5
                + QVector3D(0.5, 0.5, 0.5));

    __sun->drawWithVBO();

    popMatrix();
}

void Renderer::drawGround()
{
    pushMatrix(__ground->getMatrix());

    __material = __ground->getMaterial();
    __shader = __material->bind();

    __shader->transmitUniform("cascadeShadow", 0);

    calcMVP();
    transmitNormalMatrix();
    transmitViewMatrix();
    transmitMVP();
    __shader->transmitUniform("lightDir", __lightDir);
    __ground->drawWithVBO();

    popMatrix();
}

bool Renderer::cull(Instance* m, float offset)
{
    return (
                (m->getPosition().distanceToPoint(__camera->getPosition())*m->getPosition().distanceToPoint(__camera->getPosition())) > 500 + offset
                && QVector3D::dotProduct((m->getPosition() - (__camera->getPosition()-scene->getCameraZ()*20.0)),scene->getCameraZ()) < 0
    );
}

void Renderer::drawMeshes()
{
    foreach(Mesh* m, scene->getMeshes())
    {
        __material = m->getMaterial();
        __shader = __material->bind();

        __shader->transmitUniform("cascadeShadow", 0);

        __shader->transmitUniform("lightDir", __lightDir);
        pushMatrix(m->getMatrix());
        calcMVP();
        transmitNormalMatrix();
        transmitViewMatrix();
        transmitMVP();

        m->drawWithVBO();

        popMatrix();

        foreach(Instance* i, m->getInstances())
        {
            if(cull(i, 250)) continue;

            pushMatrix(i->getMatrix());
            calcMVP();
            transmitNormalMatrix();
            transmitViewMatrix();
            transmitMVP();
            m->drawWithVBO();

            popMatrix();
        }
    }
}

void Renderer::resize(unsigned int w, unsigned int h)
{
    finalWidth = w;
    finalHeight = h;
    float a = w / static_cast<float>(h);

    if(ratio != 1.0)
    {
        w = (int)std::floor(w*ratio +.5);
        h = (int)std::floor(w/a +.5);
    }

    if(postComposer != 0)
    {
        postComposer->resize(w, h);
        postComposer->setFinalRenderSize(finalWidth, finalHeight);
    }

    if(mainFBO != 0)
        mainFBO->resize(w, h);

    if(__sun != 0)
        __sun->getLight()->resize(1);


    if(__camera != 0)
    {
        __camera->setPerspective(65.0f, a, 0.1f, 500.0f);
        matrixProj = __camera->getProjection();
    }
}


PostComposer* Renderer::getPostComposer()
{
    return postComposer;
}

void Renderer::pushMatrix(const Matrix4 &mat)
{
    xform.push(matrixModel);
    matrixModel *= mat;
}

void Renderer::popMatrix()
{
    matrixModel = xform.pop();
}

void Renderer::calcMVP()
{
    __mv = matrixView * matrixModel;
    __mvp = matrixProj * __mv;
}

void Renderer::transmitMVP()
{
    __shader->transmitUniform("mvp", __mvp);
}

void Renderer::transmitNormalMatrix()
{

    float* array = __mv.array;
    float a11 =   array[10] * array[5] - array[6] * array[9];
    float a21 = - array[10] * array[1] + array[2] * array[9];
    float a31 =   array[6] * array[1] - array[2] * array[5];
    float a12 = - array[10] * array[4] + array[6] * array[8];
    float a22 =   array[10] * array[0] - array[2] * array[8];
    float a32 = - array[6] * array[0] + array[2] * array[4];
    float a13 =   array[9] * array[4] - array[5] * array[8];
    float a23 = - array[9] * array[0] + array[1] * array[8];
    float a33 =   array[5] * array[0] - array[1] * array[4];

    float det = array[0] * a11 + array[1] * a12 + array[2] * a13;

    float idet = 1.0 / det;

    QMatrix3x3 tmp;
    tmp(0,0)=idet * a11;
    tmp(0,1)=idet * a21;
    tmp(0,2)=idet * a31;
    tmp(1,0)=idet * a12;
    tmp(1,1)=idet * a22;
    tmp(1,2)=idet * a32;
    tmp(2,0)=idet * a13;
    tmp(2,1)=idet * a23;
    tmp(2,2)=idet * a33;

    __shader->transmitUniform("normalMatrix", tmp.transposed());
}

void Renderer::transmitViewMatrix()
{
    float* array = matrixView.array;
    float a11 =   array[10] * array[5] - array[6] * array[9];
    float a21 = - array[10] * array[1] + array[2] * array[9];
    float a31 =   array[6] * array[1] - array[2] * array[5];
    float a12 = - array[10] * array[4] + array[6] * array[8];
    float a22 =   array[10] * array[0] - array[2] * array[8];
    float a32 = - array[6] * array[0] + array[2] * array[4];
    float a13 =   array[9] * array[4] - array[5] * array[8];
    float a23 = - array[9] * array[0] + array[1] * array[8];
    float a33 =   array[5] * array[0] - array[1] * array[4];

    float det = array[0] * a11 + array[1] * a12 + array[2] * a13;

    float idet = 1.0 / det;

    QMatrix3x3 tmp;
    tmp(0,0)=idet * a11;
    tmp(0,1)=idet * a21;
    tmp(0,2)=idet * a31;
    tmp(1,0)=idet * a12;
    tmp(1,1)=idet * a22;
    tmp(1,2)=idet * a32;
    tmp(2,0)=idet * a13;
    tmp(2,1)=idet * a23;
    tmp(2,2)=idet * a33;

    __shader->transmitUniform("viewMatrix", tmp.transposed());
}

void Renderer::setRatio(float r)
{
    ratio = r > 0.0 && r < 1.0 ? r : 1.0;
    resize(finalWidth, finalHeight);
}

void Renderer::loadShaders()
{
    ShaderLibrary::addShader("rt_basic",
                             "shaders/rt_basic",
                             QStringList()<<"position"<<"normal"<<"texcoord",
                             QStringList()<<"mvp"<<"normalMatrix"<<"viewMatrix"<<"lightDir"<<"texDiffuse"<<"texAlpha"<<"shadowQuality"<<"texShadow"<<"shadowMatrix"<<"shadowResolution"<<"texShadowCascade"<<"shadowMatrixCascade"<<"cascadeShadow");

    ShaderLibrary::addShader("rt_sun",
                             "shaders/rt_sun",
                             QStringList()<<"position"<<"texcoord",
                             QStringList()<<"mvp"<<"texDiffuse"<<"texAlpha");

    ShaderLibrary::addShader("rt_ground",
                             "shaders/rt_ground",
                             QStringList()<<"position"<<"normal",
                             QStringList()<<"mvp"<<"normalMatrix"<<"viewMatrix"<<"lightDir"<<"heightRange"<<"tileSize"<<"texMoss"<<"texEarth"<<"texShatter"<<"texNormalMoss"<<"texNormalEarth"<<"texNormalShatter"<<"shadowQuality"<<"texShadow"<<"shadowMatrix"<<"shadowResolution"<<"texShadowCascade"<<"shadowMatrixCascade"<<"cascadeShadow");

    ShaderLibrary::addShader("rt_shadow",
                             "shaders/rt_shadow",
                             QStringList()<<"position"<<"texcoord",
                             QStringList()<<"mvp"<<"texAlpha"<<"useAlpha");

    ShaderLibrary::addShader("rt_skybox",
                             "shaders/rt_skybox",
                             QStringList()<<"position",
                             QStringList()<<"mvp"<<"texCube");

    ShaderLibrary::addShader("ss_compose",
                             "shaders/ss_compose",
                             QStringList()<<"position",
                             QStringList()<<"texScene");

    ShaderLibrary::addShader("ss_display",
                             "shaders/ss_display",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"gammaRGB"<<"aspect");

    ShaderLibrary::addShader("ss_fxaa",
                             "shaders/ss_fxaa",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"resolution");

    ShaderLibrary::addShader("ss_lenseflare_p1",
                             "shaders/ss_lenseflare_p1",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"flareThreshold");

    ShaderLibrary::addShader("ss_lenseflare_p2",
                             "shaders/ss_lenseflare_p2",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"flareSamples"<<"flareDispersal"<<"flareHaloWidth"<<"flareChromaDispertion");

    ShaderLibrary::addShader("ss_lenseflare_p3",
                             "shaders/ss_lenseflare_p3",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"texFlare"<<"texDirt"<<"flareIntensity");

    ShaderLibrary::addShader("ss_blur_linearh",
                             "shaders/ss_blur_linearh",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"resolution");

    ShaderLibrary::addShader("ss_blur_linearv",
                             "shaders/ss_blur_linearv",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"resolution");

    ShaderLibrary::addShader("ss_distortion",
                             "shaders/ss_distortion",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"resolution");

    ShaderLibrary::addShader("ss_godrays_p1",
                             "shaders/ss_godrays_p1",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"lightPos");

    ShaderLibrary::addShader("ss_godrays_p2",
                             "shaders/ss_godrays_p2",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"texGodrays");

    ShaderLibrary::addShader("ss_bloom_p1",
                             "shaders/ss_bloom_p1",
                             QStringList()<<"position",
                             QStringList()<<"texScene");

    ShaderLibrary::addShader("ss_bloom_p2",
                             "shaders/ss_bloom_p2",
                             QStringList()<<"position",
                             QStringList()<<"texScene"<<"texBloom");
}
