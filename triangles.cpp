#include <GL/gl.h>
#include <glut.h>
#include <iostream>
#include <vector>
using namespace std;

void drawTriangle(void) {
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
        glVertex3f( 0,  0, 0);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
        drawTriangle();
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Triangles");

    //Callbacks
    glutDisplayFunc(renderScene);
}
