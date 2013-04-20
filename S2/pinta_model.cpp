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
int type_of_draw;

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
//  Inits  //
/////////////

void initGlobals() {        
    xWindow = 900;
    yWindow = 900;    
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
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
}

void initFigureSize() {
    xSize = 1;
    ySize = 1;
    zSize = 1;
}

void initTypeOfDraw() {
    type_of_draw = GL_FILL;
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


///////////////
// Callbacks //
///////////////

void onKey(unsigned char key, int x, int y) {
    mode = 0;
    switch (key) {
        case 'h':
            cout << "press ESC to exit"           << endl;                                    
            cout << "press - to make small"       << endl;                                    
            cout << "press + to make big"         << endl;                                    
            cout << "press . to load Homer"       << endl;                                    
            cout << "press , to load Porsche"     << endl;                                    
            cout << "press t to change draw type" << endl;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    
    glPushMatrix();
        glLoadIdentity();
        drawAxes(); 
    glPopMatrix();
    
    glPushMatrix();
        
        // Geomethrics Transformations
        glRotated(angle, 0, 1, 0);                        
        glScaled(xSize, ySize, zSize);    
        
        // Prepare model drawing
        glMatrixMode(GL_MODELVIEW);        
        glGetDoublev(GL_MODELVIEW_MATRIX, m);       
        
        drawModel();    
    glPopMatrix();
        
    glutSwapBuffers();
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
    initFigureSize();    
    initTypeOfDraw();
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);
    glutKeyboardFunc(onKey);
    
    glutMainLoop();
    return 0;
}
