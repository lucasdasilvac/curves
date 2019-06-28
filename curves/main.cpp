#include <iostream>
#include <vector>
#include "spline.h"
#include "car3ds.h"
#include "CameraDistante.h"

using namespace std;

void draw();
void saveObj();
void loadObj();

int width = 800;
int height = 600;

int slices = 20;
int stacks = 20;

bool view1 = false;
bool view2 = false;

float tx = 0.0;
float ty = 0.0;
float tz = 0.0;

//point in local coords, to be calculated in world coords
float pl[4] = { 0.0, 0.0, 0.0, 1.0 };

//point in global coords, result of conversion from pl
float pg[4] = { 0.0, 0.0, 0.0, 1.0 };

int selectedPoint = -1;

bool lbpressed = false;
bool mbpressed = false;
bool rbpressed = false;

bool drawAxis = true;
bool drawCurve = false;
bool drawGrid = true;

float last_x = 0.0;
float last_y = 0.0;

vector<ControlPoint*> controlPoints;
int idxObject = -1;
bool showControlPoint;

TRANSFORMATIONS transformation = TRANSFORMATIONS::NONE;

vector<Camera*> cameras;
Camera* cam;

Spline* spline;
Car3ds* car;
bool moveAutomatic = false;
bool moveForward = true;

float savedCamera[9];

void resize(int w, int h)
{
    width = w;
    height = h;
}
#define BUFSIZE 512
GLuint selectBuf[BUFSIZE];
void matrixGlobal2Cam( float ex, float ey, float ez,
                       float cx, float cy, float cz,
                       float ux, float uy, float uz );
//processes the intersections
int processHits( GLint hits, GLuint buffer[] ) {
  //for each hit in buffer
    //Number of names in the name stack
    //Minimum depth of the object
    //Maximum depth of the object
    //List of names of the name stack

  int i;
  GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

  ptrNames = NULL;

  printf("Hits = %d\n",hits);
  printf("Buffer = ");
  for (i = 0; i < 4*hits; i++) {
    printf("%u ",buffer[i]);
  }
  printf("\n");

  ptr = (GLuint *) buffer;
  minZ = 0xffffffff;
  for (i = 0; i < hits; i++) {
    names = *ptr;
    //if (names == 3) {
      ptr++;
      //if ( (minZ==0xffffffff) || (*ptr < minZ && ( *(ptrNames+1)==*(ptr+3) ) ) || ( *(ptrNames+1)==0 && *(ptr+3)==1 ) ) {
      if (*ptr < minZ) {
          numberOfNames = names;
          minZ = *ptr;
          if (numberOfNames != 0)
            ptrNames = ptr+2;
      }
    //}
    ptr += names+2;
  }

  //ptr = ptrNames;
  //for (j = 0; j < numberOfNames; j++,ptr++) {
  //   printf ("%d ", *ptr);
  //}

  if (ptrNames == NULL)
    return 0;
  else
    return *ptrNames;
}

//picking
int picking( GLint cursorX, GLint cursorY, int w, int h ) {
  GLint viewport[4];

  glDisable(GL_LIGHTING);

    glSelectBuffer(BUFSIZE,selectBuf);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT,viewport);
    gluPickMatrix(cursorX,viewport[3]-cursorY,w,h,viewport);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float ar = height>0 ? (float) width / (float) height : 1.0;

    gluPerspective(30.,ar,0.1,1000.);

    glMatrixMode(GL_MODELVIEW);
    glInitNames();

    glLoadIdentity();

    //defining position and orientation of the camera
    //gluLookAt(0.0,1.0,15.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);
    //gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);
    matrixGlobal2Cam(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);

    //local system
    glPushMatrix();
        glTranslated(tx,ty,tz);
        glRotated(controlPoints[idxObject]->az,0,0,1);
        glRotated(controlPoints[idxObject]->ay,0,1,0);
        glRotated(controlPoints[idxObject]->ax,1,0,0);
        glScaled(controlPoints[idxObject]->sx, controlPoints[idxObject]->sy, controlPoints[idxObject]->sz);

        for (int i=0; i < controlPoints.size(); i++) {
            Vetor3D p = controlPoints[i]->toVector3D();
            glPushMatrix();
            glPushName(i+1);
                glTranslatef(p.x,p.y,p.z);
                glutSolidSphere(0.2,slices,stacks);
            glPopName();
            glPopMatrix();
        }
    glPopMatrix();

  glEnable(GL_LIGHTING);

//processes the intersections
    int hits;

    //restoring the original projection matrix
    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //glFlush();

    // returning to normal rendering mode
    hits = glRenderMode(GL_RENDER);

    // if there are hits process them
    if (hits != 0) {
      return processHits(hits,selectBuf);
    } else {
      return 0;
    }
}

void displayInit()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float ar = height>0 ? (float) width / (float) height : 1.0;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.,ar,0.1,1000.);
}

void displayEnd()
{
    glutSwapBuffers();
}

void draw(){
    glPushMatrix();
    if(drawAxis)
        Desenha::drawEixos(1);
    if(drawGrid)
        Desenha::drawGrid(10, 0, 10, 1);
    glPopMatrix();

    if(showControlPoint){
        for(auto p : controlPoints){
            p->drawObject();
        }
    }

    if(drawCurve){
        vector<Vetor3D> points;
        for(ControlPoint* p : controlPoints){
            points.push_back(p->getValuesTranslation());
        }
        spline->setControlPoints(points);
        spline->drawCurve();
        car->drawWithSpline(*spline, cameras[1]);
        if(moveAutomatic)
            if(moveForward){
                car->moveForward();
            }else{
                car->moveBackward();
            }
    }
}

void displayInner()
{


    if(view2 == true){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float ar = height>0 ? (float) width / (float) height : 1.0;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30.,ar,0.1,1000.);

        glMatrixMode(GL_MODELVIEW);
        //viewport 01 - top left
        glViewport(0, height/2.0, width/2.0, height/2.0);
            glLoadIdentity();
            Vetor3D eye = cameras[1%cameras.size()]->e;
            Vetor3D center = cameras[1%cameras.size()]->c;
            gluLookAt(cam->e.x, cam->e.y, cam->e.z,
                      center.x, center.y, center.z,
                      cam->u.x, cam->u.y, cam->u.z);
                draw();
        //viewport 02 - top right
        //glViewport(width/2.0, height/2.0, width/2.0, height/2.0);
            glViewport(width/2.0, height/2.0, width/2.0, height/2.0);
            glLoadIdentity();
            gluLookAt(-eye.x,eye.y,-eye.z, center.x,center.y,center.z, 0,1,0);
                draw();
        //viewport 03 - bottom left
        glViewport(0, 0, width/2.0, height/2.0);
            glLoadIdentity();
            //gluLookAt(5,1,0, 0,1,0, 0,1,0);
            gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0,1,0);
                draw();
        //viewport 04 - bottom right
        glViewport(width/2.0, 0, width/2.0, height/2.0);
            glLoadIdentity();
            //gluLookAt(0,6,0, 0,1,0, 0,0,-1);
            gluLookAt(eye.x, eye.y+1, eye.z,
                      center.x, center.y-0.9, center.z,
                      0, 1, 0);
                draw();


    }else{
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(cam->e.x, cam->e.y, cam->e.z,
                  cam->c.x, cam->c.y, cam->c.z,
                  cam->u.x, cam->u.y, cam->u.z);
        draw();
        if(view1 == true){
                glScissor(0, 3*height/4, width/4, height/4);
                glEnable(GL_SCISSOR_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDisable(GL_SCISSOR_TEST);

                glViewport(0, 3*height/4, width/4, height/4);
                glLoadIdentity();
                Vetor3D eye = cameras[1%cameras.size()]->e;
                Vetor3D center = cameras[1%cameras.size()]->c;
                gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
                draw();
                glViewport(0, 0, width, height);
            }
        glViewport(0, 0, width, height);
    }
}

void display()
{
    displayInit();
    displayInner();
    displayEnd();
}

void mouseButton(int button, int state, int x, int y) {
    // if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        // when the button is pressed
        if (state == GLUT_DOWN) {
                lbpressed = true;

        } else {
            lbpressed = false;
        }
    }
    // if the middle button is pressed
    if (button == GLUT_MIDDLE_BUTTON) {
        // when the button is pressed
        if (state == GLUT_DOWN) {
            mbpressed = true;
        } else {
            mbpressed = false;
        }
    }
    // if the left button is pressed
    if (button == GLUT_RIGHT_BUTTON) {
        // when the button is pressed
        if (state == GLUT_DOWN) {
            rbpressed = true;
        } else {
            rbpressed = false;
        }
    }

    last_x = x;
    last_y = y;
}

void mouseMove(int x, int y) {
    float factor = 10.0;
    if (lbpressed && !rbpressed && !mbpressed) {
        if (transformation == TRANSFORMATIONS::NONE) {
            cam->rotatex(y,last_y);
            cam->rotatey(x,last_x);
        } else {
            tz += (y - last_y)/factor;
            tx += (x - last_x)/factor;
            ty = controlPoints[idxObject]->getValuesTransformation(transformation).y;
            controlPoints[idxObject]->setValuesTransformation(transformation, Vetor3D(tx, ty, tz));
        }
    }
    factor = 100.0;
    if (!lbpressed && rbpressed && !mbpressed) {
        if (transformation == TRANSFORMATIONS::NONE) {
            cam->translatex(x,last_x);
            cam->translatey(y,last_y);
        } else {
            tx += (x - last_x)/factor;
            ty += -(y - last_y)/factor;
            tz = controlPoints[idxObject]->getValuesTransformation(transformation).z;
            controlPoints[idxObject]->setValuesTransformation(transformation, Vetor3D(tx, ty, tz));
        }
    }
    if (lbpressed && rbpressed && !mbpressed) {
        cam->zoom(y,last_y);
    }

    last_x = x;
    last_y = y;
}

void key(unsigned char key, int x, int y){
    switch (key)
    {
    case '1':
        view1 = !view1;
        break;
    case '2':
        view2 = !view2;
        break;
    case 13:
    case 27:
        exit(0);
        break;
    case '-':
        spline->delta *= 2;
        break;
    case '=':
        spline->delta *= 0.5;
        break;
    case 'd':
        drawCurve = !drawCurve;
        break;
    case 'g':
        drawGrid = !drawGrid;
        break;

    case 'F':
        glutFullScreen();
        break;

    case 'i':
        spline->setMode(SplineMode::INTERPOLATOR);
        break;

    case 'b':
        spline->setMode(SplineMode::BEZIER);
        break;

    case 'h':
        spline->setMode(SplineMode::HERMITE);
        break;

    case 'c':
        spline->setMode(SplineMode::CATMULLROM);
        break;

    case 's':
        spline->setMode(SplineMode::BSPLINE);
        break;

    case 'C':
        static int posCam = 0;
        posCam++;
        cam = cameras[posCam%cameras.size()];
    break;

    case 'f':
        glutReshapeWindow(800,600);
        break;

    case 'p':
        showControlPoint = !showControlPoint;
        break;

    case 'n':
        if(spline->getMode() == SplineMode::BSPLINE || spline->getMode() == SplineMode::CATMULLROM)
            controlPoints.push_back(new ControlPoint(Vetor3D(0,0,0)));
        break;
    case 'M':
            moveAutomatic = !moveAutomatic;
        break;
    case 'm':
            moveForward = !moveForward;
        break;
    case 't':
        if(transformation != TRANSFORMATIONS::TRANSLATION
                && (idxObject >= 0 && idxObject < controlPoints.size())){
            transformation = TRANSFORMATIONS::TRANSLATION;
            tx = controlPoints[idxObject]->getValuesTranslation().x;
            ty = controlPoints[idxObject]->getValuesTranslation().y;
            tx = controlPoints[idxObject]->getValuesTranslation().z;
        }
        else
            transformation = TRANSFORMATIONS::NONE;
        break;

    case '>':
        idxObject++;
        transformation = TRANSFORMATIONS::NONE;
        if(idxObject > 0 && idxObject <= controlPoints.size()){
            controlPoints[idxObject-1]->selected = false;
        }
        else if(idxObject > controlPoints.size()){
            idxObject = -1;
        }
        if(idxObject >= 0 && idxObject < controlPoints.size()){
            controlPoints[idxObject]->selected = true;
        }
        else{
            idxObject = -1;
        }
        break;

    case 'a':
    {
        drawAxis = !drawAxis;
        break;
    }

    case 'l':
        saveObj();
        break;
    case 'L':
        controlPoints.clear();
        loadObj();
        break;

    }

    glutPostRedisplay();
}

void specialKey(int key, int x, int y){
    if(key == GLUT_KEY_DOWN){
    }
    if(key == GLUT_KEY_UP){
        car->moveForward();
    }
}

void idle(void)
{
    glutPostRedisplay();
}

void initializer(int* argc, char** argv){
    //GLUT
    glutInit(argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Curves");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    glutIdleFunc(idle);

    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    //OpenGL
    glClearColor(1,1,1,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);;
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);

    glutMainLoop();
}

void matrixGlobal2Cam( float ex, float ey, float ez,
                       float cx, float cy, float cz,
                       float ux, float uy, float uz ) {
    Vetor3D e(ex,ey,ez);
    Vetor3D c(cx,cy,cz);
    Vetor3D u(ux,uy,uz);

    Vetor3D k_ = !(e - c); //!k_;
    Vetor3D i_ = !(u ^ k_); //!i_;
    Vetor3D j_ = !(k_ ^ i_); //!j_;

    Vetor3D Rt_O;
        Rt_O.x = -(i_ * e);
        Rt_O.y = -(j_ * e);
        Rt_O.z = -(k_ * e);

    float matrix[16] = { i_.x, i_.y, i_.z, Rt_O.x,
                         j_.x, j_.y, j_.z, Rt_O.y,
                         k_.x, k_.y, k_.z, Rt_O.z,
                            0,    0,    0, 1 };

    glMultTransposeMatrixf(matrix);
}



int main(int argc, char** argv)
{
    srand(time(NULL));
    cameras.push_back(new CameraDistante());
    cameras.push_back(new CameraDistante());

    cam = cameras[0];

    spline = new Spline();
    spline->addControlPoints(Vetor3D(0, 0, 0));
    spline->addControlPoints(Vetor3D(1, 1, 0));
    spline->addControlPoints(Vetor3D(5, 2, 0));
    spline->addControlPoints(Vetor3D(5, 0, 0));

    for(Vetor3D v : spline->getControlPoints()){
        controlPoints.push_back(new ControlPoint(v));
    }

    car = new Car3ds();
    initializer(&argc, argv);

    return EXIT_SUCCESS;

}

void saveObj(){
    fstream f;
    f.open("save.txt", fstream::out);
    for(auto obj : controlPoints){
        obj->saveObj(f);
        f << endl;
    }
    f.close();
}

void loadObj(){
    fstream f;
    f.open("save.txt", fstream::in);

    string type;

    float tx,ty,tz;
    float cr,cg,cb;
    float ax,ay,az;
    float sx,sy,sz;
    while(f >> type){
        f >> tx >> ty >> tz;
        f >> cr >> cg >> cb;
        f >> ax >> ay >> az;
        f >> sx >> sy >> sz;

        if(type == "Point"){
            controlPoints.push_back(new ControlPoint(Vetor3D(tx, ty, tz)));
        }else {
            printf("ERROR");
            break;
        }
    }
    f.close();
}
