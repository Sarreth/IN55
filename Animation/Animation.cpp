#include "Animation.h"
using namespace std;


Animation::Animation(MD5Model& model):g_model(model),m_progress(0),m_KeyFrameIndex(0){
    affichechildByName("head");
}

Animation::~Animation(){
}

MD5Model Animation::getModel(){
    return g_model;
}


MD5Model::Joint Animation::rotate(MD5Model::Joint child, MD5Model::Joint father, QVector3D axis, qreal angle){
    MD5Model::Joint newJoint;
    QQuaternion qRotation = QQuaternion::fromAxisAndAngle(axis,angle);
    qRotation.normalize();
    QQuaternion qInvRotation = qRotation.conjugate();
    newJoint.m_Name = child.m_Name;
    newJoint.m_ParentID = child.m_ParentID;
    //newJoint.m_Orient = qRotation * child.m_Orient * qInvRotation;
    newJoint.m_Orient = multiply(qRotation,child.m_Orient);

    newJoint.m_Pos = child.m_Pos;
    newJoint.m_Pos -= father.m_Pos;
    //newJoint.m_Pos = (qRotation * QQuaternion(newJoint.m_Orient) * qInvRotation).vector();
    newJoint.m_Pos = multiply(multiply(qRotation,QQuaternion(newJoint.m_Pos)),qInvRotation).vector();
    newJoint.m_Pos += father.m_Pos;
    return newJoint;
}


QQuaternion Animation::multiply(QQuaternion q1, QQuaternion q2){
    QQuaternion newQ;
    newQ.setScalar( q1.scalar()*q2.scalar() - q1.x()*q2.x() - q1.y()*q2.y() - q1.z()*q2.z());
    newQ.setX(q1.scalar()*q2.x() + q1.x()*q2.scalar() + q1.y()*q2.z() - q1.z()*q2.y());
    newQ.setY( q1.scalar()*q2.y() + q1.y()*q2.scalar() - q1.x()*q2.z() + q1.z()*q2.x());
    newQ.setZ( q1.z()*q2.scalar() + q1.scalar()*q2.z() + q1.x()*q2.y() - q1.y()*q2.x());
    return newQ;
}


void Animation::updateModel(){
    g_model.update();
}


bool Animation::runDeltaAnimation(){

    MD5Model::JointList skelton1 = keyFrames[m_KeyFrameIndex];
    MD5Model::JointList skelton2 = keyFrames[m_KeyFrameIndex+1];
    MD5Model::JointList newSkelton;


    for(unsigned int i=0; i<skelton1.size(); i++){
        MD5Model::Joint joint1 = skelton1[i];
        MD5Model::Joint joint2 = skelton2[i];
        QQuaternion qPos1 = QQuaternion(0,joint1.m_Pos);
        QQuaternion qPos2 = QQuaternion(0,joint2.m_Pos);

        MD5Model::Joint newJoint;
        newJoint.m_Name = joint1.m_Name;
        newJoint.m_ParentID = joint1.m_ParentID;
        newJoint.m_Pos = QQuaternion::slerp(qPos1,qPos2,m_progress).vector();
        newJoint.m_Orient = QQuaternion::slerp(joint1.m_Orient,joint2.m_Orient,m_progress);

        newSkelton.push_back(newJoint);
    }
    //afficheSkelton(newSkelton);
    g_model.setJointList(newSkelton);
    updateModel();
    m_progress = m_progress + 0.02;
    if(m_progress > 1){
        m_progress = 0;
        m_KeyFrameIndex = m_KeyFrameIndex +1;
        if(m_KeyFrameIndex >= keyFrames.size()-1){
            m_KeyFrameIndex = 0;
            m_progress = 0;
            return true;
        }
    }
    return false;
}


void Animation::afficheSkelton(MD5Model::JointList& jl){
        for(unsigned int j = 0; j<jl.size();j++)
        {
            MD5Model::Joint joint = jl[j];
            if(joint.m_Name=="origin")
            qDebug() << "JOINT " << qPrintable(QString::fromStdString(joint.m_Name)) << joint.m_ParentID << joint.m_Orient << joint.m_Pos;

        }
}


void Animation::getChild(int idJoint, std::vector<int>& tableIndex, MD5Model::JointList jl ){
    for(unsigned int i=0; i<jl.size();i++){
        MD5Model::Joint joint = jl[i];
        if(joint.m_ParentID == idJoint){
            tableIndex.push_back(i);
            getChild(i,tableIndex,jl);
        }
    }
}

void Animation::affichechild(std::vector<int> tableIndex){
    qDebug() << "TABLEINDEX";
    MD5Model::JointList jl = g_model.getJointList();
    for(unsigned int i=0; i<tableIndex.size();i++){
        int j = tableIndex[i];
       qDebug() << qPrintable(QString::fromStdString(jl[j].m_Name));
    }
}

void Animation::affichechildByName(string name){
    int indexHead = g_model.getIndexJointByName(name);
    std::vector<int> tableIndex;
    getChild(indexHead, tableIndex,g_model.getJointList());
    affichechild(tableIndex);
}
