#include "_include.h"

/* handle to the window */
int hWnd;

/* window width and height */
GLsizei WinWidth = 600, WinHeight = 600;

/* window width and height */
float SphereRadius = 100.0f;

float rotUpDown = 0.0f;
float rotLeftRight = 0.0f;

float CamRadius = 350.0f;
float camAngle = 0;

Octree Tree(2, 6,	-SphereRadius - 5.0f, SphereRadius + 5.0f,
					-SphereRadius - 5.0f, SphereRadius + 5.0f,
					-SphereRadius - 5.0f, SphereRadius + 5.0f);

IcoSphere myIcoSphere(SphereRadius, 5);

struct t_mouse {
	int x, y, button, state;
} Mouse;

void myReshape(int w, int h)
{
	glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    WinWidth = w;
    WinHeight = h;
}

void myMotion(int x, int y)
{
	if(Mouse.button == GLUT_LEFT_BUTTON) {
		if(Mouse.state == GLUT_DOWN) {
			rotUpDown += (y-Mouse.y);
			rotLeftRight += (x-Mouse.x);
			glutPostRedisplay();
		}
	}
	else if(Mouse.button == GLUT_RIGHT_BUTTON) {
		if(Mouse.state == GLUT_DOWN) {
			float amt = (y-Mouse.y);
			if(amt > 0) { //zoom out
				if(camAngle < 0) {
					camAngle += amt*0.1f;
					if(camAngle > 0) { camAngle = 0; }
				} else {
					CamRadius += amt;
				}
			} else if(amt < 0) { //zoom in
				if(camAngle == 0) {
					CamRadius += amt;
					if(CamRadius < 100 + myIcoSphere.Radius() ) { //init cam angle
						CamRadius = 100 + myIcoSphere.Radius();
						camAngle += amt*0.1f;
					}
				} else {
					camAngle += amt*0.1f;
				}
			}
			
			glutPostRedisplay();
		}
	}

	Mouse.x = x;
	Mouse.y = y;
}

void myPassiveMouse(int x, int y)
{
	
}

void myMouse(int button, int state, int x, int y)
{

	Mouse.x = x;
	Mouse.y = y;
	Mouse.state = state;
	Mouse.button = button;
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	static float r = 0;
	//translate the camera
	glRotatef(camAngle, 1.0f, 0, 0);
	glTranslatef(0, 0, -CamRadius);
	glRotatef(rotLeftRight, 0, 1.0f, 0);
	glRotatef(rotUpDown, 1.0f, 0, 0);
	r+=0.3f;

	DWORD t = timeGetTime();
	
	myIcoSphere.Draw();

	printf("Drawn in: %d ms\n", (DWORD)(timeGetTime() - t));

	glutSwapBuffers();
}

//void myTimer(int value)
//{
//	glutPostRedisplay();
//	glutTimerFunc(10, myTimer, 0);
//}

void myKeyboard(unsigned char key, int x, int y)
{
	if(key == 'a') {
		myIcoSphere.incLod();
		glutPostRedisplay();
	} else if(key == 'z') {
		myIcoSphere.decLod();
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //double buffered. Use glSwapBuffers()
	glutInitWindowSize(WinWidth, WinHeight);
	glutInitWindowPosition(200, 200);
	hWnd = glutCreateWindow("My IsoSphere Test"); // saves a handle to the window

	TerrainVertex** ptr = myIcoSphere.Vertices();
	int num = myIcoSphere.NumVertices();
	for(int i=0; i<myIcoSphere.NumVertices(); ++i) {
		if(!Tree.InsertItem( ptr[i] )) {
			return 0;
		}
	}

	glutDisplayFunc(myDisplay);
	glutReshapeFunc (myReshape);
	//glutTimerFunc(10, myTimer, 0);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutPassiveMotionFunc(myPassiveMouse);

	glutMainLoop();

	return 0;
}