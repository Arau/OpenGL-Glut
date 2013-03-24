#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

void drawTriangle(void) {
        glVertex3f(-0.5, -0.5, 0);
        glVertex3f( 0.5, -0.5, 0);
        glVertex3f( 0,  0, 0);
}

///////////////
// Callbacks //
///////////////

void onChangeWindowSize(int width, int height) {
    if (width >  height) 
        glViewport((width/2) - (height/2), 0, height, height);    
    else 
        glViewport(0, (height/2) - (width/2), width, width);                     
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Triangles");

    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);
    glutMainLoop();
    return 0;
}
