#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>

#define USEFUL_WIDTH 1280
#define USEFUL_HEIGHT 720

typedef struct{
	GLdouble eye[3];
 	GLdouble center[3];
 	GLdouble up[3];
} LookAt;

LookAt lookAt;
GLfloat fov;

void initialize(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	fov = 90;

	for(int i = 0; i < 3; i++){
		lookAt.eye[i] = 100;
		lookAt.center[i] = 0;
	}
	lookAt.up[0] = USEFUL_WIDTH / 2;
	lookAt.up[1] = USEFUL_HEIGHT;
	lookAt.up[2] = 100;
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

	gluPerspective(fov, getUsefulAreaRatio(), 0.1, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(lookAt.eye[0], lookAt.eye[1], lookAt.eye[2], lookAt.center[0], lookAt.center[1], lookAt.center[2], lookAt.up[0], lookAt.up[1], lookAt.up[2]); 
}

void callback_draw(){
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 1.0f);
	glutWireSphere(50.0, 50, 100);

	glutSwapBuffers();
 }

void callback_reshape(GLsizei width, GLsizei height){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(0, USEFUL_WIDTH, 0, USEFUL_HEIGHT, 1, 1000); // left, right, bottom, top, nearVal, farVal

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
	// lookAt test
	switch(key){
		case('w'):
			lookAt.eye[0] -= 5;
		break;
		case('s'):
			lookAt.eye[0] += 5;
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA);

	glutInitWindowSize(800, 450);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sistema Solar");

	glutDisplayFunc(callback_draw);
	glutReshapeFunc(callback_reshape);
	glutKeyboardFunc(callback_keyboardDownFunc);

	glutTimerFunc(0, callback_timerFunc, 30);

	initialize();

	glutMainLoop();

	return 0;
}
