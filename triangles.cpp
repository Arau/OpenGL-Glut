#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

float red, green, blue;
int xWindow, yWindow, xPast, yPast;
int mode, vertex;
vector<vector<float> > coord;

void defineColor() {
    red   = 0.1;
    green = 1.0;
    blue  = 1.0;    
}

void moveColors(int x, int y) {
    //Moving right
    if (x > xPast and x < xWindow) {
        if (red   < 1.0) red   += 0.01;
        if (green < 1.0) green += 0.02;
        if (blue  < 1.0) blue  += 0.01;    
    } // Moving left
    else if (x < xPast and x > 0) {
        if (red   > 0.0) red   -= 0.01;
        if (green > 0.0) green -= 0.02;
        if (blue  > 0.0) blue  -= 0.01;    
    }
    
    // Moving up
    if (y > yPast and y < yWindow) {
        if (red   < 1.0) red   += 0.02;
        if (green < 1.0) green += 0.01;
        if (blue  < 1.0) blue  += 0.01;    
    } // Moving down
    else if (y < yPast and y > 0) {
        if (red   > 0.0) red   -= 0.02;
        if (green > 0.0) green -= 0.01;
        if (blue  > 0.0) blue  -= 0.01;        
    }
    
    // Update globals
    xPast = x; yPast = y;    
}


void drawTriangle(void) {
    glBegin(GL_TRIANGLES);
        glColor3f(red, green, blue);
        glVertex3f(-0.8, -0.8, 0);
        glVertex3f( 0.8, -0.8, 0);
        glVertex3f( 0,  1, 0);
    glEnd();
}

void defineTriangles() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
        glVertex3f(coord[0][0], coord[0][1], 0.0);
        glVertex3f(coord[1][0], coord[1][1], 0.0);
        glVertex3f(coord[2][0], coord[2][1], 0.0);
    glEnd();
}

///////////////
//  Inits  ////
///////////////

void initGlobals() {        
    xWindow = 600;
    yWindow = 600;
    mode    = 0;
    vertex  = 0;
    coord = vector<vector<float> > (3, vector<float>(2,0));
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(xWindow, yWindow);
    glutCreateWindow("Triangles");        
}

void initColors() {
    red     = 1.0f;
    green   = 1.0f;
    blue    = 1.0f;
}

///////////////
// End Inits //
///////////////

///////////////
// Callbacks //
///////////////

void onKey(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':
            cout << "press ESC to exit" << endl;
            break;
        case 27: // ascii 27 == ESC
            exit(0); 
            break;
        case 'f':
            mode = 0; // Color Mode
            break;
        case 't':
            mode = 1; // Triangle Mode
            break;
    }
}

void onMouseMove(int x, int y) {            
    if (mode == 0) 
        moveColors(x,y);
           
    glutPostRedisplay();
}

void onMouseClick(int button, int event, int x, int y) {    
    if (event == GLUT_DOWN) {
        // Color Mode
        if (mode == 0) {
            defineColor();
            
            // Update globals
            xPast = x; 
            yPast = y;
        }
        else if (mode == 1) {  // Triange mode                          
            coord[vertex][0] = (x/float(xWindow/2))-1; 
            coord[vertex][1] = 1 - (y/float(yWindow/2));
            ++vertex;                                                 
            if (vertex == 3) {                
                vertex = 0;
                glutPostRedisplay();
            }
        }
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
    if (mode == 0) {        
        drawTriangle();        
    }
    else if (mode == 1) {        
        defineTriangles();
    }
    
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
    initColors();
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);
    glutKeyboardFunc(onKey);
    
    glutMainLoop();
    return 0;
}
