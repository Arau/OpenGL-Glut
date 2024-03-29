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
int type_of_draw, delay, state;
long long index;
int modeNormal;

////////////////
// Model VARs //
////////////////

struct myNormal {
    GLdouble x;
    GLdouble y;
    GLdouble z;
};

struct myVertex {
    GLdouble x;
    GLdouble y;
    GLdouble z;
};

vector<Face> mod_faces;
vector<Vertex> mod_vertices;
vector<Normal> mod_normals;
vector<Normal> mod_normals_vertices;

myVertex model_center;

double mod_scale;

GLdouble m[16];

Model model;

vector<myVertex> corners(4);
int cornerIndex;

////////////
// Camera //
////////////

myVertex obs, vrp, up;
double aspect;
double fovy;
double rotX, rotY, rotZ;
int zNear = 1;
int zFar  = 200;
double PI = 3.141592653589793;
int camAngle, camAngleX, camAngleY, camAngleZ;
int xMove, yMove, zMove;

////////////
// Lights //
////////////

int switchLight[] = { 1, 0, 0 };
GLfloat pos_light2[3];

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

void drawFloor() {                      
    glBegin(GL_LINES);
        for (int i = -70; i <= 70; ++i) {
            glVertex3i(i, -1,  70);  // vertical lines
            glVertex3i(i, -1, -70);
            
            glVertex3i(-70, -1, i); // horizontal lines
            glVertex3i( 70, -1, i);
        }        

    glEnd();    
                    
}

void defineModelMaterial(int modmat) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Materials[modmat].ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Materials[modmat].diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Materials[modmat].specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Materials[modmat].shininess);
}

void drawModel() {
    glPushMatrix();   
    
    glScaled(mod_scale, mod_scale, mod_scale);    
    glTranslated(-model_center.x, -model_center.y, -model_center.z);    
    
    int modmat;
    for (int i = 0; i < mod_faces.size(); i++) {
        glPolygonMode(GL_FRONT_AND_BACK, type_of_draw);
        glBegin(GL_POLYGON);
        
        if (modmat != mod_faces[i].mat) {
            modmat = mod_faces[i].mat; 	            
            defineModelMaterial(modmat);            
        }
        
        
        if (modeNormal == 0) {
            // normal 
            if (mod_faces[i].n.size() != 0) {            
                int normal = mod_faces[i].n[0];
                glNormal3f(mod_normals[normal], mod_normals[normal + 1], mod_normals[normal + 2]);     
            }
            else
                glNormal3f(mod_faces[i].normalC[0], mod_faces[i].normalC[1], mod_faces[i].normalC[2]);                        
            
            for (int j = 0; j < mod_faces[i].v.size(); j++) {
                int vertex = mod_faces[i].v[j];
                glVertex3d(mod_vertices[vertex], mod_vertices[vertex + 1], mod_vertices[vertex + 2]);
            }       
        }
        else {
            for (int j = 0; j < mod_faces[i].v.size(); j++) {
                int vertex = mod_faces[i].v[j];
                glNormal3f(mod_normals_vertices[vertex],mod_normals_vertices[vertex+1],mod_normals_vertices[vertex+2]);
                glVertex3d(mod_vertices[vertex], mod_vertices[vertex + 1], mod_vertices[vertex + 2]);
            }       
        }
        glEnd();    
    }
    
    glPopMatrix();
}

/////////////
//  Lights //
/////////////

void updateLightPos() {
    pos_light2[0] = corners[cornerIndex].x;
    pos_light2[1] = corners[cornerIndex].y;    
    pos_light2[2] = corners[cornerIndex].z;                
}

void moveLight() {        
    ++cornerIndex;
    if (cornerIndex == 4) 
        cornerIndex = 0;        
    
    updateLightPos();    
}

void switchOn(int light) {
    int gLight = 0;
    if (light == 0) gLight = GL_LIGHT0;
    else if (light == 1) gLight = GL_LIGHT1;
    else if (light == 2) gLight = GL_LIGHT2;
    
    glEnable(gLight);
}

void switchOff(int light) {  
    int gLight = 0;
    if (light == 0) gLight = GL_LIGHT0;
    else if (light == 1) gLight = GL_LIGHT1;
    else if (light == 2) gLight = GL_LIGHT2;
    
    glDisable(gLight);  
}

void switchingLight(int light) {
    if (switchLight[light] == 0) {
          switchLight[light] = 1;
          switchOn(light);
    }
    else {
        switchLight[light] = 0;
        switchOff(light);
    }
}


void defineLight(int light, float colors[]) {
    
    GLfloat light_ambient[]  = { colors[0], colors[1], colors[2], 1.0 };
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };    
    
    glLightfv (light, GL_AMBIENT, light_ambient);
    glLightfv (light, GL_DIFFUSE, light_diffuse);
    glLightfv (light, GL_SPECULAR, light_specular);    
}


void initLights() {         
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0); 
    float colors[] = { 0.75, 0.75, 0.75 };
    defineLight(GL_LIGHT1, colors);
    defineLight(GL_LIGHT2, colors);
    
}

void defineShiniMaterial(float R, float G, float B) {
    GLfloat ambient[]  = {0.1, 0.1, 0.1, 1.0f};     
    GLfloat specular[] = {0.9, 0.9, 0.9, 1.0f};
    GLfloat diffuse[]  = {R, G, B, 1.0f};
    
    glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
    glMaterialf (GL_FRONT, GL_SHININESS, 100.0f);
}

/////////////
//  Camera //
/////////////

void defineCamParams() {
    obs.x = 0,0;
    obs.y = 0,0;
    obs.z = 6.0;

    vrp.x = 0.0;
    vrp.y = 0.0;        
    vrp.z = 0.0;

    up.x = 0.0;
    up.y = 1.0;
    up.z = 0.0;
    
    fovy = 2*((atan(0.25)*180/PI));  // arctan. to Degs.        
    camAngle = 0; camAngleX = 0; camAngleY = 0; camAngleZ = 0;    
    rotX = 0; rotY = 0;  rotZ = 0;
    xMove = 0; yMove = 0; zMove = 0;
}

void moveCameraLookAt() {        
    glMatrixMode(GL_MODELVIEW);                    
    glLoadIdentity();            
    gluLookAt(
       obs.x, obs.y, obs.z,
       vrp.x, vrp.y, vrp.z,
       up.x,  up.y,  up.z
    );    
}

void moveCameraEuler() {    
    glMatrixMode(GL_MODELVIEW);   
    GLdouble m[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    glLoadIdentity();    
    glRotatef(camAngle, rotX, rotY, rotZ);    
    glMultMatrixd(m);
}

void definePersCam() {    
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();    
    gluPerspective(fovy, aspect, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);   
    moveCameraLookAt();       
}

void initCamera() {      
    defineCamParams();    
    definePersCam();    
}

/////////////
//  Inits  //
/////////////

void initGlobals() {        
    xWindow = 1200;
    yWindow = 1200; 
    aspect  = xWindow/yWindow;
}

void initGlut() {        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(xWindow, yWindow);
    glutCreateWindow("Geo. Figures");        
}

void initBuffers() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void initAngle() {
    angle = 0;
    xRot  = 0;
    yRot  = 1;
    xRot  = 0;          
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
    mod_normals_vertices.resize(mod_vertices.size());
      
    for (unsigned int i = 0; i < mod_faces.size(); ++i) {
        for (int j = 0; j < mod_faces[i].v.size(); j++) {            
            int vertex = mod_faces[i].v[j];            
            mod_normals_vertices[vertex] += mod_normals[vertex];
            mod_normals_vertices[vertex+1] += mod_normals[vertex+1];
            mod_normals_vertices[vertex+2] += mod_normals[vertex+2];
        }
    }    
    
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
    mod_scale = 2/max(model_size.x, max(model_size.y, model_size.z));
    
    modeNormal = 0;    
    cout << "Computing finished" << endl;    
}

void initTypeOfDraw() {
    type_of_draw = GL_FILL;
    initModel("porsche.obj");
}

void initCorners() {    
    
    corners[0].x = -70;
    corners[0].y =  -1;
    corners[0].z =  70;    
    
    corners[1].x = -70;
    corners[1].y =  -1;
    corners[1].z = -70;
    
    corners[2].x =  70;
    corners[2].y =  -1;
    corners[2].z = -70;
    
    corners[3].x =  70;
    corners[3].y =  -1;
    corners[3].z =  70; 
    
    cornerIndex = 0;
}

///////////////
// Callbacks //
///////////////

void onKey(unsigned char key, int x, int y) {        
    switch (key) {
        case 'h':
            cout << "press ESC to exit"                << endl;                                                                        
            cout << "press t to change draw type"      << endl;
            cout << endl;
            
            cout << "Entrega:" << endl;
            cout << "\t press 'c' to Switch On/Off light 0" << endl;
            cout << "\t press 'f' to Switch On/Off light 2" << endl;
            cout << "\t press 's' to Move light between floor corners" << endl;
            
            cout << "Lights: \n" << endl;
            cout << "\t press 'c' to Switch On/Off light 0" << endl;
            cout << "\t press 'v' to Switch On/Off light 1" << endl;
            cout << "\t press 'f' to Switch On/Off light 2" << endl;
            cout << "\t press 's' to Move light between floor corners" << endl;
            cout << "\t press 'z' to Switch Normal style Face/Vertice" << endl;
            
            cout << "Move Camera: \n" << endl;
            cout << "\tpress 'q' to move respect Y axe, hourly "    << endl;
            cout << "\tpress 'r' to move respect Y axe, (-)hourly " << endl;
            cout << endl;
            cout << "\tpress 'w' to move respect (model) Z axe, hourly"     << endl;
            cout << "\tpress 'e' to move respect (model) Z axe, (-)hourly" << endl;
            cout << endl;
            
            break;
            
        case 27:        // ascii 27 == ESC
            exit(0); 
            break;                    
        	                           
        case 'p':
            definePersCam();
            break;
                
        case 'n':    
            fovy -= 3;
            definePersCam();    
            break;
            
        case 'b':                        
            fovy += 3;
            definePersCam();            
            break;                    
                    
        case '1':
            if (camAngleY > 0) 
                camAngleY = 0;
            
            camAngleY -= 1;
            rotX = 0; rotY = 1; rotZ = 0;
            
            camAngle = camAngleY;
            moveCameraEuler();
            break;
            
        case '2':
            if (camAngleY > 0) 
                camAngleY = 0;
            
            camAngleY -= 1;
            rotX = 1; rotY = 0; rotZ = 0;
            
            camAngle = camAngleY;
            moveCameraEuler();
            break;
            
        case '3':
            if (camAngleX < 0) 
                camAngleX = 0;
            
            camAngleX += 1;
            rotX = 1; rotY = 0; rotZ = 0;
            
            camAngle = camAngleX;
            moveCameraEuler();
            break;
          
        case '4':
            if (camAngleX < 0) 
                camAngleX = 0;
            
            camAngleX += 1;
            rotX = 0; rotY = 1; rotZ = 0;                

            camAngle = camAngleX;            
            moveCameraEuler();
            break;            
	   
	case 'q':
	    glMatrixMode(GL_MODELVIEW);	    
	    glRotatef(10, 0, 1, 0);
	    break;

	case 'r':
	    glMatrixMode(GL_MODELVIEW);	    
	    glRotatef(-10, 0, 1, 0);	    
	    break;
	    
	case 'w':
	    glMatrixMode(GL_MODELVIEW);
	    glRotatef(10, 0, 0, 1);
	    break;
	
	case 'e':
	    glMatrixMode(GL_MODELVIEW);
	    glRotatef(-10, 0, 0, 1);
	    break;
	    
	case 'c':
        switchingLight(0); //Light 0	    
	    break;
        
    case 'v':
        switchingLight(1); //Light 1       
        break;
        
    case 'f':  
        updateLightPos();
        switchingLight(2); //Light 2       
        break;
        
    case 's':
        moveLight();
        break;        
        
    case 'z':
        if (modeNormal == 0) modeNormal = 1;
        else modeNormal = 0;
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
    aspect  = xWindow/yWindow;
}

void renderScene() {                    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    
    glMatrixMode(GL_MODELVIEW);        
    
    if (switchLight[1] != 0) {
        GLfloat pos_light1[] = { 30.0, 6.0, 0.0, 1.0 };
        glLightfv (GL_LIGHT1, GL_POSITION, pos_light1);        
    }
    if (switchLight[2] != 0) {        
        glLightfv (GL_LIGHT2, GL_POSITION, pos_light2);    
    }
        
    defineShiniMaterial(0.0, 0.0, 1.0);    
    drawFloor();     
    
    glPushMatrix();        
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
    initTypeOfDraw();  
    initCorners();
    initCamera();
    initLights();
    
    //Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onChangeWindowSize);        
    glutKeyboardFunc(onKey);        
    
    glutMainLoop();
    return 0;
}
