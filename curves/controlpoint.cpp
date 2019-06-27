#include "controlpoint.h"

ControlPoint::ControlPoint(Vetor3D valuesTranslation)
{
    this->tx = valuesTranslation.x;
    this->ty = valuesTranslation.y;
    this->tz = valuesTranslation.z;
}

void ControlPoint::draw(){
    if(!selected)
        glColor3f(0, 0, 1);
    else
        glColor3f(1, 1, 0);
    glutSolidSphere(.1, 36, 36);
}

string ControlPoint::identifier(){
    return "controlPoint";
}

Vetor3D ControlPoint::toVector3D(){
    return Vetor3D(tx, ty, tz);
}

void ControlPoint::saveObj(fstream &file){
    file << "Point" << " ";
    file << this->tx << " " << this->ty << " " << this->tz << " ";
    file << this->cr << " " << this->cg << " " << this->cb << " ";
    file << this->ax << " " << this->ay << " " << this->az << " ";
    file << this->sx << " " << this->sy << " " << this->sz << " ";
}
