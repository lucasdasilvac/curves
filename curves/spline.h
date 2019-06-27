#ifndef SPLINE_H
#define SPLINE_H

#include "controlpoint.h"
#include "car3ds.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

enum SplineMode {
    INTERPOLATOR, BEZIER, HERMITE, BSPLINE, CATMULLROM
};

class Spline
{
friend class Car3ds;
private:
    float M[4][4];
    Vetor3D* p;
    vector<Vetor3D> pn;
    SplineMode mode;

public:
    Spline();

    float delta{.001};
    void setControlPoints(vector<Vetor3D> controlPoints);
    void removeControlPoints();
    void addControlPoints(Vetor3D point);

    void setMode(SplineMode);
    SplineMode getMode();

    Vetor3D getPoint(float u);
    Vetor3D getD1(float u);
    Vetor3D getD2(float u);

    vector<Vetor3D> getControlPoints();
    int getQtdControlPoints();

    void drawCurve();
    void drawLocalStretch();

    friend class Car3ds;

};

#endif // SPLINE_H
