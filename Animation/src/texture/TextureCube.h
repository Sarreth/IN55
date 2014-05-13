#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "Texture.h"

class TextureCube : QOpenGLFunctions_3_2_Core
{
public:
    TextureCube(QString texDir, QString ext = ".jpg", unsigned int _index = 0, unsigned int _size = 640);
    ~TextureCube();

    void init();
    void bind();

    void load(QString texDir, QString ext = ".jpg");

    // Needs to be public to be accessed by GL calls
    unsigned int gluid;
    unsigned int glunit;
    unsigned int index;

private:
    QList<QImage> images;
    unsigned int size;
};

#endif // TEXTURECUBE_H
