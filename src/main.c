#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>

#define USEFUL_WIDTH 1280
#define USEFUL_HEIGHT 720
#define STANDARD_EYE_DISTANCE 2500
#define STANDARD_DEPTH_OF_VIEW 10000

typedef struct{
	GLdouble eye[3];
 	GLdouble center[3];
 	GLdouble up[3];
} LookAt;

LookAt lookAt;
GLfloat fov;

void initialize(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	fov = 60;

	for(int i = 0; i < 3; i++){
		lookAt.eye[i] = 500;
		lookAt.center[i] = 0;
	}
	lookAt.up[0] = 0;
	lookAt.up[1] = 1;
	lookAt.up[2] = 0;
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

void callback_draw(){
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireSphere(350, 50, 50);
	/*
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 0); // red
		glVertex3f(50, 1, 0); // X
		glColor3f(0, 1, 0); // green
		glVertex3f(0, 50, 0); // Y
		glColor3f(0, 0, 1); // blue
		glVertex3f(0, 0, 50); // Z
	glEnd();
	*/

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
		break;
		case('2'): // side view
			lookAt.eye[0] = 0;
			lookAt.eye[1] = 0;
			lookAt.eye[2] = STANDARD_EYE_DISTANCE;
		break;
		case('r'):
		case('R'): // lookAt test
			lookAt.eye[0] = 500;
			lookAt.eye[1] = 500;
			lookAt.eye[2] = 500;
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
