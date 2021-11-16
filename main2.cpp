#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <iostream>

float angle = 0.0f;
float red = 1.0f, blue = 1.0f, green = 1.0f;

// Camera variables
float cx = 0.0f;
float cy = 0.0f;
float cz = 5.0f;

void drawGluCylinder( double height, double radius, int slices, int stacks );
void drawGluSlantCylinder( double height, double radiusBase, double radiusTop, int slices, int stacks );
void drawGluCylinderWithCaps( double height, double radius, int slices, int stacks );
void drawGluSlantCylinderWithCaps( double height, double radiusBase, double radiusTop, int slices, int stacks );


void changeSize(int w, int h) {
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0)
    h = 1;
  float ratio = 1.0 * w / h;

  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);

  // Reset Matrix
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the correct perspective.
  gluPerspective(45, ratio, 1, 100);

  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  // Set the camera
  gluLookAt(cx, cy, cz,
            cx, cy, 0.0f,
            0.0f, 1.0f, 0.0f);

  //glRotatef(angle, 0.0f, 1.0f, 0.0f);

  glColor3f(red, green, blue);
  glBegin(GL_TRIANGLES);
  glVertex3f(-2.0f, -2.0f, 0.0f);
  glVertex3f(2.0f, -2.0f, 0.0f);
  glVertex3f(0.0f, 2.0f, 0.0f);

  glEnd();
  //angle += 0.1f;

  glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
  switch(key) {
		// Exit
		case 27:
			exit(0);
			break;

		// Camera
		case 'x':
			cx += 0.2f;
			break;
		case 'X':
			cx -= 0.2f;
			break;
		case 'y':
			cy += 0.2f;
			break;
		case 'Y': 
			cy -= 0.2f;
			break;
		case 'z':
			cz += 0.2f;
			break;
		case 'Z': 
			cz -= 0.2f;
			break;

		// Arm
	}
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Create Cylinder");

  // Takes function paramater to draw when needed
  glutDisplayFunc(renderScene);

  // Handles reshaping window callback
  glutReshapeFunc(changeSize);

  // here is the idle func registration
  glutIdleFunc(renderScene);

  // Handles keyboard input
  glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);

  // GLUT event processing infinite loop
  glutMainLoop();

  return 1;
}


GLUquadricObj* myReusableQuadric = 0;

void drawGluCylinder( double height, double radius, int slices, int stacks ) {
	drawGluSlantCylinder( height, radius, radius, slices, stacks );
}

void drawGluSlantCylinder( double height, double radiusBase, double radiusTop, int slices, int stacks )
{
	if ( ! myReusableQuadric ) {
		myReusableQuadric = gluNewQuadric();  
		// Should (but don't) check if pointer is still null --- to catch memory allocation errors.
		gluQuadricNormals( myReusableQuadric, GL_TRUE );
	}
	// Draw the cylinder.
	gluCylinder( myReusableQuadric, radiusBase, radiusTop, height, slices, stacks );
}


void drawGluCylinderWithCaps( double height, double radius, int slices, int stacks ) {
	drawGluSlantCylinderWithCaps( height, radius, radius, slices, stacks );
}

void drawGluSlantCylinderWithCaps( double height, double radiusBase, double radiusTop, int slices, int stacks )
{
	// First draw the cylinder
	drawGluSlantCylinder( height, radiusBase, radiusTop, slices, stacks );

	// Draw the top disk cap
	glPushMatrix();
	glTranslated(0.0, 0.0, height);
	gluDisk( myReusableQuadric, 0.0, radiusTop, slices, stacks );
	glPopMatrix();

	// Draw the bottom disk cap
	glPushMatrix();
	glRotated(180.0, 1.0, 0.0, 0.0);
	gluDisk( myReusableQuadric, 0.0, radiusBase, slices, stacks );
	glPopMatrix();

}//test