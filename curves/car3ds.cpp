#include "car3ds.h"

Car3ds::Car3ds()
{
    this->model = new Model3DS("models/car.3DS");
}

string Car3ds::identifier(){
    return "car3ds";
}

void Car3ds::moveForward(){
    u += delta;
}

void Car3ds::moveBackward(){
    u -= delta;
}

void Car3ds::draw()
{
    cout << "..." << endl;
}

void Car3ds::drawWithSpline(Spline spline, Camera *cam){

    Vetor3D point;
    static int i = 0;
    for(int j = 0; j<4; j++){
        point = spline.pn[i+j];
        spline.p[j] = point;
    }

    Vetor3D z_ = spline.getD1(u) * (-1);
    Vetor3D y_ = spline.getD2(u);
    Vetor3D x_ = y_ ^ z_;
    y_ = z_ ^ x_;
    Vetor3D t_ = spline.getPoint(u);
    z_.normaliza();
    y_.normaliza();
    x_.normaliza();

    double T[] = {
        x_.x, y_.x, z_.x, t_.x,
        x_.y, y_.y, z_.y, t_.y,
        x_.z, y_.z, z_.z, t_.z,
        0   , 0   , 0   , 1
    };

    //draw car
    glPushMatrix();
        glColor3f(1, 0, 0);
        glMultTransposeMatrixd(T);
        glTranslated(-0.40, -0.01, 0);
        glRotated(85, 1, 0, 0);
        glScaled(.1,.1,.1);
        this->model->drawModel();
    glPopMatrix();

    if(u >= 1){
        u = 0;
        i++;
        if(i > spline.pn.size()-4)
            i = 0;
    }

    cam->e = spline.getPoint(u-20*delta) ;
    cam->c = spline.getPoint(u) + spline.getD1(u);
    cam->u = (spline.getD2(u-20*delta) * (-1)).getUnit();
    cam->e = cam->u + cam->e;
}
