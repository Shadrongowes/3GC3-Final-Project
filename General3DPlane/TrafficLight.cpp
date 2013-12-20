#include <windows.h>
#include <gl/gl.h>
#include <gl/freeglut.h>

void drawTrafficLight(double size) {
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_QUADS);
		glVertex3f(-size/2, -size*1.5, -size/2);
		glVertex3f( size/2, -size*1.5, -size/2);
		glVertex3f( size/2, -size*1.5,  size/2);
		glVertex3f(-size/2, -size*1.5,  size/2);

		glVertex3f(-size/2, -size*1.5, -size/2);
		glVertex3f( size/2, -size*1.5, -size/2);
		glVertex3f( size/2,  size*1.5, -size/2);
		glVertex3f(-size/2,  size*1.5, -size/2);

		glVertex3f(-size/2, -size*1.5, -size/2);
		glVertex3f(-size/2, -size*1.5,  size/2);
		glVertex3f(-size/2,  size*1.5,  size/2);
		glVertex3f(-size/2,  size*1.5, -size/2);

		glVertex3f(-size/2, -size*1.5,  size/2);
		glVertex3f( size/2, -size*1.5,  size/2);
		glVertex3f( size/2,  size*1.5,  size/2);
		glVertex3f(-size/2,  size*1.5,  size/2);

		glVertex3f( size/2, -size*1.5, -size/2);
		glVertex3f( size/2, -size*1.5,  size/2);
		glVertex3f( size/2,  size*1.5,  size/2);
		glVertex3f( size/2,  size*1.5, -size/2);
		
		glVertex3f(-size/2,  size*1.5, -size/2);
		glVertex3f( size/2,  size*1.5, -size/2);
		glVertex3f( size/2,  size*1.5,  size/2);
		glVertex3f(-size/2,  size*1.5,  size/2);
	glEnd();

	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(0, size, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1, 0.5, 0);
		glTranslatef(0, 0, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(0, -size, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

}

void display() {
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTrafficLight(1);
	glutSwapBuffers();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Traffic Light");

	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(10, 1, 1, 50);
	gluLookAt(10, -10, 30, 0, 0, 0, 0, 1, 0);

	//set clear colour to black
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_MODELVIEW);

	glutDisplayFunc(display);

	//start the program!
	glutMainLoop();
}