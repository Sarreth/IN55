#ifndef OUIOUI_H
#define OUIOUI_H

#include "Animation.h"

class Ouioui: public Animation{

public:
    Ouioui(MD5Model& model);
    virtual ~Ouioui();
private:
    void initPositions();
};


#endif // OUIOUI_H
