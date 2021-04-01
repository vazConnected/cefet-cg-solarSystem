#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "libs/headers/celestial.h"
#include "libs/headers/textures.h"

#define USEFUL_WIDTH 1280
#define USEFUL_HEIGHT 720
#define STANDARD_EYE_DISTANCE 2100
#define STANDARD_DEPTH_OF_VIEW 10000

typedef struct{
	GLdouble eye[3];
 	GLdouble center[3];
 	GLdouble up[3];
} LookAt;

LookAt lookAt;
GLfloat fov;

bool orbit = false;
bool move = false;
bool light = true;

char showPlanet;

void initialize(){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND );
    glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

	fov = 75;

    showPlanet = - 1;

	for(int i = 0; i < 3; i++){
		lookAt.center[i] = 0;
	}
	lookAt.eye[0] = 1500;
	lookAt.eye[1] = 600;
	lookAt.eye[2] = 600;
	lookAt.up[0] = 0;
	lookAt.up[1] = 1;
	lookAt.up[2] = 0;

    // build celestials to draw
    // orbit center x, y and z, orbit radius x and z, position x, y and z, size, rotation e translation
    buildCelestial(&sun, 0, 0, 0, 0, 0, 0, 200, 1, 150, 0, "textures/2k_sun.jpg"); // build the sun

    // build the planets
	buildCelestial(&planets[mercury - 1],   -50, 200, 1,  270,  210,  220, 200, 1, 20, 0.8, "textures/2k_mercury.jpg");
	buildCelestial(&planets[venus - 1],     -50, 200, 1,  370,  360,  320, 200, 1, 25, 0.7, "textures/2k_venus_surface.jpg");
	buildCelestial(&planets[earth - 1],     -40, 200, 1,  490,  510,  450, 200, 1, 40, 0.6, "textures/2k_earth_daymap.jpg");
	buildCelestial(&planets[mars - 1],      -20, 200, 1,  620,  680,  600, 200, 1, 30, 0.5, "textures/2k_mars.jpg");
	buildCelestial(&planets[jupiter - 1],   -10, 200, 1,  810,  850,  800, 200, 1, 75, 0.4, "textures/2k_jupiter.jpg");
	buildCelestial(&planets[saturn - 1],    -10, 200, 1, 1040, 1000, 1030, 200, 1, 65, 0.3, "textures/2k_saturn.jpg");
	buildCelestial(&planets[uranus - 1],    -20, 200, 1, 1260, 1170, 1240, 200, 1, 60, 0.2, "textures/2k_uranus.jpg");
	buildCelestial(&planets[nepturne - 1], -150, 200, 1, 1530, 1350, 1380, 200, 1, 50, 0.1, "textures/2k_neptune.jpg");

	// build earth's atmosphere
	atmosphere = carregaTextura("textures/2k_earth_clouds.png");

    // build the satellites
	buildCelestial(&satellites[moon - 1],   0, 0, 0, 60, 60, 510, 230, 1, 10, 0.4, "textures/2k_moon.jpg");
	buildCelestial(&satellites[phobos - 1], 0, 0, 0, 50, 50, 650, 220, 1,  5, 0.3, "textures/2k_eris_fictional.jpg");
	buildCelestial(&satellites[deimos - 1], 0, 0, 0, 50, 50, 650, 190, 1,  5, 0.3, "textures/2k_makemake_fictional.jpg");

    // Os valores foram aleatorios (tentativa e erro) muito chato ter que ficar trocando
}

GLdouble getUsefulAreaRatio(){
	return (GLdouble)USEFUL_WIDTH / (GLdouble)USEFUL_HEIGHT;
}

GLdouble getWindowRatio(){
	return (GLdouble)glutGet(GLUT_WINDOW_WIDTH) / (GLdouble)glutGet(GLUT_WINDOW_HEIGHT);
}

void camera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fov, getUsefulAreaRatio(), 1, STANDARD_DEPTH_OF_VIEW);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(lookAt.eye[0], lookAt.eye[1], lookAt.eye[2], lookAt.center[0], lookAt.center[1], lookAt.center[2], lookAt.up[0], lookAt.up[1], lookAt.up[2]);
}

void illumination(){
        GLfloat luzDifusa[4] = {0.7, 0.7, 0.7, 1.0};
        GLfloat luzEspecular[4] = {1.0, 1.0, 1.0, 1.0};
        GLfloat especularidade[4] = {0.7, 0.6, 0.0, 1.0};
        GLint especMaterial = 15;

        glShadeModel(GL_SMOOTH);

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especularidade);
        glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, especMaterial);

        GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

        // ambient light
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
        glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);

        // sun light
        GLfloat lightPosition[4] = {sun.posx, sun.posy, sun.posz, 1.0};
        GLfloat sunLight[4] = {0.7, 0.6, 0.0, 1.0};
        glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT1, GL_SPECULAR, sunLight);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
}

void callback_draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawCelestials();

    if(orbit){
		drawOrbits(); // draw orbits only if desire
	}

    if(move){
		moveCelestials(); // move celestials only if desire
	}

	if(light){
        glEnable(GL_LIGHT0);
    }else{
        glDisable(GL_LIGHT0);
    }

    illumination();

    if(showPlanet > 0 && showPlanet < nepturne){
        lookAt.eye[0] = planets[showPlanet - 1].posx - 2*planets[showPlanet - 1].radius;
        lookAt.eye[1] = planets[showPlanet - 1].posy + 100;
        lookAt.eye[2] = planets[showPlanet - 1].posz - planets[showPlanet - 1].radius;
        lookAt.center[0] = planets[showPlanet - 1].posx;
        lookAt.center[1] = planets[showPlanet - 1].posy;
        lookAt.center[2] = planets[showPlanet - 1].posz;
    }else if(showPlanet == nepturne){
        float angulo = 2 * M_PI * planets[showPlanet - 1].restartT / MAX_LADOS;
        lookAt.eye[0] = planets[showPlanet - 1].posx - 300;
        lookAt.eye[1] = planets[showPlanet - 1].posy + 200;
        lookAt.eye[2] = planets[showPlanet - 1].posz - 200;
        lookAt.center[0] = planets[showPlanet - 1].posx - 50;
        lookAt.center[1] = cos(angulo)*planets[showPlanet - 1].posy + 300;
        lookAt.center[2] = sin(angulo)*planets[showPlanet - 1].orbitrz + 100;
    }else if(showPlanet == 0){
        lookAt.eye[0] = sun.posx + 200;
        lookAt.eye[1] = sun.posy + 200;
        lookAt.eye[2] = sun.posz - 200;
        lookAt.center[0] = sun.posx;
        lookAt.center[1] = sun.posy;
        lookAt.center[2] = sun.posz;
    }

	glutSwapBuffers();
}

void callback_reshape(GLsizei width, GLsizei height){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(0, USEFUL_WIDTH, 0, USEFUL_HEIGHT, 1, STANDARD_DEPTH_OF_VIEW); // left, right, bottom, top, nearVal, farVal

    if(getWindowRatio() == getUsefulAreaRatio()){
        glViewport(0, 0, width, height);
    }else if(getWindowRatio() < getUsefulAreaRatio()){
        GLfloat heightViewport = width / getUsefulAreaRatio();
        GLfloat yViewport = (height - heightViewport)/2;
        glViewport(0, yViewport, width, heightViewport);
    }else if (getWindowRatio() > getUsefulAreaRatio()){
        GLfloat widthViewport = ((GLfloat)height) * getUsefulAreaRatio();
        GLfloat xViewport = (width - widthViewport) / 2;
        glViewport(xViewport, 0, widthViewport, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	camera();
}

void callback_keyboardDownFunc(unsigned char key, GLint x, GLint y){

	switch(key){
		case('1'): // top vision
			lookAt.eye[0] = 0;
			lookAt.eye[1] = STANDARD_EYE_DISTANCE;
			lookAt.eye[2] = 0.01;
			lookAt.center[0] = 0;
            lookAt.center[1] = 0;
            lookAt.center[2] = 0;
			showPlanet = -1;
		break;
		case('2'): // side view
			lookAt.eye[0] = 0;
			lookAt.eye[1] = 0;
			lookAt.eye[2] = STANDARD_EYE_DISTANCE;
			lookAt.center[0] = 0;
            lookAt.center[1] = 0;
            lookAt.center[2] = 0;
			showPlanet = -1;
		break;
		case(' '): // planet view
			showPlanet++;
			if(showPlanet > nepturne) showPlanet = 0;
		break;
		case('r'):
		case('R'): // lookAt test
			lookAt.eye[0] = 1500;
			lookAt.eye[1] = 600;
			lookAt.eye[2] = 600;
			lookAt.center[0] = 0;
            lookAt.center[1] = 0;
            lookAt.center[2] = 0;
			showPlanet = -1;
		break;
		case('o'):
        case('O'): // request to draw (or not) orbits
            orbit = !orbit;
        break;
        case('m'):
        case('M'): // request to move (or not) the celestials
            move = !move;
        break;
        case('l'):
        case('L'): // request to move (or not) the celestials
            light = !light;
        break;
        case(27):
            exit(0);
        break;
	}
}

void callback_timerFunc(int msecs){
	camera();

    glutPostRedisplay();
    glutTimerFunc(msecs, callback_timerFunc, msecs);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(800, 450);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sistema Solar");

	glutDisplayFunc(callback_draw);
	glutReshapeFunc(callback_reshape);
	glutKeyboardFunc(callback_keyboardDownFunc);

	glutTimerFunc(0, callback_timerFunc, 60);

	initialize();

	glutMainLoop();

	return 0;
}
