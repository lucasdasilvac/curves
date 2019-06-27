#include "Object.h"

void Object::setValuesTransformation(TRANSFORMATIONS t, Vetor3D values) {
    if(t == TRANSFORMATIONS::TRANSLATION) {
        this->setValuesTranslation(values);
    }
    else if(t == TRANSFORMATIONS::ROTATION) {
        this->setValuesRotation(values);
    }
    else if(t == TRANSFORMATIONS::SCALE) {
        this->setValuesScale(values);
    }
}

void Object::drawObject(){
    glPushMatrix();
        glTranslated(tx, ty, tz);
        glRotated(ax, 1, 0, 0);
        glRotated(ay, 0, 1, 0);
        glRotated(az, 0, 0, 1);
        if(this->axis)
            Desenha::drawEixos(1);
        glScaled(sx, sy, sz);

        draw();

   glPopMatrix();
}

void Object::drawAxis()
{
    this->axis = true;
}

void Object::hideAxis()
{
    this->axis = false;
}

void Object::setValuesScale(const Vetor3D values){
    this->sx = values.x;
    this->sy = values.y;
    this->sz = values.z;
}

void Object::setValuesTranslation(const Vetor3D values){
    this->tx = values.x;
    this->ty = values.y;
    this->tz = values.z;
}

void Object::setValuesRotation(const Vetor3D values){
    this->ax = values.x;
    this->ay = values.y;
    this->az = values.z;
}

Vetor3D Object::getValuesTransformation(TRANSFORMATIONS t)
{
    if(t == TRANSFORMATIONS::TRANSLATION)
        return this->getValuesTranslation();
    else if(t == TRANSFORMATIONS::ROTATION)
        return this->getValuesRotation();
    else if(t == TRANSFORMATIONS::SCALE)
        return this->getValuesScale();
    else
        return Vetor3D(0,0,0);
}

Vetor3D Object::getValuesTranslation()
{
    return Vetor3D(tx, ty, tz);
}

Vetor3D Object::getValuesScale()
{
    return Vetor3D(sx, sy, sz);
}

Vetor3D Object::getValuesRotation()
{
    return Vetor3D(ax, ay, az);
}
