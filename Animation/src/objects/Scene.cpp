#include "Scene.h"

Scene::Scene():Object3D()
{
    Camera* camera = new Camera();
    camera->setMouse(QCursor::pos().x(), QCursor::pos().y());
    camera->setMouse(QCursor::pos().x(), QCursor::pos().y());
    camera->reset();

    Texture::resetUnit();
    SkyBox* skyBox = new SkyBox(new MaterialSkyBox(new TextureCube("pics/skybox/")));
    skyBox->setPosition(camera->getPosition());

    // SUN
    Texture::resetUnit();
    Texture* sunDiffuse = Texture::newFromNextUnit();
    Texture* sunAlpha = Texture::newFromNextUnit();
    sunDiffuse->load("pics/sun/sun_1k.jpg");
    sunDiffuse->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    sunDiffuse->init();
    sunAlpha->load("pics/sun/sun_1k_alpha.jpg");
    sunAlpha->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    sunAlpha->init();
    MaterialSun* sunMat = new MaterialSun(sunDiffuse, sunAlpha);
    Sun* sun = new Sun(sunMat);


    // GROUND
    Texture::resetUnit();
    // diffuses
    Texture* groundMoss = Texture::newFromNextUnit();
    Texture* groundEarth = Texture::newFromNextUnit();
    Texture* groundShatter = Texture::newFromNextUnit();
    groundMoss->load("pics/ground/moss.jpg");
    groundMoss->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    groundMoss->init();
    groundEarth->load("pics/ground/earth.jpg");
    groundEarth->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    groundEarth->init();
    groundShatter->load("pics/ground/shatter.jpg");
    groundShatter->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    groundShatter->init();
    // normals
    Texture* groundNormalMoss = Texture::newFromNextUnit();
    Texture* groundNormalEarth = Texture::newFromNextUnit();
    Texture* groundNormalShatter = Texture::newFromNextUnit();
    groundNormalMoss->load("pics/ground/moss_normal.png");
    groundNormalMoss->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    groundNormalMoss->init();
    groundNormalEarth->load("pics/ground/earth_normal.png");
    groundNormalEarth->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    groundNormalEarth->init();
    groundNormalShatter->load("pics/ground/shatter_normal.png");
    groundNormalShatter->setFilters(Texture::MIPMAP, Texture::MIPMAP);
    groundNormalShatter->init();
    // init
    MaterialGround* groundMat = new MaterialGround(groundMoss, groundEarth, groundShatter, groundNormalMoss, groundNormalEarth, groundNormalShatter);
    Ground* ground = new Ground("pics/heightmaps/heightmap.png", groundMat);

    this->addCamera(camera);
    this->setCurrentCamera(camera);
    this->setSky(skyBox);
    this->setSun(sun);
    this->setGround(ground);

}

void Scene::init()
{
    foreach(Mesh* m, meshes)
        m->initVBO();

    sky->initVBO();
    sun->initVBO();
    ground->initVBO();
}

void Scene::update(float delta)
{
    qDebug() << "Sky pos : " << sky->getPosition() << "//Cam pos : " << currentCamera->getPosition() << endl;
    float groundY = ground->getYApprox(currentCamera->getPosition().x(), currentCamera->getPosition().z(),currentCamera->getPosition().y());

    currentCamera->setMouse(QCursor::pos().x(), QCursor::pos().y());
    currentCamera->update(groundY, delta);

    float yaw = currentCamera->getYaw();
    cameraZ=QVector3D(std::sin(yaw),0,-std::cos(yaw));
    QVector3D cameraPosition = currentCamera->getPosition();

    sky->setPosition(cameraPosition);
    sun->update(cameraPosition, cameraZ);
}

QList<Mesh*> Scene::getMeshes()
{
    return meshes;
}

Sun* Scene::getSun()
{
    return sun;
}

QList<Camera*> Scene::getCameras()
{
    return cameras;
}

SkyBox* Scene::getSky()
{
    return sky;
}

Ground* Scene::getGround()
{
    return ground;
}

Mesh* Scene::getMeshAt(int index)
{
    return meshes.at(index);
}

Camera* Scene::getCameraAt(int index)
{
    return cameras.at(index);
}

Camera* Scene::getCurrentCamera()
{
    return currentCamera;
}

void Scene::setCurrentCamera(Camera* camera)
{
    currentCamera = camera;
}

void Scene::addMesh(Mesh* mesh)
{
    meshes.append(mesh);
}

void Scene::setSun(Sun* sun)
{
    this->sun = sun;
}

void Scene::addCamera(Camera* camera)
{
    cameras.append(camera);
}

void Scene::setSky(SkyBox* sky)
{
    this->sky = sky;
}

void Scene::setGround(Ground *ground)
{
    this->ground = ground;
}

void Scene::removeMesh(Mesh* mesh)
{
    meshes.removeOne(mesh);
}

void Scene::removeCamera(Camera* camera)
{
    cameras.removeOne(camera);
}

void Scene::removeMeshAt(int index)
{
    meshes.removeAt(index);
}


void Scene::removeCameraAt(int index)
{
    cameras.removeAt(index);
}

bool Scene::getRenderAble()
{
    return this->renderAble;
}

void Scene::setRenderAble(bool renderable)
{
    this->renderAble = renderable;
}

const QVector3D& Scene::getCameraZ()
{
    return cameraZ;
}

