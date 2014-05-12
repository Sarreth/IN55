#include "Sun.h"

#define SUN_ANGLE_STEP 0.001
#define SUN_YAW_OFFSET M_PI/10.0
#define SUN_YAW_START -SUN_YAW_OFFSET
#define SUN_YAW_END M_PI+SUN_YAW_OFFSET

Sun::Sun():Object3D(),QOpenGLFunctions_3_2_Core()
{
    this->geometry = 0;
    this->material = 0;

    hasNormals = false;
    hasTexCoords = false;

    castShadows = true;
}

Sun::Sun(Material *mat) : QOpenGLFunctions_3_2_Core()
{
    hasNormals = false;
    hasTexCoords = false;

    castShadows = true;
    float size = 2.0;

    float vertices[] = {
      -size,  size,  0.0,
      -size, -size,  0.0,
       size, -size,  0.0,
       size,  size,  0.0
    };

    float texcoords[] = {
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0
    };

    int indices[] = {0, 1, 2, 3};

    Geometry* g = new Geometry(vertices, indices, 12, 4);
    g->setTexCoords(texcoords);
    g->setPrimitive(GL_QUADS);

    geometry=g;
    material = mat;
    light = new Light();
    lightDistance = 150.0;
    lightTargetOffset = 13;
    sunDistance = 6.0;
    yaw = M_PI/3.0;
    speed = 0.0;
}

Sun::Sun(Geometry* geometry, Material* material):Object3D(),QOpenGLFunctions_3_2_Core()
{
    this->geometry = geometry;
    this->material = material;

    castShadows = true;
}

void Sun::initVBO()
{
    QOpenGLFunctions_3_2_Core::initializeOpenGLFunctions();
    hasNormals = (geometry->hasNormals() && material->getShader()->hasAttribute("normal"));
    hasTexCoords = (geometry->hasTexCoords() && material->getShader()->hasAttribute("texcoord"));

    int nbV = this->getGeometry()->getVerticesCount();
    int nbI = this->getGeometry()->getIndicesCount();
    int nbT = this->getGeometry()->getTexCoordsCount();

    //indices
    glGenBuffers(1,&indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,nbI*sizeof(int),0,GL_STATIC_DRAW);
    int* ind = (int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY);
    int* indData = this->getGeometry()->getIndices();
    for(int i = 0; i < nbI; i++)
        ind[i] = indData[i];

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    //vertices
    locVertices = material->getShader()->attribute("position");
    glGenBuffers(1,&vertices);
    glBindBuffer(GL_ARRAY_BUFFER,vertices);
    glBufferData(GL_ARRAY_BUFFER,nbV*sizeof(float),0,GL_STATIC_DRAW);
    float* vert = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
    float* vertData = this->getGeometry()->getVertices();
    for(int i = 0; i < nbV; i++)
        vert[i] = vertData[i];
    glUnmapBuffer(GL_ARRAY_BUFFER);
    if(hasNormals)
    {
        locNormals = material->getShader()->attribute("normal");
        glGenBuffers(1,&normals);
        glBindBuffer(GL_ARRAY_BUFFER,normals);
        glBufferData(GL_ARRAY_BUFFER,nbV*sizeof(float),0,GL_STATIC_DRAW);
        float* norm = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* normData = this->getGeometry()->getNormals();
        for(int i = 0; i < nbV; i++)
           norm[i] = normData[i];

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    if(hasTexCoords)
    {
        locTexCoords = material->getShader()->attribute("texcoord");
        glGenBuffers(1,&texcoords);
        glBindBuffer(GL_ARRAY_BUFFER,texcoords);
        glBufferData(GL_ARRAY_BUFFER,nbT*sizeof(float),0,GL_STATIC_DRAW);
        float* tex = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* texData = this->getGeometry()->getTexCoords();
        for(int i = 0; i < nbT; i++)
        {
           tex[i] = texData[i];
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sun::drawWithVBO()
{
    glEnableVertexAttribArray(locVertices);
    glBindBuffer(GL_ARRAY_BUFFER,vertices);
    glVertexAttribPointer(locVertices,3,GL_FLOAT,GL_FALSE,0,0);

    if(hasNormals)
    {
        glEnableVertexAttribArray(locNormals);
        glBindBuffer(GL_ARRAY_BUFFER,normals);
        glVertexAttribPointer(locNormals,3,GL_FLOAT,GL_FALSE,0,0);
    }
    if(hasTexCoords)
    {
        glEnableVertexAttribArray(locTexCoords);
        glBindBuffer(GL_ARRAY_BUFFER,texcoords);
        glVertexAttribPointer(locTexCoords,2,GL_FLOAT,GL_FALSE,0,0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indices);
    glDrawElements(geometry->getPrimitive(), geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(locVertices);
    if(hasNormals) glDisableVertexAttribArray(locNormals);
    if(hasTexCoords) glDisableVertexAttribArray(locTexCoords);

}

void Sun::debugDrawWithVBO()
{
    glEnableVertexAttribArray(locVertices);
    glBindBuffer(GL_ARRAY_BUFFER,vertices);
    glVertexAttribPointer(locVertices,3,GL_FLOAT,GL_FALSE,0,0);

    glDrawArrays(GL_POINTS, NULL, geometry->getIndicesCount());

    glDisableVertexAttribArray(locVertices);
}

Geometry* Sun::getGeometry()
{
    return geometry;
}

Material* Sun::getMaterial()
{
    return material;
}

void Sun::castsShadows(bool active)
{
    castShadows = active;
}

bool Sun::castsShadows()
{
    return castShadows;
}

float Sun::getScaleRdn()
{
    return scaleRdn;
}

void Sun::setScaleRdn(float sRdn)
{
    this->scaleRdn = sRdn;
}

float Sun::getHeightRdn()
{
    return this->heightRdn;
}

Sun::~Sun()
{
    delete geometry;
    delete material;
}

void Sun::setPourcentage(float pct)
{
    this->pourcentage = pct;
}

float Sun::getRangeScale()
{
    return rangeScale;
}

void Sun::setRangeScale(float range)
{
    this->rangeScale = range;
}

float Sun::getPourcentage()
{
    return this->pourcentage;
}

void Sun::setHeightRdn(float height)
{
    this->heightRdn = height;
}

void Sun::update(const QVector3D &positionCamera, const QVector3D &zCamera)
{
    yaw += SUN_ANGLE_STEP*speed;
    if(yaw > SUN_YAW_END) yaw = SUN_YAW_START;

    localPosition=QVector3D(std::cos(yaw), std::sin(yaw), -std::sin(yaw)/3.0);
    position = positionCamera + localPosition*sunDistance;

    matrix.lookAt(position, positionCamera, QVector3D(0,1,0));

    QVector3D offsetCamera = positionCamera + zCamera*lightTargetOffset;
    light->setPosition(offsetCamera + localPosition*lightDistance);
    light->lookAt(offsetCamera);
}

void Sun::setSpeed(float s)
{
    speed = s;
}

Light* Sun::getLight()
{
    return light;
}
