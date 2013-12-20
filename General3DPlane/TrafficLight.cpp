#include <windows.h>
#include <gl/gl.h>
#include <gl/freeglut.h>

void drawTrafficLight(float size, GLfloat topEmissive[], GLfloat middleEmissive[], GLfloat bottomEmissive[]) {
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(-size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f,  size/2);
		glVertex3f(-size/2, -size*1.5f,  size/2);

		glVertex3f(-size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f, -size/2);
		glVertex3f( size/2,  size*1.5f, -size/2);
		glVertex3f(-size/2,  size*1.5f, -size/2);

		glVertex3f(-size/2, -size*1.5f, -size/2);
		glVertex3f(-size/2, -size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f, -size/2);

		glVertex3f(-size/2, -size*1.5f,  size/2);
		glVertex3f( size/2, -size*1.5f,  size/2);
		glVertex3f( size/2,  size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f,  size/2);

		glVertex3f( size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f,  size/2);
		glVertex3f( size/2,  size*1.5f,  size/2);
		glVertex3f( size/2,  size*1.5f, -size/2);
		
		glVertex3f(-size/2,  size*1.5f, -size/2);
		glVertex3f( size/2,  size*1.5f, -size/2);
		glVertex3f( size/2,  size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f,  size/2);
	glEnd();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, topEmissive);
		glTranslatef(0, size, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, middleEmissive);
		glTranslatef(0, 0, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, bottomEmissive);
		glTranslatef(0, -size, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

}

void display() {
	GLfloat material_emissive_red[] = {1.0f, 0.0f, 0.0f, 0.0f};
	GLfloat material_emissive_orange[] = {1.0f, 0.4f, 0.0f, 0.0f};
	GLfloat material_emissive_green[] = {0.0f, 1.0f, 0.0f, 0.0f};

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTrafficLight(1, material_emissive_red, material_emissive_orange, material_emissive_green);
	glutSwapBuffers();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Traffic Light");

	//Light
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_position[] = {0.0f, 1.0f, 5.0f, 0.0f};

	//assign light qualities to a light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);

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