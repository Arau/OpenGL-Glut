#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

float red, green, blue;
float xSize, ySize, zSize;
int xWindow, yWindow, xPast, yPast;
int angle, xRot, yRot, zRot;
int mode = 0;

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

void drawSnowMan()  {        
    // Body
    glPushMatrix();              
        glRotated(90, 1, 0, 0);        
        glutWireSphere(0.4, 30, 30);            
    glPopMatrix();
    
    glPushMatrix();        
    // Head
        glTranslated(0, 0.6, 0);        
        glRotated(90, 1, 0, 0);        
        glutWireSphere(0.2, 30, 30);      
        
    // Nose               
        glTranslated(0.17, 0, 0);
        glRotated(90, 0, 1, 0);        
        glutWireCone(0.07, 0.2, 15, 15);
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

void initColors() {
    red     = 0.1f;
    green   = 0.9f;
    blue    = 0.5f;
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
        case 'v':
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
    
    glPushMatrix();        
        glRotated(angle, 0, 1, 0);        
        glRotated(90, 1, 0, 0);                        
        glScaled(xSize, ySize, zSize);
        
        drawSnowMan();         
    glPopMatrix();
    
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
