#include <GL/glut.h>
#include <math.h>
#include <SOIL2.h>
#include <stdio.h>

//variables to move the camera
GLfloat camX = 0.0;
GLfloat camY = 0.0;
GLfloat camZ = 0.0;

//variables to move the scene
GLfloat moveX = 0.0;
GLfloat moveY = 0.0;
GLfloat moveZ = 0.0;

//variables to move the object
GLfloat objX = 0.0;
GLfloat objY = 0.0;
GLfloat objZ = 0.0;

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

constexpr float PI = 3.14159265358979323846;

GLfloat vertices[][3] = { {0, 0, 0}	, {10, 0, 0}, {10, 0, 15}, {0, 0, 15}, 
							{0, 5, 0}, {10, 5, 0}, {10, 5, 15}, {0, 5, 15} };


//texture
unsigned char* chairImage;
unsigned char* grassImage;
unsigned char* pyramidimage2;
unsigned char* pyramidimage3;

GLuint chairTexture;
GLuint grassTexture;


int chairWidth, chairHeight, grassWidth, grassHeight;


GLfloat controlPoints[3][3] = { {0, 1,0}, {1, 1, 0}, {0.5,1.3,0.0} };

//drawinf curve
void drawCurve() {
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 0.0, 0.0);
	for (float t = 0.0; t <= 1.0; t += 0.01) {
		GLfloat x = (1 - t) * (1 - t) * controlPoints[0][0] + 2 * (1 - t) * t * controlPoints[1][0] + t * t * controlPoints[2][0];
		GLfloat y = (1 - t) * (1 - t) * controlPoints[0][1] + 2 * (1 - t) * t * controlPoints[1][1] + t * t * controlPoints[2][1];
		glVertex2f(x, y);
	}
	glEnd();

	glFlush();
}




//drawing axis
void drawAxes() {
	glBegin(GL_LINES);
	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-200, 0, 0);
	glVertex3f(200, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -200, 0);
	glVertex3f(0, 200, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -200);
	glVertex3f(0, 0, 200);

	glEnd();
}


//horizontal grid
void drawGrid() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -20; line <= 20; line += step) {
		glVertex3f(line, -0.4, 20);
		glVertex3f(line, -0.4, -20);

		glVertex3f(20, -0.4, line);
		glVertex3f(-20, -0.4, line);
	}
	glEnd();
}

//vertical grid
void drawGridV() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -20; line <= 20; line += step) {
		glVertex3f(0.0, line, 20);
		glVertex3f(0.0, line, -20);

		glVertex3f(20, line, 0.0);
		glVertex3f(-20, line, 0.0);
	}
	glEnd();
}

//drawing a cylinder
void drawCylinder(float radius, float height) {
	float x = 0;
	float y = 0;
	float z = 0;

	float angle = 0.0;
	float stepSize = 0.1;

	glBegin(GL_QUAD_STRIP);
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		z = radius * sin(angle);
		glVertex3f(x, y, z);
		glVertex3f(x, y + height, z);
		angle += stepSize;
	}
	glVertex3f(radius, 0, 0);
	glVertex3f(radius, height, 0);
	glEnd();
}

//drawing a quad/polygon (4)
void surface4(int v1, int v2, int v3, int v4) {
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[v1]);
	glVertex3fv(vertices[v2]);
	glVertex3fv(vertices[v3]);
	glVertex3fv(vertices[v4]);
	glEnd();
}

//drawing atriangle
void surface3(int v1, int v2, int v3) {
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[v1]);
	glVertex3fv(vertices[v2]);
	glVertex3fv(vertices[v3]);
	glEnd();
}

//staircase
void singleStaircase() {

	//main background
	glPushMatrix();
	
	//bottom
	surface4(0, 3, 2, 1);
	//top
	surface4(4, 7, 6, 5);
	//front
	surface4(7, 3, 2, 6);
	//behind
	surface4(1, 5, 4, 0);
	//left
	surface4(6, 2, 1, 5);
	//right
	surface4(0, 4, 7, 3);
	glPopMatrix();
}





//loading grass
void loadTextureDataFromGrassImage() {
	grassImage = SOIL_load_image("grass.jpeg", &grassWidth, &grassHeight, 0, SOIL_LOAD_RGB);

	if (grassImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadGrassTexture() {

	loadTextureDataFromGrassImage(); // Load pattern into image data array
	glGenTextures(1, &grassTexture);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, grassWidth, grassHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, grassImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(grassImage);
}


void drawLand() {
	//GLfloat step = 1.0f;
	GLint line;
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100, 0.0, -100);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-100, 0.0, 100);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100, 0.0, 100);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100, 0.0, -100);

	glEnd();

}



//################################################  CHAIRS    #################################
//single chair
void loadTextureDataFromImage() {
	chairImage = SOIL_load_image("Textures/Chair/chair_yellow.png", &chairWidth, &chairHeight, 0, SOIL_LOAD_RGB);

	if (chairImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadChairTextures() {

	loadTextureDataFromImage(); // Load pattern into image data array
	glGenTextures(1, &chairTexture);
	glBindTexture(GL_TEXTURE_2D, chairTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, chairWidth, chairHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, chairImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(chairImage);
}

void chair() {

	glBegin(GL_QUADS);
	//lowert
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);


	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);

	glEnd();


}

void drawChair() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,chairTexture);
	glScalef(2.0, 0.4, 2.0);
	chair();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, chairTexture);
	glScalef(2.0, 3.0, 0.5);
	chair();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.5, 2.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawCylinder(0.3, 4.5);
	glPopMatrix();

}

void singleChairSet() {
	const int numChairs = 6;
	const int numRows = 3;
	const float chairSpacing = 4;
	
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numChairs; col++) {
			glPushMatrix();
			glTranslatef(col * chairSpacing, 0.0, row*chairSpacing);
			drawChair();
			glPopMatrix();
		}
	}
}



//################################  OLYMPIC LOGO ############################################


void drawRing(float outerRadius, float innerRadius, int numSegments) {
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i <= numSegments; i++) {
		float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
		float xOuter = outerRadius * cos(theta);
		float yOuter = outerRadius * sin(theta);

		float xInner = innerRadius * cos(theta);
		float yInner = innerRadius * sin(theta);

		glVertex2f(xOuter, yOuter);
		glVertex2f(xInner, yInner);
	}

	glEnd();
}

void drawSymbol() {
	const float chairSpacing = 5;
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glTranslatef(i*chairSpacing, 0.0, 0.0);
		drawRing(4.0, 3.0, 32);
		glPopMatrix();
	}

	glPushMatrix();
	
	for (int i = 0; i < 2; i++) {
		glTranslatef(3.0, -4, 0.0);
		glPushMatrix();
		glTranslatef(i * chairSpacing, 0.0, 0.0);
		drawRing(4.0, 3.0, 32);
		glPopMatrix();
	}
	glPopMatrix();

	
}









//single item inside the stadium
void singleObject() {
	glColor3f(0.6, 0.6, 0.6);
	singleStaircase();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 5.0, 10.0);
	singleStaircase();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(0.0, 10.0, 20.0);
	singleStaircase();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(4.0, 6.0, 5.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(3.0, 3.0, 3.0);
	chair();
	glPopMatrix();
}



void init(void) {
	glClearColor(0.0, 0.74, 1.0, 1.0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	loadChairTextures();
	loadGrassTexture();



}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(0.0 + camX, 0.0 + camY, 5.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(moveX, moveY, moveZ);
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	
	//drawGrid();
	//drawAxes();

	//stair cases
	//singleObject();

	//singleObject();
	//drawGridV();
	//drawCurve();
	//drawCylinder(10.0, 4.0);

	drawLand();

	glPushMatrix();
	drawSymbol();
	//CHAIRS
	//singleChairSet();
	glPopMatrix();




	glPopMatrix();
	glutSwapBuffers();

}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);
}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 0.5;
	if (key == GLUT_KEY_DOWN)
		camY -= 0.5;
	if (key == GLUT_KEY_LEFT)
		camZ += 0.5;
	if (key == GLUT_KEY_RIGHT)
		camZ -= 0.5;
	glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'z')
		moveZ++;
	if (key == 'Z')
		moveZ--;
	if (key == 'x')
		moveX++;
	if (key == 'X')
		moveX--;
	if (key == 'y')
		moveY++;
	if (key == 'Y')
		moveY--;
	if (key == 'l')
		rotY -= 5.0;
	if (key == 'r')
		rotY += 5.0;
	if (key == '!')
		glEnable(GL_LIGHT0);
	if (key == '@')
		glDisable(GL_LIGHT0);

	glutPostRedisplay();

}




int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Olympic Stadium");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}