#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

float red, green, blue;
int xWindow, yWindow, xPast, yPast;
int mode, color;
double angle, xTrans, yTrans, zTrans;

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


void drawAxes() {           
        glBegin(GL_LINES);
                glColor3f(1, 0, 0);        
                glVertex3i(  0, 0, 0);
                glVertex3i( 10, 0, 0);
                
                glColor3f(0, 1, 0);
                glVertex3i( 0,  0, 0);
                glVertex3i( 0, 10, 0);
                
                glColor3f(1, 1, 1);
                glVertex3i( 0, 0,  0);
                glVertex3i( 0, 0, 10);
        glEnd();        
}

void drawTeapot() {        
    glPushMatrix();                         
        glTranslated(xTrans, yTrans, zTrans);        
        glScaled(0.5, 0.5, 0.5); 
        glRotated(angle, 0, 1, 0);
        glColor3f(red, green, blue);        
        glutWireTeapot(0.5);
    glPopMatrix();
}

///////////////
//  Inits  ////
///////////////

void initGlobals() {        
    xWindow = 600;
    yWindow = 600;
    mode    = 2;    
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(xWindow, yWindow);
    glutCreateWindow("Geo. Figures");        
}

void initColors() {
    color   = 0;
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
            cout << "0 -> No color"     << endl;
            cout << "1 -> Color mode"   << endl; 
            
            cout << endl;
            
            cout << "x -> Translate for X" << endl;
            cout << "y -> Translate for Y" << endl;
            cout << "z -> Translate for Z" << endl;            
            
            cout << endl;                       
            break;
        case 27:        // ascii 27 == ESC
            exit(0); 
            break;
        case '0':
            color = 0;
            break;
        case '1':
            color = 1;
            break;                        
    }
    glutPostRedisplay();
}

void onMouseMove(int x, int y) {                
    if (color) 
        moveColors(x,y);        
    
    xPast = x; yPast = y;
    glutPostRedisplay();
}

void onMouseClick(int button, int event, int x, int y) {    
    if (event == GLUT_DOWN) {
        // Color Mode
        if (color == 1) {
            defineColor();                        
        }     
        
        if (x > (xWindow/2)) {
            xTrans += 0.02;
            angle += 25;
        }        
        else {
            xTrans -= 0.02;            
            angle -= 25;
        }
        if (y > (yWindow/2)) 
            yTrans -= 0.02;
        
        else 
            yTrans += 0.02;
        
        if (angle == 360) 
            angle = 0;
        
        
    }            
    
    // Update globals
    xPast = x; 
    yPast = y;

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
    
    drawAxes();         
    drawTeapot();    
    
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
