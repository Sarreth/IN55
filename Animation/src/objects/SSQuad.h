#ifndef SSQUAD_H
#define SSQUAD_H

#include <QOpenGLFunctions_3_2_Core>

class SSQuad : protected QOpenGLFunctions_3_2_Core
{
public:
    SSQuad();

    void init();
    void draw(unsigned int posIndex);

private:
    static const float vertices[8];
    unsigned int vbuffer;
};


#endif // SSQUAD_H
