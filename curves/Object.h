#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <fstream>
#include "Desenha.h"
#include "Vetor3D.h"
#include "colorrgb.h"

using namespace std;

enum TRANSFORMATIONS {
    TRANSLATION, ROTATION, SCALE, NONE
};

class Object
{
protected:
    bool axis{false};
    virtual void draw() = 0;
public:
    float sx{1.0}, sy{1.0}, sz{1.0}; //scale
    float tx{0.0}, ty{0.0}, tz{0.0}; //translation
    float ax{0.0}, ay{0.0}, az{0.0}; //rotation
    float cr{1.0}, cg{1.0}, cb{1.0}; //color
    bool selected{false};

    virtual string identifier() = 0;

    void saveObj(fstream&);
    virtual void drawObject();

    void drawAxis();
    void hideAxis();

    void setValuesTransformation(TRANSFORMATIONS t, Vetor3D values);
    void setValuesScale(const Vetor3D values);
    void setValuesTranslation(const Vetor3D values);
    void setValuesRotation(const Vetor3D values);

    Vetor3D getValuesTransformation(TRANSFORMATIONS t);
    Vetor3D getValuesTranslation();
    Vetor3D getValuesScale();
    Vetor3D getValuesRotation();

};

#endif // OBJECT_H
