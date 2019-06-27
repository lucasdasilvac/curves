#include "spline.h"

Spline::Spline(){
    p = new Vetor3D[4];
    this->setMode(SplineMode::BSPLINE);
}

void Spline::setControlPoints(vector<Vetor3D> controlPoints){
    pn.clear();
    for(Vetor3D p : controlPoints){
        this->pn.push_back(p);
    }

    for(int i = 0; i < pn.size() && pn.size() <= 4; i++){
        p[i] = pn[i];
    }

}

void Spline::removeControlPoints(){
    this->pn.clear();
    delete p;
    p = new Vetor3D[4];
}

void Spline::addControlPoints(Vetor3D point){
    this->pn.push_back(point);
}

void Spline::setMode(SplineMode mode){
    this->mode = mode;

    if(mode == SplineMode::INTERPOLATOR){
        float Maux[4][4] = {
            {1, 0, 0, 0},
            {-5.5, 9, -4.5, 1},
            {9, -22.5, 18, -4.5},
            {-4.5, 13.5, -13.5, 4.5}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }

    if(mode == SplineMode::HERMITE){
        float Maux[][4] = {
            {2, -2, 1, 1},
            {-3, 3, -2, -1},
            {0, 0, 1, 0},
            {1, 0, 0, 0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
    else if(mode == SplineMode::BEZIER){
        float Maux[][4] = {
            {-1, 3, -3, 1},
            {3, -6, 3, 0},
            {-3, 3, 0, 0},
            {1, 0, 0, 0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }

    }
    else if(mode == SplineMode::BSPLINE){
        float Maux[][4] = {
            {-1/6.0, 3/6.0, -3/6.0, 1/6.0},
            {3/6.0, -6/6.0, 3/6.0, 0/6.0},
            {-3/6.0, 0/6.0, 3/6.0, 0/6.0},
            {1/6.0, 4/6.0, 1/6.0, 0/6.0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
    else if(mode == SplineMode::CATMULLROM){
        float Maux[][4] = {
            {-1/2.0, 3/2.0, -3/2.0, 1/2.0},
            {2/2.0, -5/2.0, 4/2.0, -1/2.0},
            {-1/2.0, 0.0, 1/2.0, 0.0},
            {0.0, 2/2.0, 0.0, 0.0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
}

SplineMode Spline::getMode(){
    return this->mode;
}

Vetor3D Spline::getPoint(float u){

    Vetor3D result;
    float us[] = {pow(u, 3), pow(u, 2), u, 1};

    float matrix[4];

    for(int i = 0; i<4; i++){
        matrix[i] = 0;
        for(int j = 0; j<4; j++){
            matrix[i] += us[j] * M[j][i];
        }
    }

    for(int i = 0; i<4; i++){
        result.x += matrix[i] * p[i].x;
        result.y += matrix[i] * p[i].y;
        result.z += matrix[i] * p[i].z;
    }

    return result;
}

Vetor3D Spline::getD1(float u){

    Vetor3D result;
    float us[] = {3*pow(u, 2), 2*u, 1, 0};

    float matrix[4];

    for(int i = 0; i<4; i++){
        matrix[i] = 0;
        for(int j = 0; j<4; j++){
            matrix[i] += us[j] * M[j][i];
        }
    }

    for(int i = 0; i<4; i++){
        result.x += matrix[i] * p[i].x;
        result.y += matrix[i] * p[i].y;
        result.z += matrix[i] * p[i].z;
    }

    return result;

}

Vetor3D Spline::getD2(float u){

    Vetor3D result;
    float us[] = {6*u, 2, 0, 0};

    float matrix[4];

    for(int i = 0; i<4; i++){
        matrix[i] = 0;
        for(int j = 0; j<4; j++){
            matrix[i] += us[j] * M[j][i];
        }
    }

    for(int i = 0; i<4; i++){
        result.x += matrix[i] * p[i].x;
        result.y += matrix[i] * p[i].y;
        result.z += matrix[i] * p[i].z;
    }

    return result;

}

vector<Vetor3D> Spline::getControlPoints(){
    return pn;
}

int Spline::getQtdControlPoints(){
    return pn.size();
}

void Spline::drawCurve(){

    Vetor3D point;
    for(int i = 0; i <= pn.size()-4; i++){

        for(int j = 0; j<4; j++){
            point = pn[i+j];
            p[j] = point;
        }

        for(float u = 0; u <= 1; u += delta){
            Vetor3D z_ = getD1(u) * (-1);
            Vetor3D y_ = getD2(u);
            Vetor3D x_ = y_ ^ z_;
            y_ = z_ ^ x_;
            Vetor3D t_ = getPoint(u);
            z_.normaliza();
            y_.normaliza();
            x_.normaliza();

            double T[] = {
                x_.x, y_.x, z_.x, t_.x,
                x_.y, y_.y, z_.y, t_.y,
                x_.z, y_.z, z_.z, t_.z,
                0   , 0   , 0   , 1
            };

            glPushMatrix();
                glColor3f(0, 0, 0);
                glMultTransposeMatrixd(T);
                drawLocalStretch();
            glPopMatrix();

        }
    }

}

void Spline::drawLocalStretch(){
    glPushMatrix();
        glTranslated(0.25, 0, 0);
        glRotated(90, 0, 1, 0);
        glRotated(180, 1, 0, 0);
        glScaled(1,1,2);
        glPushMatrix();
            glColor3f(.4,.3,.4);
            Desenha::drawQuadFace(Vetor3D(0,0,0), Vetor3D(.3, 0, .6));
        glPopMatrix();
    glPopMatrix();
}
