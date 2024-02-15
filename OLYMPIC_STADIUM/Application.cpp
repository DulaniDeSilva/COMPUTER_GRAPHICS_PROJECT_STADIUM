#include <GL/glut.h>
#include <math.h>
#include <SOIL2.h>
#include <stdio.h>

//variables to move the camera
GLfloat camX = 0.0;
GLfloat camY = 0.0;
GLfloat camZ = 0.0;


//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;


////variables to move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

GLfloat doorRotate = 0.0f;

// lift door
GLfloat openLeft = 0.0f;
GLfloat openRight = 0.0f;
GLfloat goUp = 0.0f;



int animateForthFlag = 0.0;
int animateFifthFlag = 0.0;


constexpr float PI = 3.14159265358979323846;

//################ animate base
GLfloat moveBase;
GLfloat moveSecond;
GLfloat moveThird;
int animateBaseFlag = 0.0;
int animateSecondFlag = 0.0;
int moveLampStand = 0.0;
int animateThirdFlag = 0.0;

unsigned char* trackImage;
GLuint trackTexture;
int trackWidth, trackHeight;

unsigned char* chairImage;
GLuint chairTexture;
int chairWidth, chairHeight;

unsigned char* grassImage;
GLuint grassTexture;
int grassWidth, grassHeight;

unsigned char* wallImage;
GLuint wallTexture;
int wallWidth, wallHeight;

unsigned char* symbolImage;
GLuint symbolTexture;
int symbolWidth, symbolHeight;

GLuint textureIDLand;
GLuint topTextureID;
GLuint towerTexture;


//drawing a cylinder: not solid 
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

void drawCube() {

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

}


//drawing solid cylinder
void drawSolidCylinder(float radius, float height) {
	float angle = 0.0;
	float stepSize = 0.1;

	glBegin(GL_QUAD_STRIP);
	while (angle < 2 * PI) {
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		glVertex3f(x, 0, z);
		glVertex3f(x, height, z);
		angle += stepSize;
	}
	glVertex3f(radius, 0, 0);
	glVertex3f(radius, height, 0);
	glEnd();


}

//drawing circle
void drawCircle(double radius, int n) {
	double angle = 0.0;
	glBegin(GL_POLYGON);
	for (int c = 0; c <= n; c++) {
		double x = radius * cos(angle);
		double y = radius * sin(angle);
		glVertex2f(x, y);
		angle = angle + ((2 * PI) / n);
	}
	glEnd();
}

//drawing axis
void drawAxes() {
	glBegin(GL_LINES);
	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1000, 0, 0);
	glVertex3f(1000, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -1000, 0);
	glVertex3f(0,1000, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -1000);
	glVertex3f(0, 0, 1000);

	glEnd();
}

//horizontal grid
void drawGrid() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -1000; line <= 1000; line += step) {
		glVertex3f(line, -0.4, 1000);
		glVertex3f(line, -0.4, -1000);

		glVertex3f(1000, -0.4, line);
		glVertex3f(-1000, -0.4, line);
	}
	glEnd();
}

//LAND

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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2000, -0.8, -2000);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-2000, -0.8, 2000);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2000, -0.8, 2000);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2000, -0.8, -2000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawGrassLand() {
	glColor3f(0.10, 0.30, 0.10);
	//glColor3f(0.05, 0.57, 0.46);
	glBegin(GL_POLYGON);
	glVertex3f(-1500.0, -0.8,-1500.0);
	glVertex3f(-1500.0, -0.8,1500.0);
	glVertex3f(1500.0, -0.8, 1500.0);
	glVertex3f(1500.0, -0.8, -1500.0);
	glEnd();
}



//#############################################  STADIUM FLOOR TRACK #############################################
void loadTextureDataFromTrackImage() {
	trackImage = SOIL_load_image("olympictrack.jpg", &trackWidth, &trackHeight, 0, SOIL_LOAD_RGB);

	if (trackImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadTrackTexture() {

	loadTextureDataFromTrackImage(); // Load pattern into image data array
	glGenTextures(1, &trackTexture);
	glBindTexture(GL_TEXTURE_2D, trackTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, trackWidth, trackHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, trackImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(trackImage);
}

void loadCircleLand() {
	textureIDLand = SOIL_load_OGL_texture(
		"flor2.jpeg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);
	if (!textureIDLand) {
		printf("Texture loading failed: %s\n", SOIL_last_result());
	}
}


void stadiumGround() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	GLUquadric* qaud = gluNewQuadric();
	gluQuadricTexture(qaud, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, textureIDLand);
	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	gluDisk(qaud, 10.0, 1000, 20, 20);
	gluDeleteQuadric(qaud);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glColor3f(0.01, 0.49, 0.18);
	glBegin(GL_POLYGON);
	glVertex3f(475, 0.2, -475);
	glVertex3f(-475, 0.2, -475);
	glVertex3f(-475, 0.2, 475);
	glVertex3f(475, 0.2, 475);
	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, trackTexture);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(300, 0.5, -300);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-300, 0.5, -300);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-300, 0.5, 300);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(300, 0.5, 300);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}


//############################################   STADIUM FRONT WALLS AND DOOR #################################

void loadTextureDataFromWallImage() {
	wallImage = SOIL_load_image("wall11.jpg", &wallWidth, &wallHeight, 0, SOIL_LOAD_RGB);

	if (wallImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadWallTexture() {

	loadTextureDataFromWallImage(); // Load pattern into image data array
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, wallWidth, wallHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, wallImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(wallImage);
}

void drawDoor() {
	//Left dooor
	glPushMatrix();
	//glColor3f(0.76, 0.50, 0.32);
	glTranslated(-25.0, 30.0, 0.0);
	glRotatef(doorRotate, 0.0, 1.0, 0.0);
	glTranslated(25.0, 0.0, 0.0);
	glScaled(50.0, 60.0, 5.0);
	glutSolidCube(1.0);
	glPopMatrix();

	//right door
	glPushMatrix();
	//glColor3f(0.76, 0.50, 0.32);
	glTranslated(25.0, 30.0, 0.0);
	glRotatef(-doorRotate, 0.0, 1.0, 0.0);
	glTranslated(-25.0, 0.0, 0.0);
	glScaled(50.0, 60.0, 5.0);
	glutSolidCube(1.0);
	glPopMatrix();

}

void frontWall() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslatef(25.0, 0.0, 0.0);
	glScalef(450.0, 120.0, 5.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-475.0, 0.0, 0.0);
	glScalef(450.0, 120.0, 5.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25.0, 60.0, 0.0);
	glScalef(50.0, 60.0, 5.0);
	drawCube();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	

	drawDoor();

}

// ############################################ STADIUM LOWER BUILDING ##########################################
void drawWalls() {
	glPushMatrix();
	glTranslatef(0.0, 0.0, -950.0);
	glRotatef(180.0, 0.0, 1.0, 0.0);
	frontWall();
	glPopMatrix();

	frontWall();

	glPushMatrix();
	glTranslatef(-475.0, 0.0, -475.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	frontWall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(475.0, 0.0, -475.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	frontWall();
	glPopMatrix();

}


//############################################# Top Structure#########################

//curve line structures
void drawCurveWalls() {
	glColor3f(0.0, 0.0, 0.0);
	//GLfloat radius = 10;
	//GLfloat height = 0.1;

	for (GLfloat i = 400; i <= 500; i += 0.5) {
		drawCylinder(i, 0.2);
		glTranslatef(0.0, 0.5, 0.0);
	}

}

void wiredHalfSphere() {
	glEnable(GL_CLIP_PLANE0);
	double planeEq[4] = { 0.0, 1.0, 0.0,0.0 };
	glClipPlane(GL_CLIP_PLANE0, planeEq);
	glPushMatrix();
	glutWireSphere(480.0, 30.0, 30.0);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE0);
}

void drawTop() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.0, 120.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glColor3f(1.0, 1.0, 1.0);
	gluCylinder(quad, 400.0, 400.0, 75, 100,100);
	gluDeleteQuadric(quad);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 100.0, 0.0);
	//drawCurveWalls();
	glPopMatrix();




	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.0, 200.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadric* quad2 = gluNewQuadric();
	gluQuadricTexture(quad2, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glColor3f(1.0, 1.0, 1.0);
	gluCylinder(quad2, 480.0, 480.0, 75, 100, 100);
	gluDeleteQuadric(quad2);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 195.0, 0.0);
	//drawCurveWalls();
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.0, 300.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadric* quad3 = gluNewQuadric();
	gluQuadricTexture(quad3, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glColor3f(1.0, 1.0, 1.0);
	gluCylinder(quad3, 480.0, 480.0, 75, 100, 100);
	gluDeleteQuadric(quad3);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 380.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	//glutSolidTorus(50.0, 480.0, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 300.0, 0.0);
	wiredHalfSphere();
	glPopMatrix();




}


void drawCircleTraingle(double radius, int n) {
	double angle = 0.0;
	glBegin(GL_POINTS);
	for (int c = 0; c <= n; c++) {
		angle = angle + (2 * PI) / n;
		double x = radius * cos(angle);
		double z = radius * sin(angle);
		glVertex3f(x,0, z);

		glPushMatrix();
		glTranslatef(x, 0.0, z);
		glutSolidSphere(10.0, 32, 32);
		glPopMatrix();
	}
	glEnd();
	
}

//################################## LARGE TOWERS

void loadTowerTexture() {
	towerTexture = SOIL_load_OGL_texture(
		"pole.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);

	if (!towerTexture) {
		printf("Texture loading failed: %s\n", SOIL_last_result());
	}
}

void drawTorus() {

	glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	glTranslated(0.0, 100.0, 0.0);
	glutSolidSphere(6, 80, 80);
	glPopMatrix();

	//bottom torus
	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	//glTranslated(0.0, 0.0, 0.85);
	glColor3f(0.6, 0.6, 0.6);
	glutSolidTorus(2, 2, 50, 50);
	glPopMatrix();

	//top torus
	glPushMatrix();
	glTranslated(0.0, 100, 0.0);
	glRotated(-90, 1.0, 0.0, 0.0);
	glColor3f(0.6, 0.6, 0.6);
	glutSolidTorus(2, 2, 50, 50);
	//glutSolidTorus(0.15, 1.73, 10, 50);
	glPopMatrix();


	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, towerTexture);
	gluCylinder(quad, 2, 2, 100.0, 20, 20);
	gluDeleteQuadric(quad);
	glPopMatrix();

}

void drawCircularFlagSet() {
	const int numChairs = 6; //number of single
	const int numSets = 6;    // Number of chair sets
	const float circleRadius = 950.0; // Radius of the circle

	for (int setIndex = 0; setIndex < numSets; setIndex++) {
		float angleIncrement = 360.0 / numChairs; // Angle between chairs

		for (int chairIndex = 0; chairIndex < numChairs; chairIndex++) {
			float angle = chairIndex * angleIncrement + setIndex * (360.0 / numSets);

			float x = circleRadius * cos(angle * (3.14159265 / 180.0));
			float z = circleRadius * sin(angle * (3.14159265 / 180.0));

			glPushMatrix();
			glTranslatef(x, 0.0, z);
			glScalef(10.0, 10.0, 10.0);
			drawTorus();
			glPopMatrix();
		}
	}
}


//############################################# Stadium chairs

//structure to cover the stdium first level
void drawSeatArea() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wallTexture);

	glPushMatrix();
	glTranslatef(275, 120, -475);
	glScalef(200, 5, 950);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-475, 120, -475);
	glScalef(200, 5, 950);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-475, 120, -475);
	glScalef(950, 5, 200);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-475, 120, 275);
	glScalef(950, 5, 200);
	drawCube();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

void loadTextureDataFromImage() {
	chairImage = SOIL_load_image("red_texture.jpg", &chairWidth, &chairHeight, 0, SOIL_LOAD_RGB);

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, chairTexture);
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
	glDisable(GL_TEXTURE_2D);

}

void drawChair() {
	
	glPushMatrix();
	glScalef(4.0, 0.8, 4.0);
	chair();
	glPopMatrix();

	glPushMatrix();
	glScalef(4.0, 6.0, 1.0);
	chair();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(3.5, 2.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawCylinder(0.6, 9.0);
	glPopMatrix();

}

void drawChairSet() {
	GLfloat z = 0;
	GLfloat y = 5;
	for (int i = 0; i <= 9; i++) {
		GLfloat x = -250;
		for (int j = 0; j <= 90; j++) {
			glPushMatrix();
			glTranslatef(x, y, z);
			drawChair();
			x += 5;
			glPopMatrix();
		}
		z += 8;
		y -= 5;
	}
}

void stadiumChairs() {

	glPushMatrix();
	glTranslatef(0.0, 40, 325.0);
	glRotatef(180.0, 0.0, 1.0, 0.0);
	drawChairSet();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, 40.0, -325.0);
	drawChairSet();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-325.0, 40.0, 0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawChairSet();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(325.0, 40.0, 0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawChairSet();
	glPopMatrix();



}

void totalSeatingArea() {
	drawSeatArea();

	glPushMatrix();
	glTranslatef(0.0, 10.0, 0.0);
	stadiumChairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 120.0, 0.0);
	for (int i = 280; i <= 450; i++) {
		glColor3f(0.78, 0.78, 0.8);
		drawSolidCylinder(i, 2.0);
	}
	glPopMatrix();
	

	glPushMatrix();
	glTranslatef(0.0, 130.0, 0.0);
	stadiumChairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 220.0, 0.0);
	glColor3f(0.49, 0.38, 0.38);
	for (int i = 280; i <= 450; i++) {
		glColor3f(0.78, 0.78, 0.8);
		drawSolidCylinder(i, 2.0);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 230.0, 0.0);
	stadiumChairs();
	glPopMatrix();
}


//#############################################  LAMP STRUCTURE ################################################
void animateLampBase(int x) {
	if (animateBaseFlag && moveBase <= 10.0) {
		moveBase += 0.1;
		glutPostRedisplay();
		glutTimerFunc(1000 / 10, animateLampBase, 1.0);
	}

}

void animateLampSecond(int x) {
	if (animateSecondFlag && moveSecond <= 15.0) {
		moveSecond += 0.2;
		glutPostRedisplay();
		glutTimerFunc(1000 / 10, animateLampSecond, 1.0);
	}
}

void animateLampThird(int x) {
	if (animateThirdFlag && moveThird <= 40.0) {
		moveThird += 1;
		glutPostRedisplay();
		glutTimerFunc(1000 / 10, animateLampThird, 1.0);
	}
}

void lampbase() {
	//Large base
	glPushMatrix();
	glColor3f(1, 0.733, 0.36);
	glTranslatef(0.0, -10.0, 0.0);
	drawSolidCylinder(50, 10.0);
	glPopMatrix();
}

void secondbase() {
	//small base
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, -5.0, 0.0);
	drawSolidCylinder(30, 5.0);
	glPopMatrix();

}

void halfSphere() {
	glEnable(GL_CLIP_PLANE0);
	double planeEq[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, planeEq);
	glPushMatrix();
	glutSolidSphere(20.0, 100, 100);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);
}

void lampStand() {
	glPushMatrix();
	glTranslatef(0.0, -30.0, 0.0);

	glColor3f(1.0, 0.733, 0.36);
	drawSolidCylinder(10, 15.0);

	glPushMatrix();
	glColor3f(1.0, 0.733, 0.36);
	glTranslatef(0.0, 30.0, 0.0);
	glRotatef(180.0, 0.0, 0.0, 1.0);
	halfSphere();
	glPopMatrix();
	glPopMatrix();

}

void drawLamp() {
	glPushMatrix();
	glTranslatef(0.0, moveBase, 0.0);
	lampbase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, moveSecond, 0.0);
	secondbase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, moveThird, 0.0);
	lampStand();
	glPopMatrix();



}

//##################################### Elevator structure

void drawElivator() {

	glPushMatrix();
	glTranslatef(openLeft, 0.0, 0.0);
	glPushMatrix();
	glColor3f(0.0, 1.0, 1.0);
	glScalef(40.0, 160.0, 5.0);
	drawCube();
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(openRight, 0.0, 0.0);
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(40.0, 0.0, 0.0);
	glScalef(40.0, 160.0, 5.0);
	drawCube();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -80.0);
	glScalef(80.0, 160.0, 5.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -80.0);
	glScalef(5.0, 160.0, 80.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(80.0, 0.0, -80.0);
	glScalef(5.0, 160.0, 80.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 160.0, -80.0);
	glScalef(80.0, 5.0, 80.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -80.0);
	glScalef(80.0, 5.0, 80.0);
	drawCube();
	glPopMatrix();

}

void fourElivators() {

	glPushMatrix();
	glTranslatef(0.0, goUp, 0.0);
	glPushMatrix();
	glTranslatef(250, 0.0, -300.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	drawElivator();
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, goUp, 0.0);
	glPushMatrix();
	glTranslatef(-300, 0.0, -250.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	drawElivator();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, goUp, 0.0);
	glPushMatrix();
	glTranslatef(-200, 0.0, 250.0);
	glRotatef(135.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	drawElivator();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, goUp, 0.0);
	glPushMatrix();
	glTranslatef(300, 0.0, 250.0);
	glRotatef(225.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	drawElivator();
	glPopMatrix();
	glPopMatrix();
}

//################################# OLYMPIC SYMBOL
void loadTextureDataFromSymbolImage() {
	symbolImage = SOIL_load_image("wall11.jpg", &symbolWidth, &symbolHeight, 0, SOIL_LOAD_RGB);

	if (wallImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadSymbolTexture() {

	loadTextureDataFromSymbolImage(); // Load pattern into image data array
	glGenTextures(1, &symbolTexture);
	glBindTexture(GL_TEXTURE_2D, symbolTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, symbolWidth, symbolHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, symbolImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(symbolImage);
}

void drawOlympicSymbol() {
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(-30.0, 0.0, 0.0);
	glutSolidTorus(2.0, 12.0, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glutSolidTorus(2.0, 12.0, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(30.0, 0.0, 0.0);
	glutSolidTorus(2.0, 12.0, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(-16.0, -16.0, 0.0);
	glutSolidTorus(2.0, 12.0, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(16.0, -16.0, 0.0);
	glutSolidTorus(2.0, 12.0, 30, 30);
	glPopMatrix();
}

void symbolHolder() {

	
	glPushMatrix();
	glTranslatef(100.0, 60.0, 50.0);
	drawOlympicSymbol();
	glPopMatrix();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, symbolTexture);
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(50.0, 20.0, 15.0);
	glScalef(100.0, 10.0, 50.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glScalef(200.0, 20.0, 80.0);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//################################# Shooting structure
void robot(){
	//head
	glPushMatrix();
	glTranslatef(0.0, 11.0, 0.0);
	glutSolidSphere(2.0, 30, 30);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0, 5.0, 0.0);
	drawSolidCylinder(2.0, 4.0);
	glPopMatrix();


	//left arm 1
	glPushMatrix();
	glTranslatef(-2.0, 9.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//rigth arm 1
	glPushMatrix();
	glTranslatef(3.5, 9.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//left arm 2
	glPushMatrix();
	glTranslatef(-4.0, 6.0, 0.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//rigth arm 2
	glPushMatrix();
	glTranslatef(3.5,6.0, 0.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();


	//left leg 1
	glPushMatrix();
	glTranslatef(-1.0, 3.0, 0.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//right leg 1
	glPushMatrix();
	glTranslatef(1.0, 3.0, 0.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//left leg 2
	glPushMatrix();
	glTranslatef(-1.0, 0.0, 0.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//right leg 2
	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	drawSolidCylinder(0.8, 2.0);
	glPopMatrix();

	//ball
	glPushMatrix();
	glTranslatef(5.0, 2.0, 0.0);
	glutSolidSphere(2.0, 30, 30);
	glPopMatrix();
}





//##################################### DRAWING THE STADIUM #######################


void OlympicStadium() {
	glPushMatrix();

	
	drawGrassLand();
	

	glPushMatrix();
	glTranslatef(0.0, 0.0, 475);
	drawWalls();
	glPopMatrix();

	
	stadiumGround();

	totalSeatingArea();

	drawLamp();

	drawTop();

	//wall attached symbol
	glPushMatrix();
	glTranslatef(-300.0, 60.0, 480.0);
	drawOlympicSymbol();
	glPopMatrix();

	//front olympic symbol
	glPushMatrix();
	glTranslatef(200.0, 5.0, 700.0);
	glRotatef(-30.0, 0.0, 1.0, 0.0);
	symbolHolder();
	glPopMatrix();

	drawCircularFlagSet();

	glPushMatrix();
	glTranslatef(-50.0, 0.0, -50.0);
	glScalef(4.0, 4.0, 4.0);
	robot();
	glPopMatrix();


	fourElivators();

	glPopMatrix();
}

void display(void) {
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(0.0 + camX, 0.0 + camY, 150.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(sceTX, sceTY, sceTZ);
	glRotatef(sceRY, 0.0, 1.0, 0.0);
	glRotatef(objRY, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawAxes();


	glPushMatrix();
	OlympicStadium();
	glPopMatrix();

	




	glPopMatrix();
	glutSwapBuffers();
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);

}


void init(void) {
	//glClearColor(0.0, 0.74, 1.0, 1.0);
	glClearColor(0.1, 0.1, 0.33, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);
	loadTrackTexture();
	loadChairTextures();
	loadGrassTexture();
	loadCircleLand();
	loadWallTexture();
	loadSymbolTexture();

}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, aspect_ratio, 1.0, 10000.0);
}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 50;
	if (key == GLUT_KEY_DOWN)
		camY -= 50;
	if (key == GLUT_KEY_LEFT)
		//camZ+= 0.5;
		sceTX -= 5;
	if (key == GLUT_KEY_RIGHT)
		//camZ -= 0.5;
		sceTX += 5;
	glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'z')
		sceTZ -= 50;
	if (key == 'Z')
		sceTZ += 50;
	if (key == 'x')
		sceTX -= 5;
	if (key == 'X')
		sceTX += 5;
	if (key == 'y')
		sceTY += 1;
	if (key == 'Y')
		sceTY -= 5;
	if (key == 'l')
		objRY -= 3;
	if (key == 'r')
		objRY += 3;
	if (key == 'm')
		sceRY += 3;
	if (key == 'n')
		sceRY -= 3;
	if (key == '!')
		glEnable(GL_LIGHT0);
	if (key == '@')
		glDisable(GL_LIGHT0);
	if (key == 'c')
		camX -= 5;
	if (key == 'C')
		camX += 5;
	if (key == 'd')
		if (doorRotate < 120)
			doorRotate += 120;
	if (key == 'D')
		if (doorRotate > 0)
			doorRotate -= 120;
	if (key == '1')
		animateBaseFlag = 1;
		glutTimerFunc(1000 / 60, animateLampBase, 1.0);
	if (key == '2')
		animateSecondFlag = 1;
		glutTimerFunc(1000 / 60, animateLampSecond, 1.0);
	if (key == '3')
		animateThirdFlag = 1;
		glutTimerFunc(1000 / 60, animateLampThird, 1.0);
	if (key == '4') {
		if (openLeft != -40) {
			openLeft -= 1;
			openRight += 1;
		}
	}
	if (key == '5') {
		if (openLeft != 0) {
			openLeft += 1;
			openRight -= 1;
		}
	}
	if (key == '6') {
		if (goUp != 300) {
			goUp += 10;
		}
	}
	if (key == '7') {
		if (goUp != 0) {
			goUp -= 10;
		}
	}



			
		

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