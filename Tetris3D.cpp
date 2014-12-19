#include<iostream>
#include<stdio.h>
#include<time.h>
#include<GL/glut.h>
#include<GL/gl.h>
#include<math.h>
#include<stdarg.h>
#include <Windows.h>
#include<cstdlib>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
// Screen Properties
#define FULL_HD 1			// FULL HD or HD?
#define WINDOW_WIDTH  (FULL_HD==1? 1920:1366)	
#define WINDOW_HEIGHT (FULL_HD==1? 1080:768) 
#define SPEED_OF_FALL 0.4
#define CUBE_SIDE 0.9
#define NO_OF_CUBES 15
#define ARENA_SIZE CUBE_SIDE*NO_OF_CUBES
#define TETRIS_HEIGHT 20
#define NO_OF_SHAPES 6
#define LIFT_UP 0.02
#define REDUCE_SPEED 0.01
#define MOVE_ALL_DOWN 1000
#define SCORE_ADD 13
#define GAME_OVER_HEIGHT 8
#define BOMB_SPEED 0.10


#define N 100 // Number of cubes?
using namespace std;


#pragma region Variables
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = -0.25;
//float lx = 0.0f, lz = 0.0f, ly = -0.0;
// XZ position of the camera
float x = ARENA_SIZE / 2, z = 35.0f, y = 14.0f;
//float x = ARENA_SIZE / 2, z = -25.0f, y = 5.0f;
int random[10000] = { 0 };
//float angle = 0.0f, rangle = 0.0f;
int w = 640, h = 480;
float ux = 0.0f;
float uy = 1.0f;
double slowness = SPEED_OF_FALL;
long long score = 0;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
int index_score = 1;
int distruct_flag = 0;
int money_cheat = 0;
int set_snowman = 0;

#pragma endregion 
//void display();
// void renderScene();
// Stores the height of the [i][j]th tetris column
int board[NO_OF_CUBES + 1][NO_OF_CUBES + 1];
int blocks[NO_OF_CUBES + 1][NO_OF_CUBES + 1][TETRIS_HEIGHT + 1];
int currentDrop = TETRIS_HEIGHT;
int shape[NO_OF_CUBES + 1][NO_OF_CUBES + 1];

double redMatrix[NO_OF_CUBES + 1][NO_OF_CUBES + 1][TETRIS_HEIGHT + 1];
double greenMatrix[NO_OF_CUBES + 1][NO_OF_CUBES + 1][TETRIS_HEIGHT + 1];
double blueMatrix[NO_OF_CUBES + 1][NO_OF_CUBES + 1][TETRIS_HEIGHT + 1];

double start_time = 0.0;
double time_elapsed = 0.0;
int posX = 5, posY = 6;
int shouldPlaceX = 1000, shouldPlaceY = 1000, shouldPlaceZ = 1000;
int shouldDarkenX = 1000, shouldDarkenY = 1000;
double red = (rand() % 256) / 256.0;
double green = (rand() % 256) / 256.0;
double blue = (rand() % 256) / 256.0;

double escX = x, escY = y, escZ = z, esclx = lx, escly = ly, esclz = lz;
double greyscale;// , greenchange, bluechange;

void printw(float x, float y, float z, char* format, ...)
{
	va_list args;   //  Variable argument list
	int len;        // String length
	int i;          //  Iterator
	char *text;    // Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	// _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list
	va_end(args);
	//glPushMatrix();
	//glScalef(3.0f, 3.0f, 3.0f);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(x, y, z);
	//glPopMatrix();

	//  Draw the characters one by one
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}
void setSnowMan(){

	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glPushMatrix();
	glTranslatef(18.0f, 4.0f, ARENA_SIZE / 2.0);
	glutSolidSphere(4.00f, 20, 20);
	glPopMatrix();

	// Draw Head
	glPushMatrix();
	glTranslatef(18.0, 9.0f, ARENA_SIZE / 2.0);
	glutSolidSphere(3.0f, 20, 20);
	glPopMatrix();



	// Draw Head
	glPushMatrix();
	glTranslatef(18.0, 13.5f, ARENA_SIZE / 2.0);
	glutSolidSphere(2.0f, 20, 20);
	glPopMatrix();
	glPushMatrix();
	/*
	glTranslatef(0.0, 3.0f, 0);
	glutSolidSphere(2.5f, 20, 20);
	*/

	// Draw Eyes		
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(17.0f, 14.5f, 2 + ARENA_SIZE / 2);
	glutSolidSphere(0.2f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glPopMatrix();

	// Draw mouth?
	glPushMatrix();
	glTranslatef(18.0, 12.4f, 2 + ARENA_SIZE / 2.0);
	glScaled(1, 0.2, 1);
	glutSolidCube(0.4);
	glPopMatrix();

	/*glPushMatrix();
	glTranslatef(17.8, 12.6f, 2 + ARENA_SIZE / 2.0);
	glScaled(0.2, 1.4, 1);
	//glRotated(150,0,0,1);
	glutSolidCube(0.2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(18.2, 12.6f, 2 + ARENA_SIZE / 2.0);
	glScaled(0.2, 1.4, 1);

	//glRotated(150,0,0,1);
	glutSolidCube(0.2);
	glPopMatrix();
	*/

	// Draw hand
	glPushMatrix();
	glTranslatef(18.0, 10.0f, ARENA_SIZE / 2.0);
	glScaled(17, 0.8, 1);
	glutSolidCube(0.8);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(19.0f, 14.5f, 2 + ARENA_SIZE / 2);
	glutSolidSphere(0.2f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glPopMatrix();



	// Draw Nose
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.5f);
	glTranslatef(18.0, 13.5f, 3 + ARENA_SIZE / 2.0);
	glutSolidCone(0.4f, 3.0f, 10, 2);
	glPopMatrix();
	//glFlush();

}
void placeCube(int i, int j, int k = 0) {
	glPushMatrix();
	glTranslatef(i*CUBE_SIDE + CUBE_SIDE / 2, k*CUBE_SIDE + CUBE_SIDE / 2.0, j*CUBE_SIDE - CUBE_SIDE / 2);

	//glColor3f(0.0f, 0.0f, 0.9f);
	//glColor3f(red, green, blue);

	glutSolidCube(CUBE_SIDE - 0.009);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutWireCube(CUBE_SIDE);
	glPopMatrix();
}
void darkenSquare(int p, int q){
	glColor3f(greyscale, greyscale, greyscale);
	for (int i = 0; i < NO_OF_CUBES; i++){
		for (int j = 0; j < NO_OF_CUBES; j++){
			if (shape[i][j]){
				glBegin(GL_QUADS);
				glVertex3f(i*CUBE_SIDE, board[i][j] * CUBE_SIDE + 2 * LIFT_UP, (j - 1)*CUBE_SIDE);
				glVertex3f((i + 1)*CUBE_SIDE, board[i][j] * CUBE_SIDE + 2 * LIFT_UP, (j - 1)*CUBE_SIDE);
				glVertex3f((i + 1)*CUBE_SIDE, board[i][j] * CUBE_SIDE + 2 * LIFT_UP, (j)*CUBE_SIDE);
				glVertex3f(i*CUBE_SIDE, board[i][j] * CUBE_SIDE + 2 * LIFT_UP, (j)*CUBE_SIDE);
				glEnd();
			}
		}
	}

}

void mousePressed(int button, int state, int px, int py){

}
void moveFront(){
	float fraction = 0.01f;
	//x += lx * fraction;
	//z += lz * fraction;

}

void setBoard(){
	glColor3f(0.6, 0.6, 0.6);	// BackGround 1
	glBegin(GL_QUADS);
	glVertex3f(-100000, -0.5f, -100000);
	glVertex3f(-100000, -0.5f, 100000);

	glVertex3f(100000, -0.5f, 100000);
	glVertex3f(100000, -0.5f, -100000);
	glEnd();

	glColor3f(0.9f, 0.9f, 0.9f); // Colour of wall
	glBegin(GL_QUADS);
	glVertex3f(-3 * ARENA_SIZE, 0.0f, 0.0f - 5 * CUBE_SIDE);
	glVertex3f(-3 * ARENA_SIZE, 1000.0f, 0.0f - 5 * CUBE_SIDE);
	glVertex3f(3 * ARENA_SIZE, 1000.0f, 0.0f - 5 * CUBE_SIDE);
	glVertex3f(3 * ARENA_SIZE, 0.0f, 0.0f - 5 * CUBE_SIDE);
	glEnd();

	glColor3f(0.8, 0.8, 0.8); // Colour of wall
	glBegin(GL_QUADS);
	glVertex3f(-ARENA_SIZE, 0.0f, 0.0f - 5 * CUBE_SIDE);
	glVertex3f(-ARENA_SIZE, 0.0f, ARENA_SIZE - 5 * CUBE_SIDE);
	glVertex3f(-ARENA_SIZE, 1000.0f, ARENA_SIZE - 5 * CUBE_SIDE);
	glVertex3f(-ARENA_SIZE, 1000.0f, 0.0f - 5 * CUBE_SIDE);
	glEnd();

	glColor3f(0.8, 0.8, 0.8); // Colour of wall
	glBegin(GL_QUADS);
	glVertex3f(2 * ARENA_SIZE, 0.0f, 0.0f - 5 * CUBE_SIDE);
	glVertex3f(2 * ARENA_SIZE, 0.0f, ARENA_SIZE - 5 * CUBE_SIDE);
	glVertex3f(2 * ARENA_SIZE, 1000.0f, ARENA_SIZE - 5 * CUBE_SIDE);
	glVertex3f(2 * ARENA_SIZE, 1000.0f, 0.0f - 5 * CUBE_SIDE);
	glEnd();

	/*
	glColor3f(0.9f, 0.9f, 0.9f); // Colour of wall
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f - CUBE_SIDE);
	glVertex3f(0.0f, 1000.0f, 0.0f - CUBE_SIDE);
	glVertex3f(ARENA_SIZE, 1000.0f, 0.0f - CUBE_SIDE);
	glVertex3f(ARENA_SIZE, 0.0f, 0.0f - CUBE_SIDE);
	glEnd();

	glColor3f(0.8, 0.8, 0.8); // Colour of wall
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f - CUBE_SIDE);
	glVertex3f(0.0f, 0.0f, ARENA_SIZE - CUBE_SIDE);
	glVertex3f(0.0f, 1000.0f, ARENA_SIZE - CUBE_SIDE);
	glVertex3f(0.0f, 1000.0f, 0.0f - CUBE_SIDE);
	glEnd();

	glColor3f(0.8, 0.8, 0.8); // Colour of wall
	glBegin(GL_QUADS);
	glVertex3f(ARENA_SIZE, 0.0f, 0.0f - CUBE_SIDE);
	glVertex3f(ARENA_SIZE, 0.0f, ARENA_SIZE - CUBE_SIDE);
	glVertex3f(ARENA_SIZE, 1000.0f, ARENA_SIZE - CUBE_SIDE);
	glVertex3f(ARENA_SIZE, 1000.0f, 0.0f - CUBE_SIDE);
	glEnd();
	*/

	double p, q;
	for (int i = 0; i < NO_OF_CUBES; i++){
		for (int j = 0; j < NO_OF_CUBES; j++){

			if ((i + j) % 2){
				glColor3f(1.0f, 1.0f, 1.0f);	// Colour 1
			}
			else{
				glColor3f(0.9f, 0.9f, 0.9f);	// Colour 2
			}
			//if (i==0&&j==0)
			//	glColor3f(0.0f, 0.0f, 1.0f);
			p = i*CUBE_SIDE; q = j*CUBE_SIDE;
			glBegin(GL_QUADS);
			glVertex3f(p, 0, q);
			glVertex3f(p + CUBE_SIDE, 0, q);
			//	glVertex3f(p + CUBE_SIDE, 0.0f, CUBE_SIDE -q );
			//	glVertex3f(p, 0.0f,CUBE_SIDE -q  );
			glVertex3f(p + CUBE_SIDE, 0, q - CUBE_SIDE);
			glVertex3f(p, 0, q - CUBE_SIDE);
			glEnd();

		}
	}
}
void setHistory(){
	int moveAllDown = 0;
	int moveAllPos = -1;
	for (int k = 0; k < TETRIS_HEIGHT; k++){
		int countBlocks = 0;
		for (int i = 0; i < NO_OF_CUBES; i++){
			for (int j = 0; j < NO_OF_CUBES; j++){
				if (blocks[i][j][k])
					countBlocks++;
			}
		}
		if (countBlocks == NO_OF_CUBES*NO_OF_CUBES){
			moveAllDown = 1;
			moveAllPos = k;
		}
	}
	if (moveAllDown){
		for (int i = 0; i < NO_OF_CUBES; i++){
			for (int j = 0; j < NO_OF_CUBES; j++){
				if (board[i][j])
					board[i][j]--;
			}
		}
		for (int k = moveAllPos; k < TETRIS_HEIGHT; k++){
			for (int i = 0; i < NO_OF_CUBES; i++){
				for (int j = 0; j < NO_OF_CUBES; j++){
					blocks[i][j][k] = blocks[i][j][k + 1];
				}
			}
		}
		score += MOVE_ALL_DOWN;
	}
	for (int i = 0; i < NO_OF_CUBES; i++){
		for (int j = 0; j < NO_OF_CUBES; j++){
			for (int k = 0; k < TETRIS_HEIGHT; k++){
				if (blocks[i][j][k]){
					glColor3f(redMatrix[i][j][k], greenMatrix[i][j][k], blueMatrix[i][j][k]);
					placeCube(i, j, k);
				}
			}
		}
	}
	darkenSquare(posX, posY - 1);

	for (int i = 0; i < NO_OF_CUBES; i++){
		for (int j = 0; j < NO_OF_CUBES; j++){
			if (shape[i][j]){
				if (distruct_flag != 1){
					glColor3f(red, green, blue);
					placeCube(i, j, currentDrop);
				}
				else{
					glColor3f((rand() % 256) / 256.0, (rand() % 256) / 256.0, (rand() % 256) / 256.0);
					placeCube(i, j, currentDrop);
				}

			}
		}
	}
	for (int i = 0; i < NO_OF_CUBES; i++){
		for (int j = 0; j < NO_OF_CUBES; j++){
			if (board[i][j] >= GAME_OVER_HEIGHT){
				if (!distruct_flag){
					glColor3f(1.0f, 1.0f, 0.0f);
					printw(-10, 0, 6, "GAME OVER !!\n Score is %lld", score);
					slowness = INFINITE - 1;
				}
			}
		}
	}
	//placeCube(posX, posY, currentDrop);

}
void playDropSound(){
	PlaySound(TEXT("D:\\sounds\\zap.wav"), NULL, SND_ASYNC | SND_FILENAME);
}
void createNewShape(){
	greyscale = 0.8f;
	money_cheat = 0;

	red = (rand() % 256) / 256.0;
	green = (rand() % 256) / 256.0;
	blue = (rand() % 256) / 256.0;

	memset(shape, 0, sizeof(shape));
	posX = rand() % (NO_OF_CUBES - 3);
	posY = rand() % (NO_OF_CUBES - 3);
	glColor3f(red, green, blue);
	int shapeID = rand() % NO_OF_SHAPES;
	//shapeID = 4;
	switch (shapeID){
	case 0:
		shape[posX][posY] = 1;
		break;
	case 1:
		shape[posX][posY] = 1;
		shape[posX + 1][posY] = 1;
		shape[posX + 2][posY] = 1;
		break;
	case 2:
		shape[posX][posY] = 1;
		shape[posX + 1][posY] = 1;
		shape[posX + 2][posY] = 1;
		shape[posX + 2][posY - 1] = 1;
		break;
	case 3:
		shape[posX][posY] = 1;
		shape[posX + 1][posY] = 1;
		shape[posX + 2][posY] = 1;
		shape[posX + 1][posY + 1] = 1;
		break;
	case 4:
		shape[posX][posY] = 1;
		shape[posX + 1][posY] = 1;
		shape[posX + 1][posY + 1] = 1;
		shape[posX][posY + 1] = 1;
		break;
	case 5:
		shape[posX][posY] = 1;
		shape[posX + 1][posY] = 1;
		shape[posX][posY - 1] = 1;
		shape[posX + 1][posY + 1] = 1;
		break;
	}
	currentDrop = TETRIS_HEIGHT;
}
void createDistro(){
	for (int k = 0; k < TETRIS_HEIGHT; k++){
		for (int i = 0; i < NO_OF_CUBES; i++){
			for (int j = 0; j < NO_OF_CUBES; j++){
				blocks[i][j][k] = 0;
				board[i][j] = 0;
			}
		}
	}
}
void checkTimer(){
	int flag = 0;
	double now_time = clock();
	time_elapsed = (now_time - start_time) / CLOCKS_PER_SEC;
	if (currentDrop >= 0 && time_elapsed >= slowness){
		greyscale -= ((double)0.5 / TETRIS_HEIGHT);
		start_time = clock();
#pragma region CheckStatusOfBlocks
		for (int i = 0; i < NO_OF_CUBES; i++){
			for (int j = 0; j < NO_OF_CUBES; j++){
				if (shape[i][j] && (blocks[i][j][currentDrop - 1] || currentDrop == 0)){
					// If control gets here, it means that we need to place shape at this level
					// wherever it currently is
					flag = 1;
					goto lab;
				}
			}
		}
#pragma endregion
	lab:
		if (flag == 0){
			currentDrop--;
		}
		else{
#pragma region BlockHalts
#pragma region SetBlockFlags
			for (int i = 0; i < NO_OF_CUBES; i++){
				for (int j = 0; j < NO_OF_CUBES; j++){
					if (shape[i][j]){
						blocks[i][j][currentDrop] = 1;
						redMatrix[i][j][currentDrop] = red;
						greenMatrix[i][j][currentDrop] = green;
						blueMatrix[i][j][currentDrop] = blue;
					}
				}
			}
#pragma endregion
			if (currentDrop == 0){
#pragma region BlockReachFloor
				for (int i = 0; i < NO_OF_CUBES; i++){
					for (int j = 0; j < NO_OF_CUBES; j++){
						if (shape[i][j])
							board[i][j] = 1;
					}
				}
#pragma endregion
			}
			else{
#pragma region BlockStoppedMidway
				for (int i = 0; i < NO_OF_CUBES; i++){
					for (int j = 0; j < NO_OF_CUBES; j++){
						if (shape[i][j]){
							board[i][j] ++;
						}
					}
				}
#pragma endregion
			}
			if (!distruct_flag)playDropSound();
			if (distruct_flag){
				distruct_flag = 0;
				createDistro();
				PlaySound(TEXT("D:\\sounds\\explode1.wav"), NULL, SND_ASYNC | SND_FILENAME);
			}
			createNewShape();
			score += SCORE_ADD*index_score;
			index_score++;
#pragma endregion
			slowness = SPEED_OF_FALL;
		}
#pragma region SingleCube
		/*if (blocks[posX][posY][currentDrop - 1] || currentDrop == 0){
		blocks[posX][posY][currentDrop] = 1;
		if (currentDrop == 0)
		board[posX][posY] = 1;
		else
		board[posX][posY] ++;
		createNewShape();
		}
		else{
		currentDrop--;
		}
		*/
#pragma endregion
#pragma region Try1
		/*if (currentDrop == TETRIS_HEIGHT)
		currentDrop--;
		else{

		//this_thread::sleep_for(std::chrono::seconds(1));
		//	if (blocks[posX][posY][currentDrop])
		//		blocks[posX][posY][currentDrop - 1] = 1;
		//shouldPlaceZ = currentDrop;
		//	shouldDarkenX = posX;
		//	shouldDarkenY = posY;
		//	placeCube(posX, posY, currentDrop);
		//	shouldPlaceX = posX;
		//	shouldPlaceY = posY;

		if (current)

		if (currentDrop == 0){
		blocks[posX][posY][0] = 1;
		}
		//currentDrop--;
		*/
#pragma endregion
		//glFlush();

	}
	//if (slowness != REDUCE_SPEED){
	//	setSnowMan();
	//}
}
void renderScene() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1, 0.0f);
	//gluLookAt(x + lx, y + ly, z + lz, x , y, z , 0.0f, 1, 0.0f);

	printw(-10, 0, 3, "Score : %ld", score);
	glColor3f(0.0f, 0.0f, 0.0f);
	if (slowness == INFINITE)
		printw(4.5, 8, 3, "GAME PAUSED !");
	if (distruct_flag){
		printw(0.5, 8, 3, "Cheat Code Activated!! Initiating Distruction Sequence.");
		slowness = BOMB_SPEED;
	}
	if (money_cheat){
		printw(0.5, 8, 3, "Cheat Code Activated!! It is Jackpot time!");
	}
	if (set_snowman)
		setSnowMan();
	setHistory();
	setBoard();
	checkTimer();

	glFlush();
}
void processSpecialKeys(int key, int xx, int yy) {
	//	placeCube(posX, posY, currentDrop);
	//this_thread::sleep_for(std::chrono::seconds(1));
	int flag = 0;
	glFlush();
	switch (key) {
	case GLUT_KEY_LEFT:
		for (int i = 0; i < NO_OF_CUBES; i++){
			if (shape[0][i])
				return;
		}
		for (int i = 0; i < NO_OF_CUBES - 1; i++){
			for (int j = 0; j < NO_OF_CUBES; j++){
				shape[i][j] = shape[i + 1][j];
			}
		}
		for (int i = 0; i < NO_OF_CUBES; i++){
			shape[NO_OF_CUBES - 1][i] = 0;
		}
		break;
	case GLUT_KEY_RIGHT:
		for (int i = 0; i < NO_OF_CUBES; i++){
			if (shape[NO_OF_CUBES - 1][i])
				return;
		}
		for (int i = NO_OF_CUBES - 1; i > 0; i--){
			for (int j = NO_OF_CUBES - 1; j >= 0; j--){
				shape[i][j] = shape[i - 1][j];
			}
		}
		for (int i = 0; i < NO_OF_CUBES; i++){
			shape[0][i] = 0;
		}
		break;
	case GLUT_KEY_UP:
		for (int i = 0; i < NO_OF_CUBES; i++){
			if (shape[i][0])
				return;
		}
		for (int i = 0; i < NO_OF_CUBES; i++){
			for (int j = 0; j < NO_OF_CUBES - 1; j++){
				shape[i][j] = shape[i][j + 1];
			}
		}
		for (int i = 0; i < NO_OF_CUBES; i++){
			shape[i][NO_OF_CUBES - 1] = 0;
		}
		break;
	case GLUT_KEY_DOWN:
		for (int i = 0; i < NO_OF_CUBES; i++){
			if (shape[i][NO_OF_CUBES - 1])
				return;
		}
		for (int i = NO_OF_CUBES - 1; i >= 0; i--){
			for (int j = NO_OF_CUBES - 1; j > 0; j--){
				shape[i][j] = shape[i][j - 1];
			}
		}
		for (int i = 0; i < NO_OF_CUBES; i++){
			shape[i][0] = 0;
		}
		break;
	}
	renderScene();
	//placeCube(posX, posY, 0);
	//glFlush();
}

void keyPressed(unsigned char key, int x, int y){
	if (key == 32){
		if (slowness == REDUCE_SPEED)
			slowness = SPEED_OF_FALL;
		else
			slowness = REDUCE_SPEED;
	}
	if (key == 'm'){
		set_snowman ^= 1;
	}
	if (key == 13){
		slowness = REDUCE_SPEED;
	}
	if (key == 'p'){
		if (slowness == INFINITE)
			slowness = SPEED_OF_FALL;
		else
			slowness = INFINITE;
	}
	if (key == '$'){
		money_cheat = 1;
		score += 100000;
	}
	float fraction = 0.8f;
	if ('0' <= (int)key && (int)key <= '9')
		PlaySound(TEXT("D:\\sounds\\camera.wav"), NULL, SND_ASYNC | SND_FILENAME);

	switch (key) {
	case '1':
		x = escX; y = escY; z = escZ; lx = esclx; ly = escly; lz = esclz;
		break;
	case '2':
		x = 1770; y = 622; z = 22.2;  lx = 0; ly = -0.644218; lz = -1;
		break;
	case '3':
		x = 794; y = 529; z = 47;  lx = 0; ly = -0.0168103; lz = -1;
		break;
	case '+':
		distruct_flag = 1;
		if (slowness == INFINITE - 1){
			score = 0;
			index_score = 0;
		}
		//printw()
		//PlaySound(TEXT("D:\\sounds\\explode1.wav"), NULL, SND_ASYNC | SND_FILENAME);
		//createDistro();

		break;

#pragma region KeysASDW
	case 'a':
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'd':
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'w':
		x += lx * fraction;
		z += lz * fraction;
		break;
	case 's':
		x -= lx * fraction;
		z -= lz * fraction;
		break;
		//case 'p':

#pragma endregion
#pragma region KeysTG
	case 't':
		angle += 0.1f;
		ly = sin(angle);
		break;
	case 'g':
		angle += 0.1f;
		ly = -sin(angle);
		break;
#pragma endregion
#pragma region RotateKey
	case 'r':

		int mostX = -1, mostY = -1, leastX = 2 * NO_OF_CUBES, leastY = 2 * NO_OF_CUBES;
		for (int i = 0; i < NO_OF_CUBES; i++){
			for (int j = 0; j < NO_OF_CUBES; j++){
				if (shape[i][j]){
					if (i <= leastX)
						leastX = i;
					if (i >= mostX)
						mostX = i;
					if (j <= leastY)
						leastY = j;
					if (j >= mostY)
						mostY = j;
				}
			}
		}
		int n = max(mostX - leastX + 1, mostY - leastY + 1);
		int f = floor(n / 2);
		int c = ceil(n / 2);
		int temp;
		if (NO_OF_CUBES - mostX < 2 || NO_OF_CUBES - mostY < 2)
			return;
		PlaySound(TEXT("D:\\sounds\\rotate.wav"), NULL, SND_ASYNC | SND_FILENAME);
		for (int x = 0; x <= f; x++){
			for (int y = 0; y < c; y++){
				int temp = shape[x + leastX][y + leastY];

				shape[x + leastX][y + leastY] = shape[y + leastX][n - 1 - x + leastY];
				shape[y + leastX][n + leastY - 1 - x] = shape[n - 1 - x + leastX][n - 1 - y + leastY];
				shape[n + leastX - 1 - x][n + leastY - 1 - y] = shape[n - 1 - y + leastX][x + leastY];
				shape[n + leastX - 1 - y][x + leastY] = temp;
			}
		}
		renderScene();
		break;
#pragma endregion
	}
	//cout << x << " " << y << " " << z << " " << lx << " " << ly << " " << lz << " " << endl;
}

void display(void){
	renderScene();
	glFlush();
}
void timerEvent(int t){
	glutTimerFunc(1000, timerEvent, 1000);
	//placeCube(posX, posY);
	cout << "placed at" << posX << " " << posY << " " << currentDrop << endl;
	cout << endl << endl << endl << "hi" << endl << endl << endl;
	if (currentDrop >= 0){
		if (currentDrop == TETRIS_HEIGHT)
			currentDrop--;
		else{
			darkenSquare(posX, posY);

			//this_thread::sleep_for(std::chrono::seconds(1));

			if (blocks[posX][posY][currentDrop])
				blocks[posX][posY][currentDrop - 1] = 1;

			placeCube(posX, posY, currentDrop);
			glFlush();

			currentDrop--;
			if (currentDrop == 0)
				blocks[posX][posY][0] = 1;
		}
	}

	glutPostRedisplay();
	glFlush();
	cout << "hi";
}

void mouseMoved(int x, int y){
	float fraction = 0.8f;
	if (x > 2 * WINDOW_WIDTH / 3){
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
	}
	else if (x < WINDOW_WIDTH / 3){
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
	}
}

void resize(int w, int h)
{
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv){
	start_time = clock();
	//escX = x; escY = y; escZ = z; esclx = lx; escly = ly; esclz = lz;
	srand(time(NULL));
	createNewShape();
	//PlaySound(TEXT("D:\\sounds\\kathi.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Tetris 3D");


	glutDisplayFunc(moveFront);
	glutReshapeFunc(resize);
	glutMouseFunc(mousePressed);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(processSpecialKeys);
	//	glutTimerFunc(1000,timerEvent,1000);// oid(*func)(int value), value);

	glutIdleFunc(renderScene);
	glutSpecialFunc(processSpecialKeys);
	//	glutPassiveMotionFunc(mouseMoved);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}
