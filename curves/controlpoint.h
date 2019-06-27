#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "Object.h"

class ControlPoint : public Object
{
public:
    ControlPoint(Vetor3D valuesTranslation);
    void draw();

    string identifier();

    Vetor3D toVector3D();
    void saveObj(fstream &file);
};

#endif // CONTROLPOINT_H
