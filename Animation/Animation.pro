#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T17:14:40
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += widgets multimedia

TARGET = Animation
TEMPLATE = app


SOURCES += src/main.cpp\
        src/ui/MainWindow.cpp \
    src/ui/OpenGLWidget.cpp \
    CameraLibre.cpp \
    Objets.cpp \
    MD5Model.cpp \
    src/texture/Texture.cpp \
    src/objects/Sun.cpp \
    src/renders/Shader.cpp \
    src/materials/Material.cpp \
    src/objects/Object3D.cpp \
    src/utils/Geometry.cpp \
    src/objects/Light.cpp \
    src/renders/Renderer.cpp \
    src/objects/Scene.cpp \
    src/objects/Mesh.cpp \
    src/objects/SkyBox.cpp \
    src/materials/MaterialSun.cpp \
    src/materials/MaterialSkyBox.cpp \
    src/texture/TextureCube.cpp \
    src/renders/ShaderLibrary.cpp \
    src/objects/Camera.cpp \
    src/objects/Ground.cpp \
    src/renders/FrameBuffer.cpp \
    src/objects/Instance.cpp \
    src/materials/MaterialGround.cpp \
    src/renders/PostComposer.cpp \
    src/objects/SSQuad.cpp \
    src/utils/Matrix4.cpp


HEADERS  += src/ui/MainWindow.h \
    src/ui/OpenGLWidget.h \
    CameraLibre.h \
    Objets.h \
    MD5Model.h \
    src/texture/Texture.h \
    src/objects/Sun.h \
    src/renders/Shader.h \
    src/materials/Material.h \
    src/objects/Object3D.h \
    src/utils/Geometry.h \
    src/objects/Light.h \
    src/renders/Renderer.h \
    src/objects/Scene.h \
    src/objects/Mesh.h \
    src/objects/SkyBox.h \
    src/materials/MaterialSun.h \
    src/materials/MaterialSkyBox.h \
    src/texture/TextureCube.h \
    src/renders/ShaderLibrary.h \
    src/objects/Camera.h \
    src/objects/Ground.h \
    src/renders/FrameBuffer.h \
    src/objects/Instance.h \
    src/materials/MaterialGround.h \
    src/renders/PostComposer.h \
    src/objects/SSQuad.h \
    src/utils/Matrix4.h
