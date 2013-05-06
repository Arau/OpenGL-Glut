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
float solar_angle[10]; 
int xWindow, yWindow, xPast, yPast;
int angle, xRot, yRot, zRot;
int type_of_draw, delay, state;
long long index;

////////////////
// Model VARs //
////////////////

struct myVertex {
    GLdouble x;
    GLdouble y;
    GLdouble z;
};

vector<Face> mod_faces;
vector<Vertex> mod_vertices;
vector<Normal> mod_normals;


myVertex model_center;

double mod_scale;
int modmat;

GLdouble m[16];

Model model;


////////////
// Camera //
////////////
myVertex obs, vrp, up;


//////////////
// Utils    //
//////////////

void getLimitVertex(myVertex &min, myVertex &max) {
        
    // inicialize
    
    min.x = mod_vertices[0];
    min.y = mod_vertices[1];
    min.z = mod_vertices[2];
    
    max.x = mod_vertices[0] + 1;
    max.y = mod_vertices[1] + 1;
    max.z = mod_vertices[3] + 1;
       
    for (int i = 0; i < mod_vertices.size(); i += 3) {
        if      (mod_vertices[i] < min.x) min.x = mod_vertices[i];
        else if (mod_vertices[i] > max.x) max.x = mod_vertices[i];
        
        if      (mod_vertices[i + 1] < min.y) min.y = mod_vertices[i+1];
        else if (mod_vertices[i + 1] > max.y) max.y = mod_vertices[i+1];
        
        if      (mod_vertices[i + 2] < min.z) min.z = mod_vertices[i+2];
        else if (mod_vertices[i + 2] > max.z) max.z = mod_vertices[i+2];
    }
}

//////////////
// Drawing  //
//////////////

void drawAxes() {          
    glBegin(GL_LINES);
            glColor3f(1, 0.4, 0.4);        
            glVertex3i(-50, 0, 0);
            glVertex3i( 50, 0, 0);
            
            glColor3f(0.4, 1, 0.4);
            glVertex3i( 0,-50, 0);
            glVertex3i( 0, 50, 0);
            
            glColor3f(1, 1, 1);
            glVertex3i( 0, 0,-50);
            glVertex3i( 0, 0, 50);
    glEnd();     
}

void drawModel() {
    glPushMatrix();   
    
    glScaled(mod_scale, mod_scale, mod_scale);    
    glTranslated(-model_center.x, -model_center.y, -model_center.z);
    glColor3f(0, 1 ,1);
    
    for (int i = 0; i < mod_faces.size(); i++) {
        glPolygonMode(GL_FRONT_AND_BACK, type_of_draw);
        glBegin(GL_POLYGON);
        
        if (modmat != mod_faces[i].mat)
            modmat = mod_faces[i].mat;        
        
        // normal 
        if (mod_faces[i].n.size() != 0) {
            int normal = mod_faces[i].n[0];
            glNormal3f(mod_normals[normal], mod_normals[normal + 1], mod_normals[normal + 2]);
        }
        else
            glNormal3f(mod_faces[i].normalC[0], mod_faces[i].normalC[1], mod_faces[i].normalC[2]);
                
        // drawing vertices
        for (int j = 0; j < mod_faces[i].v.size(); j++) {
            int vertex = mod_faces[i].v[j];
            glVertex3d(mod_vertices[vertex], mod_vertices[vertex + 1], mod_vertices[vertex + 2]);
        }       

        glEnd();    
    }
    
    glPopMatrix();
}

/////////////
//  Camera //
/////////////

void defineCamParams() {
    obs.x = 0,0;
    obs.y = 0,0;
    obs.z = 2.0;

    vrp.x = 0.0;
    vrp.y = 0.0;
    vrp.z = 0.0;

    up.x = 0.0;
    up.y = 1.0;
    up.z = 0.0;
}

void defineOrthoCam() {    
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, 0.1, 100.0);    
}

void moveCamera() {
    glMatrixMode(GL_MODELVIEW);            
        
    glLoadIdentity();
    
    cout << "Observer: " << obs.x << ' ' << obs.y << ' ' << obs.z << endl;
    
    gluLookAt(
       obs.x, obs.y, obs.z,
       vrp.x, vrp.y, vrp.z,
       up.x,  up.y,  up.z
    );    
}

void initCamera() {    
    defineCamParams();    
    defineOrthoCam();
    moveCamera();   
}

/////////////
//  Inits  //
/////////////

void initGlobals() {        
    xWindow = 1200;
    yWindow = 700;        
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(xWindow, yWindow);
    glutCreateWindow("Geo. Figures");        
}

void initBuffers() {
    glEnable(GL_DEPTH_TEST);
}

void initAngle() {
    angle = 0;
    xRot  = 0;
    yRot  = 1;
    xRot  = 0;       
    for (int i = 0; i < 10; ++i) 
        solar_angle[i] = 0;
}

void initModel(string name) {
   
    /* Model loading and computation */
    
    model = Model();
    model.load(name);
    model.dumpStats();
    cout << "model loaded" << endl;
    
    // vector load
    mod_vertices = model.vertices();
    mod_faces    = model.faces();
    mod_normals  = model.normals();             
    
    // model limits
    myVertex vertex_max;
    myVertex vertex_min;
    getLimitVertex(vertex_min, vertex_max);        
    
    // model center    
    model_center.x = (vertex_min.x + vertex_max.x)/2;
    model_center.y = (vertex_min.y + vertex_max.y)/2;
    model_center.z = (vertex_min.z + vertex_max.z)/2;
         
    // model size
    myVertex model_size;
    model_size.x = abs(vertex_max.x - vertex_min.x);
    model_size.y = abs(vertex_max.y - vertex_min.y);
    model_size.z = abs(vertex_max.z - vertex_min.z);
    
    // scale until 1 diameter sphere
    mod_scale = 1/max(model_size.x, max(model_size.y, model_size.z));
    
    cout << "Computing finished" << endl;    
}

void initTypeOfDraw() {
    type_of_draw = GL_FILL;
    initModel("homer.obj");
}

///////////////
// Callbacks //
///////////////

void onKey(unsigned char key, int x, int y) {        
    switch (key) {
        case 'h':
            cout << "press ESC to exit"                << endl;                                                                        
            cout << "press t to change draw type"      << endl;            
            cout << "press v to change scene view"     << endl;            
            break;
            
        case 27:        // ascii 27 == ESC
            exit(0); 
            break;                    
        case 't':
            if (type_of_draw == GL_FILL)
                type_of_draw = GL_LINE;
            else 
                type_of_draw = GL_FILL;
            break;          
            
        case 'v':            
            obs.x = 0; obs.y = 2; obs.z = 0; // plant view
            up.x = 1; up.y = 0; up.z = 0;
            moveCamera();
            break;
            
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
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    int i = 0;        
    
    glPushMatrix();
        glLoadIdentity();
        drawAxes(); 
    glPopMatrix();
        
    glPushMatrix();        
        // Prepare model drawing
        glMatrixMode(GL_MODELVIEW);        
        glGetDoublev(GL_MODELVIEW_MATRIX, m);               
        drawModel();                            
    glPopMatrix();

        
    glutSwapBuffers();
}

void animation() {
    if (index > 1000000000)
        index = 0;       
    
    if (index % delay == 0)
        glutPostRedisplay();
    ++index;    
}

///////////////////
// End Callbacks //
///////////////////

int main(int argc, char **argv) {    
    // Inits
    glutInit(&argc, argv);    
    initGlobals();
    initGlut();    
    initBuffers();
    initAngle();    
    initTypeOfDraw();    
    initCamera();
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);        
    glutKeyboardFunc(onKey);        
    
    glutMainLoop();
    return 0;
}
