#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "model.h"
using namespace std;

float red, green, blue;
float xSize, ySize, zSize;
int xWindow, yWindow, xPast, yPast;
int angle, xRot, yRot, zRot;
int mode = 0;

struct myVertex {
    GLdouble x;
    GLdouble y;
    GLdouble z;
};

myVertex mods;

vector<Face> modface;
vector<Vertex> modv;
vector<Normal> modn;

myVertex modM;
myVertex modm;
myVertex modc;

double mod_scale;
int modmat;

double modsphereR;
GLdouble m[16];

Model model;



void drawAxes() {          
    glBegin(GL_LINES);
            glColor3f(1, 0, 0);        
            glVertex3i(  0, 0, 0);
            glVertex3i( 20, 0, 0);
            
            glColor3f(0, 1, 0);
            glVertex3i( 0,  0, 0);
            glVertex3i( 0, 20, 0);
            
            glColor3f(1, 1, 1);
            glVertex3i( 0, 0,  0);
            glVertex3i( 0, 0, 20);
    glEnd();     
}

void drawModel() {
    glPushMatrix();
    glScaled(mod_scale, mod_scale, mod_scale);    
    glTranslated(-modc.x, -modc.y, -modc.z);
    //glColor3f(0, 1 ,1);
    for (unsigned int i = 0; i < modface.size(); i++) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_POLYGON);
        
        if (modmat != modface[i].mat) {
            modmat = modface[i].mat;
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Materials[modmat].ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Materials[modmat].diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Materials[modmat].specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Materials[modmat].shininess);
        }
        // la normal tambe va abans de pintar
        if (modface[i].n.size() != 0) {
            glNormal3f(modn[modface[i].n[0]], modn[modface[i].n[0] + 1], modn[modface[i].n[0] + 2]);
        }
        else {
            glNormal3f(modface[i].normalC[0], modface[i].normalC[1], modface[i].normalC[2]);
        }
        
        // pintar vertex
        for (unsigned int j = 0; j < modface[i].v.size(); j++) {
            glVertex3d(modv[modface[i].v[j]], modv[modface[i].v[j] + 1], modv[modface[i].v[j] + 2]);
        }       

        glEnd();    
    }
    glPopMatrix();
    
}
///////////////
//  Inits  ////
///////////////

void initGlobals() {        
    xWindow = 600;
    yWindow = 600;    
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(xWindow, yWindow);
    glutCreateWindow("Geo. Figures");        
}

void initAngle() {
    angle = 0;
    xRot  = 0;
    yRot  = 1;
    xRot  = 0;   
}

void initFigureSize() {
    xSize = 1;
    ySize = 1;
    zSize = 1;
}

void initModel(string name) {
    /***************************************************************/
    /* Model loading and computation *******************************/
    
    model = Model();
    model.load(name);
    model.dumpStats();
    cout << "model loaded" << endl;
    
    // vector load
    modv    = model.vertices();
    modface = model.faces();
    modn    = model.normals();
    
    double sphR;
    
    // model edges
    for (unsigned int i = 0; i < modv.size(); i += 3) {
        if      (modv[i] < modm.x) modm.x = modv[i];
        else if (modv[i] > modM.x) modM.x = modv[i];
        
        if      (modv[i + 1] < modm.y) modm.y = modv[i+1];
        else if (modv[i + 1] > modM.y) modM.y = modv[i+1];
        
        if      (modv[i + 2] < modm.z) modm.z = modv[i+2];
        else if (modv[i + 2] > modM.z) modM.z = modv[i+2];
    }
    
    // model size
    mods.x = abs(modM.x - modm.x);
    mods.y = abs(modM.y - modm.y);
    mods.z = abs(modM.z - modm.z);
    
    // model center
    modc.x = (modm.x + modM.x)/2;
    modc.y = (modm.y + modM.y)/2;
    modc.z = (modm.z + modM.z)/2;
    
    // containing sphere
    for (unsigned int i = 0; i < modv.size(); i += 3) {
        sphR = sqrt(pow(modv[i] - modc.x, 2) + pow(modv[i + 1] - modc.y, 2) + pow(modv[i + 2] - modc.z, 2));
        if (sphR > modsphereR) modsphereR = sphR;
    }
    
    // scale to fit the model within size 6 diameter sphere
    mod_scale = 6/max(mods.x, max(mods.y, mods.z));
    
    
    cout << "Computing finished" << endl;
    
    /* End model affairs *******************************************/
    /***************************************************************/

    
}

///////////////
// End Inits //
///////////////

///////////////
// Callbacks //
///////////////

void onKey(unsigned char key, int x, int y) {
    mode = 0;
    switch (key) {
        case 'h':
            cout << "press ESC to exit" << endl;                                    
            cout << "press - to make small" << endl;                                    
            cout << "press + to make big" << endl;                                    
            cout << "press v to change perspective" << endl;         
            break;
        case 27:        // ascii 27 == ESC
            exit(0); 
            break;
        case 45:        // ascii 45 == '-'            
            
            xSize -= 0.3;
            ySize -= 0.3;
            zSize -= 0.3;            
            mode = 1;
            break;
        case 43:        // ascii 43 == '+'            
            xSize += 0.3;
            ySize += 0.3;
            zSize += 0.3;            
            mode = 1;
            break;
        case '.':        
            initModel("homer.obj");
            break;
        case ',':            
            initModel("porsche.obj");
            break;
    }    
    glutPostRedisplay();
}

void onMouseMove(int x, int y) {                    
    if (angle >= 360) 
        angle = 0;
    
    if (x > xPast)  
        angle += 10;
    else 
        angle -= 10;            
        
    mode = 0;
    xPast = x; yPast = y;        
    glutPostRedisplay();
}

void onMouseClick(int button, int event, int x, int y) {    
    if (event == GLUT_DOWN) {            
        // Update globals
        xPast = x; 
        yPast = y;        
    }            
    glutPostRedisplay();
}

void onChangeWindowSize(int width, int height) {
    
    // Centered square viewport
    if (width >  height) 
        glViewport((width/2) - (height/2), 0, height, height);    
    else 
        glViewport(0, (height/2) - (width/2), width, width);    
    
    // Update globals
    xWindow = width;
    yWindow = height;
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);    
    
    glPushMatrix();
        glLoadIdentity();
        drawAxes(); 
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glGetDoublev(GL_MODELVIEW_MATRIX, m);       
    
    drawModel();    
    
    glutSwapBuffers();
}

/////////////////
// End Callbacks
/////////////////

int main(int argc, char **argv) {    
    // Inits
    glutInit(&argc, argv);    
    initGlobals();
    initGlut();    
    initAngle();
    initFigureSize();    
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);
    glutKeyboardFunc(onKey);
    
    glutMainLoop();
    return 0;
}
