#ifndef CAR3DS_H
#define CAR3DS_H

#include "modelo3ds.h"
#include "spline.h"
#include "CameraDistante.h"

class Spline;

class Car3ds : public Modelo3ds
{
private:
    float u{0.0};
    float delta{0.01};
public:
    Car3ds();
    ~Car3ds();

    string identifier();

    void moveForward();
    void moveBackward();
    void draw();
    void drawWithSpline(Spline spline, Camera *cam);
};

#endif // CAR3DS_H
