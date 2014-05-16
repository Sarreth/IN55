#include "Ouioui.h"
#include "Animation.h"
Ouioui::Ouioui(MD5Model& model):Animation(model){

    initPositions();
}

Ouioui::~Ouioui(){
}


void Ouioui::initPositions(){

    //postion de base
    MD5Model::JointList skelton = g_model.getJointList();
    keyFrames.push_back(skelton);
    MD5Model::JointList skelton2(skelton);

    //position tête levée
    int indexHead = g_model.getIndexJointByName("head");
    //int indexNeck = g_model.getIndexJointByName("neck");
    MD5Model::Joint newJointH = rotate( skelton[indexHead],  skelton[indexHead],  QVector3D(1,0,0), 25);
    skelton[indexHead] = newJointH;

    std::vector<int> tableIndex;
    getChild(indexHead, tableIndex,skelton);
    //affichechild(tableIndex);
    int index;
    for(unsigned int j=0; j<tableIndex.size();j++){
       index =  tableIndex[j];
       MD5Model::Joint newJoint = rotate( skelton[index],  skelton[indexHead],  QVector3D(1,0,0), 25);
       skelton[index] = newJoint;
    }

    keyFrames.push_back(skelton);
    keyFrames.push_back(skelton2);
    keyFrames.push_back(skelton);
    keyFrames.push_back(skelton2);

    //afficheSkelton(skelton);
    //afficheSkelton(skelton2);

}
