#ifndef ANIMATION_H
#define ANIMATION_H
#include "MD5Model.h"


class Animation{

public:
    Animation(MD5Model& model);
    virtual ~Animation();
    MD5Model getModel();
    bool runDeltaAnimation();


protected:
    MD5Model& g_model;
    std::vector<MD5Model::JointList> keyFrames;
    float m_progress;
    int m_KeyFrameIndex;

    MD5Model::Joint rotate(MD5Model::Joint child, MD5Model::Joint father, QVector3D axis, qreal angle);
    void updateModel();
    QQuaternion multiply(QQuaternion q1, QQuaternion q2);
    void afficheSkelton(MD5Model::JointList& jl);
    void getChild(int idJoint, std::vector<int> &tableIndex, MD5Model::JointList jl);
    void affichechild(std::vector<int> tableIndex);
    void affichechildByName(std::string name);
    virtual void initPositions()=0;

};

#endif // ANIMATION_H
