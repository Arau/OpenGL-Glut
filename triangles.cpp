#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

float red, green, blue;
int xWindow, yWindow, xPast, yPast;

void drawTriangle(void) {
        glColor3f(red, green, blue);
        glVertex3f(-0.8, -0.8, 0);
        glVertex3f( 0.8, -0.8, 0);
        glVertex3f( 0,  1, 0);
}

///////////////
//  Inits  ////
///////////////

void initGlut() {    
    
    xWindow = 600;
    yWindow = 600;
    
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

void onMouseMove(int x, int y) {            
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
    
    glutPostRedisplay();
}

void onMouseClick(int button, int event, int x, int y) {
    if (event == GLUT_DOWN) {
        red   = 0.1;
        green = 1.0;
        blue  = 1.0;
    }        
    
    // Update globals
    xPast = x; yPast = y;
    
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
    glBegin(GL_TRIANGLES);        
        drawTriangle();
    glEnd();
    glutSwapBuffers();
}

/////////////////
// End Callbacks
/////////////////

int main(int argc, char **argv) {    
    // Init Globals
    glutInit(&argc, argv);
    initGlut();
    initColors();
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);
    
    glutMainLoop();
    return 0;
}
