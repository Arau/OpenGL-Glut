#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

float red, green, blue;
int xWindow, yWindow, xPast, yPast;
int mode, vertex;

void defineColor() {
    red   = 0.1;
    green = 1.0;
    blue  = 1.0;    
}

void moveColors(int x, int y) {
    //Moving right
    if (x > xPast and x < xWindow) {
        if (red   < 1.0) red   += 0.005;
        if (green < 1.0) green += 0.009;
        if (blue  < 1.0) blue  += 0.005;    
    } // Moving left
    else if (x < xPast and x > 0) {
        if (red   > 0.0) red   -= 0.005;
        if (green > 0.0) green -= 0.009;
        if (blue  > 0.0) blue  -= 0.005;    
    }
    
    // Moving up
    if (y > yPast and y < yWindow) {
        if (red   < 1.0) red   += 0.009;
        if (green < 1.0) green += 0.005;
        if (blue  < 1.0) blue  += 0.006;    
    } // Moving down
    else if (y < yPast and y > 0) {
        if (red   > 0.0) red   -= 0.009;
        if (green > 0.0) green -= 0.005;
        if (blue  > 0.0) blue  -= 0.005;        
    }
    
    // Update globals
    xPast = x; yPast = y;    
}


void drawTriangle(void) {
    glBegin(GL_TRIANGLES);
        
        glColor3f(red, green, blue);
        glVertex3f(-0.8, -0.8, 0);
        
        glColor3f(green, blue, red);
        glVertex3f( 0.8, -0.8, 0);
        
        glColor3f(blue, red, green);        
        glVertex3f( 0,  1, 0);
    glEnd();
}

void drawWireCube() {
    glPushMatrix();         
        glColor3f(red, green, blue);        
        glutWireCube(1.0);
    glPopMatrix();
}

void drawTeapot() {
    glPushMatrix();         
        glColor3f(red, green, blue);        
        glutWireTeapot(0.6);
    glPopMatrix();
}

void drawTetrahedron() {
    glPushMatrix();         
        glColor3f(red, green, blue);        
        glutWireTetrahedron();
    glPopMatrix();
}

///////////////
//  Inits  ////
///////////////

void initGlobals() {        
    xWindow = 600;
    yWindow = 600;
    mode    = 0;
    vertex  = 0;    
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(xWindow, yWindow);
    glutCreateWindow("Geo. Figures");        
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
        case 't':      // Triangle
            mode = 0; 
            break;        
        case 'c':      // Cube
            mode = 1;
            break;
        case 'p':      // Teapot
            mode = 2;
            break;
        case 's':      // Sierpinski
            mode = 3;
            break;         
    }
    glutPostRedisplay();
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
        drawWireCube();
    }
    else if (mode == 2) {        
        drawTeapot();
    }
    else if (mode == 3) {        
        drawTetrahedron();
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
