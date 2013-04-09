#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

float red, green, blue;
int xWindow, yWindow, xPast, yPast;
int angle, xRot, yRot, zRot;

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
    glRotated(angle, 0, 1, 0);
    
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
        case 27:        // ascii 27 == ESC
            exit(0); 
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
    
    drawAxes(); 
    drawSnowMan();    
    
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
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);
    glutKeyboardFunc(onKey);
    
    glutMainLoop();
    return 0;
}
