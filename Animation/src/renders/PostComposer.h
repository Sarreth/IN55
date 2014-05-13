#ifndef POSTCOMPOSER_H
#define POSTCOMPOSER_H

#include "FrameBuffer.h"
#include "../objects/SSQuad.h"

class PostComposer
{
public:
    PostComposer(FrameBuffer* inputFBO);
    void render();
    void resize(unsigned int _width, unsigned int _height);
    void resizeHalfBuffers();

    void toggleFXAA(bool);
    void toggleDistord(bool);
    void toggleLenseFlares(bool);
    void toggleGodrays(bool);
    void toggleBloom(bool);

    void setFlareDispersal(float);
    void setFlareIntensity(float);
    void setGamma(float);

    void setLightPosScreen(const QVector3D &pos);
    void setFinalRenderSize(int w, int h);

    void debugQuad(Texture* tex);

private:
    void swapBuffers();
    void swapHalfBuffers();

    void compose();
    void display();

    void passFXAA();
    void passLenseFlare();
    void passDistortion();
    void passBlurLinear(bool useHalf = false);
    void passGodrays();
    void passBloom();


    FrameBuffer* inputBuffer;

    FrameBuffer* readBuffer;
    FrameBuffer* writeBuffer;

    FrameBuffer* readHalfBuffer;
    FrameBuffer* writeHalfBuffer;

    SSQuad* quad;

    Shader* shaderCompose;
    Shader* shaderDisplay;
    Shader* shaderFXAA;
    Shader* shaderLenseFlare1;
    Shader* shaderLenseFlare2;
    Shader* shaderLenseFlare3;
    Shader* shaderBlurLinearH;
    Shader* shaderBlurLinearV;
    Shader* shaderDistortion;
    Shader* shaderGodrays1;
    Shader* shaderGodrays2;
    Shader* shaderBloom1;
    Shader* shaderBloom2;

    bool doFXAA;
    bool doDistord;
    bool doLenseFlares;
    bool doGodrays;
    bool doBloom;

    unsigned int width;
    unsigned int height;
    float widthf;
    float heightf;
    unsigned int finalRenderWidth;
    unsigned int finalRenderHeight;

    int halfBuffersRatio;

    QVector3D gammaRGB;

    QVector3D flareThreshold;
    int flareSamples;
    float flareDispersal;
    float flareHaloWidth;
    QVector3D flareChromaDispertion;
    float flareIntensity;
    Texture* flareDirt;

    QVector3D lightPosScreen;
};

#endif // POSTCOMPOSER_H
