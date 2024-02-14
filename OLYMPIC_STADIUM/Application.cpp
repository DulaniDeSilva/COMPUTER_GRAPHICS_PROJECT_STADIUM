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

constexpr float PI = 3.14159265358979323846;

GLfloat vertices[][3] = { {0, 0, 0}	, {10, 0, 0}, {10, 0, 15}, {0, 0, 15}, 
							{0, 5, 0}, {10, 5, 0}, {10, 5, 15}, {0, 5, 15} };


//texture
unsigned char* chairImage;
unsigned char* grassImage;
unsigned char* blueMetalImage;
unsigned char* silverMetal;
unsigned char* trackImage;
unsigned char* wallChairImage;
unsigned char* wallImage;
unsigned char* mainFloorImage;
unsigned char* stairImage;


GLuint chairTexture;
GLuint grassTexture;
GLuint blueMetalTexture;
GLuint silverMetalTexture;
GLuint trackTexture;
GLuint wallChairTexture;
GLuint wallTexture;
GLuint mainFloorTexture;
GLuint stairTexture;
GLuint towerTexture;



int chairWidth, chairHeight, grassWidth, grassHeight, blueMetalWidth, blueMetalHeight, silverMetalWidht, silverMetalHeight, trackWidth, trackHeight, wallChairWidth,
wallChairHeight, wallHeight, wallWidth, mainFloorWidth, mainFloorHeight, stairHeight, stairWidth;


GLfloat controlPoints[3][3] = { {0, 1,0}, {1, 1, 0}, {0.5,1.3,0.0} };

void setLighting() {
	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	//Set lighting intensity and color
	// Set lighting intensity and color for sunlight-like effect
	GLfloat qaAmbientLight[] = { 0.7, 0.7, 0.6, 1.0 };  
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.7, 1.0 };  
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; 

	// Set lighting intensity and color
	GLfloat qaAmbientLight0[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat qaDiffuseLight0[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition0[] = { -500.0, 20.0, -200.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight);
	GLfloat qaLightPosition1[] = { 200.0, 80.0,80.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition1);

}







//draw curve
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

//drawing solid cylinder
void drawSolidCylinder(	float radius, float height) {
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
	glEnd();

	//top circle
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, height, 0);
	angle = 0.0;
	while (angle < 2 * PI) {
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		glVertex3f(x, height, z);
		angle += stepSize;
	}
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

void Draw3DEllipse(float cx, float cy, float cz, float rx, float ry, float rz, int num_segments) {
	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = 1;
	float y = 0;

	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++) {
		glVertex3f(x * rx + cx, y * ry + cy, cz);
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}

//############################################## SINGLE STAIR CASE #################################################
//single stair case
void loadTextureDataFromStairImage() {
	stairImage = SOIL_load_image("tile1.jpg", &stairWidth, &stairHeight, 0, SOIL_LOAD_RGB);

	if (stairImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadStairTexture() {

	loadTextureDataFromStairImage(); 
	glGenTextures(1, &stairTexture);
	glBindTexture(GL_TEXTURE_2D, stairTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, stairWidth, stairHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, stairImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(stairImage);
}

void singleStaircase() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, stairTexture);
	//bottom
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10, 0, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10, 0, 0);
	//surface4(0, 3, 2, 1);
	
	//top
	//surface4(4, 7, 6, 5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 5, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 5, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10, 5, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10, 5, 0);

	//front
	//surface4(7, 3, 2, 6);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 5, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10, 0, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10, 5, 15);

	//behind
	//surface4(1, 5, 4, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10, 5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 5, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, 0);

	//left
	//surface4(6, 2, 1, 5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10, 5, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10, 0, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10, 5, 0);

	//right
	//surface4(0, 4, 7, 3);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 5, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, 15);

	glEnd();
	glPopMatrix();
}

//###################################################  LAND  ###################################

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
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-500, 0.0, -500);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-500, 0.0, 500);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(500, 0.0, 500);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(500, 0.0, -500);
	glEnd();
}


//######################################################  CHAIRS    #################################
//single chair
void loadTextureDataFromImage() {
	chairImage = SOIL_load_image("blue_texture.jpg", &chairWidth, &chairHeight, 0, SOIL_LOAD_RGB);

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
	const int numChairs = 12;
	const int numRows = 3;
	const float chairSpacing = 4;
	
	glPushMatrix();
	glTranslatef(-4.0, 0.0, 4);
	glScalef(2.0, 3.0, 3.0);
	glutSolidCube(4.0);
	glPopMatrix();
	
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numChairs; col++) {
			glPushMatrix();
			glTranslatef(col * chairSpacing, 0.0, row*chairSpacing);
			drawChair();
			glPopMatrix();
		}
	}


}

void drawCircularChairSet() {
	const int numChairs = 6;
	const int numSets = 6; // Number of chair sets
	const float chairSpacing = 4;
	const float circleRadius = 15.0; // Radius of the circle

	for (int setIndex = 0; setIndex < numSets; setIndex++) {
		// Angle between chairs
		float angleIncrement = 360.0 / numChairs; 

		for (int chairIndex = 0; chairIndex < numChairs; chairIndex++) {
			float angle = chairIndex * angleIncrement + setIndex * (360.0 / numSets);

			float x = circleRadius * cos((angle));
			float z = circleRadius * sin((angle));

			glPushMatrix();
			glTranslatef(x, 0.0, z);
			drawChair();
			glPopMatrix();
		}
	}
}

void circleChair() {
	const int numChairs = 150;
	const int numSets = 6; // Number of chair sets
	const float chairSpacing = 4;
	const float circleRadius = 70.0; // Radius of the circle

	for (int setIndex = 0; setIndex < numSets; setIndex++) {
		float angleIncrement = 360.0 / numChairs; // Angle between chairs

		for (int chairIndex = 0; chairIndex < numChairs; chairIndex++) {
			float angle = chairIndex * angleIncrement + setIndex * (360.0 / numSets);

			// Convert polar coordinates to Cartesian coordinates
			float x = circleRadius * cos((angle));
			float z = circleRadius * sin((angle));

			glPushMatrix();
			glTranslatef(x, 0.0, z);
			chair();
			glPopMatrix();
		}
	}
}

//######################################################  OLYMPIC LOGO ############################################

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


//########################################################## OLYMPIC LAMP ####################################
void loadTextureDataFromBlueMetalImage() {
	blueMetalImage = SOIL_load_image("blue_texture.jpg", &blueMetalWidth, &blueMetalHeight, 0, SOIL_LOAD_RGB);

	if (blueMetalImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadBlueMetalTexture() {

	loadTextureDataFromBlueMetalImage(); // Load pattern into image data array
	glGenTextures(1, &blueMetalTexture);
	glBindTexture(GL_TEXTURE_2D, blueMetalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, blueMetalWidth, blueMetalHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, blueMetalImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(blueMetalImage);
}



void displayLamp() {

	GLfloat qaMetalBlue[] = {0.1, 0.2, 0.5, 1.0};
	GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat qaGold[] = { 1, 0.84, 0 };
	GLfloat qaLowAmbinet[] = { 0.2, 0.2, 0.2, 1.0 };
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaLowAmbinet);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGold);
	glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, 20);
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaWhite);
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaGold);


	glPushMatrix();
	glTranslatef(0.0, 7.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, blueMetalTexture);
	glutSolidTorus(1.0, 2.0, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, blueMetalTexture);
	drawCylinder(1.8, 8);
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidTorus(1.0, 2.0,30, 30);
	glPopMatrix();

	glEnable(GL_CLIP_PLANE0);
	double planeEq[4] = { 0.0, 1.0, 0.0, 0.0 };  // Clip in the Y direction (upward)
	glClipPlane(GL_CLIP_PLANE0, planeEq);
	glPushMatrix();
	glTranslatef(0.0, 9.0, 0.0);
	glScalef(1.0, 0.5, 1.0); 
	glRotatef(180, 1.0, 0.0, 0.0);
	glutSolidSphere(5.0, 100, 100);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);

	
	drawSolidCylinder(10, 2);
	
	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	drawSolidCylinder(6.0, 2.0);
	glPopMatrix();
	
}


//########################################################### SHOOTING FIGURE ######################################
void shootFire() {

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

//###########################################################   DRAWING STADIUM #############################

void roof() {
	for (int i = 0; i < 150; i++) {
		drawCylinder(i, 0.3);
	}
}

//curve line structures
void drawWall() {
	GLfloat radius = 10;
	GLfloat height = 0.1;

	for (GLfloat i = 80; i <= 105 ; i += 0.5) {
		drawCylinder(i, 0.2);
		glTranslatef(0.0, 0.2, 0.0);
	}
	
}

//Wall without door: not used
void baseWall() {
	glBegin(GL_QUADS);

	//front
	glVertex3f(0, 0, 0);
	glVertex3f(80, 0, 0);
	glVertex3f(80, 20, 0);
	glVertex3f(0, 20, 0);

	//back
	glVertex3f(80, 0, 5);
	glVertex3f(80, 20,5);
	glVertex3f(0, 20,5);
	glVertex3f(0, 0, 5);

	//right
	glVertex3f(80, 0, 0);
	glVertex3f(80, 0, 5);
	glVertex3f(80, 20, 5);
	glVertex3f(80, 20, 0);

	//left
	glVertex3f(0, 0, 5);
	glVertex3f(0, 20, 5);
	glVertex3f(0, 20, 0);
	glVertex3f(0, 0, 0);

	//top
	glVertex3f(80, 20, 0);
	glVertex3f(80, 20, 5);
	glVertex3f(0,20, 5);
	glVertex3f(0, 20, 0);

	//bottom
	glVertex3f(0, 0, 0);
	glVertex3f(80, 0, 0);
	glVertex3f(80, 0, 5);
	glVertex3f(0, 0, 5);

	glEnd();
}

//stadium wall
void loadTextureDataFromWallImage() {
	wallImage = SOIL_load_image("wall2.jpg", &wallWidth, &wallHeight, 0, SOIL_LOAD_RGB);

	if (wallImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadWall() {
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

//stadium chairs figure to inner wall
void loadTextureDataFromWallChairImage() {
	wallChairImage = SOIL_load_image("stadiumchairss.jpeg", &wallChairWidth, &wallChairHeight, 0, SOIL_LOAD_AUTO );

	if (wallChairImage == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadWallChair() {

	loadTextureDataFromWallChairImage(); // Load pattern into image data array
	glGenTextures(1, &wallChairTexture);
	glBindTexture(GL_TEXTURE_2D, wallChairTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, wallChairWidth, wallChairHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, wallChairImage);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(wallChairImage);
}

//half wall
void frontWallSection() {
	
	//front
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80, 0, 80);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-10, 0, 80);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-10, 20, 80);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-80, 20, 80);
	glEnd();

	//back
	glBindTexture(GL_TEXTURE_2D, wallChairTexture);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80, 0, 75);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-80, 20, 75);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-10, 20, 75);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10, 0, 75);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10, 20, 75);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-10, 20, 80);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-10, 0, 80);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10, 0, 75);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glBegin(GL_QUADS);
	glNormal3f(-1, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80, 0, 80);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-80, 20, 80);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-80, 20, 75);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-80, 0, 75);
	glEnd();

	//top
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80, 20, 75);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-80, 20, 80);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-10, 20, 80);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10, 20, 75);
	glEnd();

	//bottom
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80, 0, 75);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-80, 0, 80);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-10, 0, 80);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10, 0, 75);
	glEnd();

	
}

void frontWall() {

	glPushMatrix();
	frontWallSection();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(90.0, 0.0, 0.0);
	frontWallSection();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.3, 0.5, 1.0);
	glTranslatef(45, 20.0, 0.0);
	frontWallSection();
	glPopMatrix();


}


//#############################################  STADIUM FLOOR TRACK #############################################
void loadTextureDataFromTrackImage() {
	trackImage = SOIL_load_image("track3.jpg", &trackWidth, &trackHeight, 0, SOIL_LOAD_RGB);

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

void floor() {
	glBindTexture(GL_TEXTURE_2D, trackTexture);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80, 1.2	, 80);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(80, 1.2, 80);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(80, 1.2, -80);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-80, 1.2, -80);
	glEnd();
}

//################################################ STADIUM OUTER FLOOR ##################################################


void mainfloor() {
	//glShadeModel(GL_FLAT);

	GLfloat qaCream[] = {0.94, 0.96, 0.96, 1.0};
	GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0};
	GLfloat qaLowAmbient[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaCream);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaCream);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 30.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaCream);
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaCream);
	
	drawSolidCylinder(150, 1);

	/*
	glPushMatrix();
	glTranslatef(0.0, 1.0, 150.0);
	glScalef(10.0, 0.0, 50.0);
	glutSolidCube(4);
	glPopMatrix();*/
	
}


//############################################### STADIUM #######################################################
void base() {
	
	
	mainfloor();

	floor();
	
	frontWall();
	
	glPushMatrix();
	//glTranslatef(0.0, 0.0, 0.0);
	glRotatef(180, 0, 1, 0);
	frontWall();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	frontWall();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	frontWall();
	glPopMatrix();
	
	GLfloat qared[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat qaLowAmbient[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaWhite);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 30.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaWhite);
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaWhite);
	glPushMatrix();
	glTranslatef(0.0, 20.0, 0.0);
	drawCylinder(80, 8);
	glPopMatrix();

	//GLfloat qared[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	//GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat qaLowAmbient[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 30.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaWhite);
	glLightfv(GL_LIGHT1, GL_AMBIENT, black);

	glPushMatrix();
	glTranslatef(0.0, 20.0, 0.0);
	drawWall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 28, 0.0);
	drawCylinder(83, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 30.0, 0.0);
	drawWall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 38, 0.0);
	drawCylinder(85, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 40.0, 0.0);
	drawWall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 48, 0.0);
	drawCylinder(85, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 50.0, 0.0);
	drawWall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 58, 0.0);
	drawCylinder(85, 5);
	glPopMatrix();

}

//################################################   OLYMPIC SYMBOL holder ##############################################
void symbolholder() {
	
	glTranslatef(20, 0, 100);
	glRotatef(45.0, 0.0, -1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);

	glPushMatrix();
	glScalef(4.0, 1.0, 1.0);
	singleStaircase();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(5, 5.0, 0.25);
	glScalef(3.0, 1.0, 0.8);
	singleStaircase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 16.0, 12.0);
	drawSymbol();
	glPopMatrix();
	
	

}


// ####################################################  SINGLE FLAG ###################################################
void flagHolder() {
	
	glScalef(1.0, 1.0, 0.5);
	singleStaircase();

	//glTranslatef(5.0, 5.0, 5.0);
	drawCylinder(1.0, 50.0);


	//glTranslatef(4.0, 100.0, 5.0);
	glScalef(1.0, 1.0, 0.2);
	singleStaircase();


}

// ####################################################### FRONT FLAG HOLDERS ########################################
void drawFrontFlagholder() {
	
	for (int i = 1; i <6; i++) {
		glTranslatef(i*2, 0, 0);
		flagHolder();
	}
}

// ################################################### large towers #######################################
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
	const float circleRadius = 150.0; // Radius of the circle

	for (int setIndex = 0; setIndex < numSets; setIndex++) {
		float angleIncrement = 360.0 / numChairs; // Angle between chairs

		for (int chairIndex = 0; chairIndex < numChairs; chairIndex++) {
			float angle = chairIndex * angleIncrement + setIndex * (360.0 / numSets);

			float x = circleRadius * cos(angle * (3.14159265 / 180.0));
			float z = circleRadius * sin(angle * (3.14159265 / 180.0));

			glPushMatrix();
			glTranslatef(x, 0.0, z);
			drawTorus();
			glPopMatrix();
		}
	}
}


//##########################################################  STADIUM  INSIDE CHHAIRS #########################################
void chairLayer() {

	//glTranslatef(0.0, 0.0, -70);
	singleChairSet();

	glTranslatef(-50, 0, 0);
	singleChairSet();

}

void chairSquare() {
	glTranslatef(0.0, 0.0, -70);
	chairLayer();
}




// ################################################### DRAWING SCENE #########################################
void drawScene() {

	
	glPushMatrix();
	glTranslatef(120.0, 0.0, 0.0);
	flagHolder();
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(50.0, 0.0, 125);
	drawFrontFlagholder();
	glPopMatrix();
	


	glPushMatrix();
	glTranslatef(-80.0, 0.0, 90);
	drawFrontFlagholder();
	glPopMatrix();

	displayLamp();
	
	glPushMatrix();
	glTranslatef(-30.0, 0.0, -30.0);
	shootFire();
	glPopMatrix();

	drawCircularFlagSet();
	
	drawLand();
	
	base();
	
	symbolholder();
}



void init(void) {
	glClearColor(0.0, 0.74, 1.0, 1.0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_NORMALIZE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	loadChairTextures();
	loadGrassTexture();
	loadTrackTexture();
	
	loadWallChair();
	loadStairTexture();
	loadWall();
	loadTowerTexture();
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(0.0 + camX, 0.0 + camY, 150.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(sceTX, sceTY, sceTZ);
	glRotatef(sceRY, 0.0, 1.0, 0.0);
	glRotatef(objRY, 0.0, 1.0, 0.0);
	
	setLighting();
	//drawAxes();
	//drawGrid();
	drawScene();

	glPopMatrix();
	glutSwapBuffers();

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
		camY += 5;
	if (key == GLUT_KEY_DOWN)
		camY -= 5;
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
		sceTZ -= 5;
	if (key == 'Z')
		sceTZ += 5;
	if (key == 'x')
		sceTX -= 5;
	if (key == 'X')
		sceTX += 5;
	if (key == 'y')
		sceRY += 1;
	if (key == 'Y')
		sceRY -= 5;
	if (key == 'n')
		sceTY += 5;
	if (key == 'm')
		sceTY -= 5;
	if (key == 'l')
		objRY -= 3;
	if (key == 'r')
		objRY += 3;
	if (key == '!')
		glEnable(GL_LIGHT0);
	if (key == '@')
		glDisable(GL_LIGHT0);
	if (key == 'c')
		camX -= 5;
	if (key == 'C')
		camX += 5;

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