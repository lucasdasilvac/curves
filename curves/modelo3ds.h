#ifndef MODELO3DS_H
#define MODELO3DS_H

#include "model3ds.h"
#include "Object.h"

class Modelo3ds : public Object
{
protected:
    Model3DS* model;
    void saveObj(fstream &file);
};

#endif // MODELO3DS_H
