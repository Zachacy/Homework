#include <GL/glut.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <iostream>
#include <vector>
#include <time.h>
#include "SMFLoader.h"
#define Rotate_range 60
double arm_offset = 1;
double leg_offset = 1;
double leg_offset_y = 1;
GLdouble size = 1;
GLdouble offset;
GLdouble offset_y;
GLdouble Rotate_arm = Rotate_range;
GLdouble Rotate_leg = Rotate_range;
GLdouble Rotate_leg_y = 0;
SMFLoader loader;   //(2) declare SMFLoader
float *vertex;
float *color;
float *normal;
GLuint id;
bool flag_jump = false;
bool flag_rotate_arm = false;
bool flag_rotate_leg = false;
bool flag_rotate_leg_y = false;

bool flag_animat = false;
GLUquadric * quad;
float angle = 0;
float jump_height = 0;
float animat_dis = 0;
float animat_rotate = 0;
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
void display();
void SetLightSource();
void SetMaterial();
void keyboard(unsigned char key, int x, int y);
void draw_duck();
void init();
void makeCheckImage();
void idle(void)
{
	if (angle >= 360)
		angle = 0.1f;
	else
		angle += 0.5;

	if (flag_jump == true)
		jump_height += 0.1f;
	if (jump_height >= 5)
		flag_jump = false;
	if (jump_height > 0 && flag_jump == false) {
		jump_height -= 0.1f;
		if (jump_height <= 0)
			jump_height = 0;
	}

	if (flag_animat == true) {
		animat_dis += 0.1f;
		animat_rotate = 90;
	}
		
	if (animat_dis >= 10)
		flag_animat = false;
	if (animat_dis > 0.0 && flag_animat == false) {
		animat_dis -= 0.1f;
		if (animat_dis <= 0) {
			animat_dis = 0;
			animat_rotate = 0;
		}
			
	}

	display();
}
void makeCheckImage(void)//draw square with dark/white
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}
//texture mapping
void init(void)
{

	quad = gluNewQuadric();
	glEnable(GL_TEXTURE_2D); ///

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glDisable(GL_TEXTURE_2D);
}
// Set environment- light 
void SetLightSource()
{
	float light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_position[] = { 0, 10, 100 };
	glEnable(GL_LIGHTING);


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}
// Set object reflection propety
void SetMaterial()
{
	float material_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	float material_diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	float material_specular[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
}
// draw_duck() : load duck model
void draw_duck() {
	glPolygonMode(GL_BACK, GL_LINE);
	for (int i = 0; i<loader.getFaceSize(); i++)
	{
		// { x1,y1,z1,x2,y2,z2,x3,y3,z3 }

		vertex = loader.getFace(i);
		// { r1,g1,b1,r2,g2,b2,r3,g3,b3 }
		color = loader.getFaceColor(i);
		normal = loader.getNormal(i);
		glBegin(GL_TRIANGLES);
		//if(color)

		glNormal3f(normal[0], normal[1], normal[2]);
		glColor3f(color[0], color[1], 0);
		glVertex3f(vertex[0], vertex[1], vertex[2]);

		//if(color)
		glNormal3f(normal[3], normal[4], normal[5]);
		glColor3f(color[3], color[4], 0);
		glVertex3f(vertex[3], vertex[4], vertex[5]);

		//if(color)
		glNormal3f(normal[6], normal[7], normal[8]);
		glColor3f(color[6], color[7], 0);
		glVertex3f(vertex[6], vertex[7], vertex[8]);

		glEnd();
		free(vertex);
		free(color);
		free(normal);
	}
}
// click event of keyboard 
void keyboard(unsigned char key, int x, int y) {
	if (key == 'f' || key == 'b') {
		flag_rotate_arm = true;
		flag_rotate_leg = true;
		Rotate_leg_y = 0;
	}
	if (key == 'f')
		offset += 0.1;
	if (key == 'b')
		offset -= 0.1;
	if (key == 'm')
		flag_rotate_arm = true;
	if (key == 'l')
		flag_rotate_leg = true;
	if (key == 'i')
		flag_jump = true;
	if (key == 'a')
		flag_animat = true;
	if (key == 'q') {
		flag_rotate_arm = true;
		Rotate_leg = 0;
		flag_rotate_leg_y = true;
		offset_y -= 0.1;
	}
	if (key == 'e') {
		flag_rotate_arm = true;
		Rotate_leg = 0;
		flag_rotate_leg_y = true;
		offset_y += 0.1;
	}

	if (Rotate_arm == Rotate_range || Rotate_arm == -Rotate_range)
		arm_offset = -arm_offset;
	if (flag_rotate_arm == true)
		Rotate_arm += arm_offset * 10;

	if (Rotate_leg == Rotate_range || Rotate_leg == -Rotate_range)
		leg_offset = -leg_offset;
	if (flag_rotate_leg == true)
		Rotate_leg += leg_offset * 10;
	if (Rotate_leg_y == Rotate_range/2 || Rotate_leg_y == -Rotate_range/2)
		leg_offset_y = -leg_offset_y;
	if (flag_rotate_leg_y == true)
		Rotate_leg_y += leg_offset_y * 10;

	if (Rotate_arm > Rotate_range || Rotate_arm < -Rotate_range)
		Rotate_arm = 0;
	if (Rotate_leg > Rotate_range || Rotate_leg < -Rotate_range)
		Rotate_leg = 0;

	flag_rotate_arm = false;
	flag_rotate_leg = false;
	std::cout << offset << std::endl;
}
// display : main draw function
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	gluLookAt(3, 3.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0);
	glPushMatrix();

	//ground
	glTranslatef(0, -10.0, -10.0);
	glColor3f(0.0, 1.0, 0.0);
	glScalef(10.0, 0.1, 10.0);
	glutSolidCube(size * 15);
	glColor3f(0, 0, 0);
	glScalef(0.001, 0.5, 0.001);
	glTranslatef(500, 0.0, 40);
	glRotatef(90, 0, 1, 0);
	draw_duck();


	// soccer image
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3, 0, 0);
	glColor3f(1, 1, 1);
	glRotatef(angle, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(3, 3, 3);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 0.5, 30, 30);
	glDisable(GL_TEXTURE_2D);


	//
	//draw head
	glPopMatrix();
	glPushMatrix();
	glTranslatef(offset_y, 1.6 + jump_height + animat_dis, -8.0 + offset);
	glColor3f(1.0, 0.3, 0.0);
	glScalef(1.0, 1.0, 1.0);
	glutSolidCube(size);
	glColor3f(0, 0, 0);
	glutWireCube(size);
	//neck
	glTranslatef(0.0, -0.6, 0);
	glColor3f(1.0, 1.0, 0.0);
	glScalef(1.5, 0.6, 1.0);
	glutSolidCube(size*0.4);
	glColor3f(0, 0, 0);
	glutWireCube(size*0.4);

	//draw eye
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.2 + offset_y, 1.8 + jump_height, -7.5 + offset + animat_dis);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1.0, 1.0, 1.0);
	glutSolidSphere(size*0.1, 50, 50);

	//draw eye
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.2 + offset_y, 1.8 + jump_height, -7.5 + offset + animat_dis);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1.0, 1.0, 1.0);
	glutSolidSphere(size*0.1, 50, 50);

	//draw nose
	glPopMatrix();
	glPushMatrix();
	glTranslatef(offset_y, 1.6 + jump_height, -7.5 + offset + animat_dis);
	glColor3f(1.0, .0, .0);
	glScalef(1.0, 1.0, 1.0);
	glutSolidCone(size*0.1, 0.5, 50, 50);

	//draw mouse
	glPopMatrix();
	glPushMatrix();
	glTranslatef(offset_y, 1.4 + jump_height, -7.5 + offset + animat_dis);
	glColor3f(1.0, .0, .0);
	glScalef(1.0, 0.2, 0.2);
	glutSolidCube(size*0.5);

	//draw body
	glPopMatrix();
	glPushMatrix();
	glTranslatef(offset_y, -0.1 + jump_height + animat_dis, -8.0 + offset);
	glColor3f(0.0, 0.0, 1.0);
	glScalef(1.5, 2.0, 1.0);
	glutSolidCube(size);
	glColor3f(0, 0, 0);
	glutWireCube(size);

	glTranslatef(0.0, -0.6, 0);
	glColor3f(0.0, 0.5, 1.0);
	glScalef(1.2, 0.5, 1.0);
	glutSolidCube(size*0.5);
	glColor3f(0, 0, 0);
	glutWireCube(size*0.5);


	//draw left-foot
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.6 + offset_y, -1.3 + jump_height, -8.0 + offset);
	glRotatef(Rotate_leg, 1, 0, 0);
	glRotatef(Rotate_leg_y, 0, 0, 1);
	glColor3f(0.5, 0.5, 0.5);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(size*1.5, 50, 50);

	glTranslatef(0, -8.5, -0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.3, 2.0, 0.4);
	glutSolidCube(size*7.5);
	glColor3f(0, 0, 0);
	glutWireCube(size*7.5);

	//draw right-foot
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.6 + offset_y, -1.3 + jump_height, -8.0 + offset);
	glRotatef(-Rotate_leg, 1, 0, 0);
	glRotatef(-Rotate_leg_y, 0, 0, 1);
	glColor3f(0.5, 0.5, 0.5);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(size*1.5, 50, 50);

	glTranslatef(0, -8.5, -0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.3, 2.0, 0.4);
	glutSolidCube(size*7.5);
	glColor3f(0, 0, 0);
	glutWireCube(size*7.5);


	//draw right-hand
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.8 + offset_y + animat_dis, 0.5 + jump_height, -8.0 + offset);
	glRotatef(Rotate_arm, 1, 0, 0);
	glRotated(animat_rotate, 0, 0, 1);
	glColor3f(0.5, 0.5, 0.5);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(size, 50, 50);

	glTranslatef(0, -6.0, -0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.3);
	glutSolidCube(size*8.0);
	glColor3f(0, 0, 0);
	glutWireCube(size*8.0);


	//draw left-hand
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.8 + offset_y - animat_dis, 0.5 + jump_height, -8.0 + offset);
	glRotatef(-Rotate_arm, 1, 0, 0);
	glRotated(-animat_rotate, 0, 0, 1);
	glColor3f(0.5, 0.5, 0.5);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(size, 50, 50);

	glTranslatef(0, -6.0, -0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.3);
	glutSolidCube(size*8.0);
	glColor3f(0, 0, 0);
	glutWireCube(size*8.0);





	glFlush();

	glutSwapBuffers();
}

void  reshape(int  w, int  h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 80.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0);
}
int  main(int  argc, char ** argv)
{
	loader.load("duck.smf");	// (3) Load model file contant
	makeCheckImage();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	SetLightSource();
	SetMaterial();
	glutMainLoop();
	return  0;
}
