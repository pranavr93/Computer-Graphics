// Author Pranav Ramarao
// BITS Hyderabad 
#include<iostream>
#include<GL/glut.h>
#include<GL/GL.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
using namespace std;

#pragma region CONSTANTS
#define N 100				// No. of vertices in the graph
#define L0	900				// L0 is the length of a side of display square area
#define K 5					// K is a constant
#define EPS 20				// EPS is the threshold 

#define INF	1<<27			// Initializing INF to 2^27	

#define RADIUS 25			// Radius of nodes 
#define FULL_HD 1			// FULL HD or HD?

#define WINDOW_WIDTH  (FULL_HD==1? 1920:1366)	
#define WINDOW_HEIGHT (FULL_HD==1? 1080:768) 

#define BACKGROUND_RED 50		// Background Red Value
#define BACKGROUND_GREEN 50		// Background Green Value
#define BACKGROUND_BLUE 50		// Background Blue Value

#define DRAW_RED 255
#define DRAW_GREEN 255
#define DRAW_BLUE 255

#define ANIMATION 1
#define MORE_MOTION 1

#define EXTRA_MOVEMENT 100
#define RIGHT_MARGIN 1900
#define LEFT_MARGIN 20
#define TOP_MARGIN 20
#define BOTTOM_MARGIN 1060

#define ERROR 10
#define ITERATIONS_LIMIT1 200
#define ITERATIONS_LIMIT2 200

#define RADIUS_SQ (RADIUS+ERROR) * (RADIUS+ERROR)

#define EDGE_COLOR1 0/255.0
#define EDGE_COLOR2 200/255.0
#define EDGE_COLOR3 255/255.0

#define HIGHLIGHT_CIRCLE1 255/255.0
#define HIGHLIGHT_CIRCLE2 255/255.0
#define HIGHLIGHT_CIRCLE3 0/255.0
#pragma endregion CONSTANTS

#pragma region VECTORS_USED
vector<int> graph[N + 1];
vector<int> dist[N + 1];
vector<double> LDist[N + 1];
vector<double> KDist[N + 1];
vector<int> x;
vector<int> y;
vector<int> x_copy;
vector<int> y_copy;
#pragma endregion VECTORS_USED

#pragma region VARIBALE_FLAGS
int stage1 = 1, stage2 = 0, stage3 = 0;
int storeX;
int storeY;
int clicked = 0;
int superflag = 0;
int storeIndex = -1;
int num;
int globflag = 1;
#pragma endregion VARIBALE_FLAGS

void myInit()
{
	//glClearColor(BACKGROUND_RED/255.0, BACKGROUND_GREEN/255.0, BACKGROUND_BLUE/255.0, 1.0);
	glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);
	glMatrixMode(GL_MODELVIEW);
}
void myDisplay()
{

	glClearColor(BACKGROUND_RED / 255.0, BACKGROUND_GREEN / 255.0, BACKGROUND_BLUE / 255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

#pragma region line
void swap(GLint* x0, GLint* y0, GLint* x1, GLint* y1)
{
	GLint tmp;
	tmp = *x0;
	*x0 = *x1;
	*x1 = tmp;
	tmp = *y0;
	*y0 = *y1;
	*y1 = tmp;
}

void drawLine(int x0, int y0, int x1, int y1)
{
	int dx, dy, d, x, y;
	float m;
	if (x0 == x1)
	{
		if (y1 < y0)
		{
			swap(&x0, &y0, &x1, &y1);
		}
		x = x0;
		y = y0;
		while (y <= y1)
		{
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
			y++;
		}
	}
	else
	{
		m = (y1 - y0 + 0.0) / (x1 - x0 + 0.0);
		if ((m >= 0) && (m <= 1))
		{
			if (x1 < x0)
			{
				swap(&x0, &y0, &x1, &y1);
			}
			dy = y1 - y0;
			dx = x1 - x0;
			d = 2 * dy - dx;
			int incrE = 2 * dy;
			int incrNE = 2 * (dy - dx);
			x = x0;
			y = y0;
			while (x <= x1)
			{
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				if (d <= 0)
				{
					d += incrE;
					x++;
				}
				else
				{
					d += incrNE;
					x++;
					y++;
				}
			}
		}
		else if (m > 1)
		{
			if (x1 < x0)
			{
				swap(&x0, &y0, &x1, &y1);
			}
			dy = y1 - y0;
			dx = x1 - x0;
			d = dy - 2 * dx;
			int incrN = -2 * dx;
			int incrNE = 2 * (dy - dx);
			x = x0;
			y = y0;
			while (y <= y1)
			{
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				if (d > 0)
				{
					d += incrN;
					y++;
				}
				else
				{
					d += incrNE;
					x++;
					y++;
				}
			}
		}
		else if ((m >= -1) && (m < 0))
		{
			if (x1 < x0)
			{
				swap(&x0, &y0, &x1, &y1);
			}
			dy = y1 - y0;
			dx = x1 - x0;
			d = 2 * dy + dx;
			int incrSE = 2 * (dy + dx);
			int incrE = 2 * dy;
			x = x0;
			y = y0;
			while (x <= x1)
			{
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				if (d <= 0)
				{
					d += incrSE;
					x++;
					y--;
				}
				else
				{
					d += incrE;
					x++;
				}
			}
		}
		else
		{
			if (x1 < x0)
			{
				swap(&x0, &y0, &x1, &y1);
			}
			dy = y1 - y0;
			dx = x1 - x0;
			d = dy + 2 * dx;
			int incrS = 2 * dx;
			int incrSE = 2 * (dy + dx);
			x = x0;
			y = y0;
			while (y >= y1)
			{
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				if (d <= 0)
				{
					d += incrS;
					y--;
				}
				else
				{
					d += incrSE;
					x++;
					y--;
				}
			}
		}
	}
	glFlush();
}
#pragma endregion line
#pragma region circle
void circlePoints(int x, int y, int xc, int yc)
{
	glBegin(GL_POINTS);
	glVertex2i(xc + x, yc + y);
	glVertex2i(xc - x, yc + y);
	glVertex2i(xc + x, yc - y);
	glVertex2i(xc - x, yc - y);
	glVertex2i(xc + y, yc + x);
	glVertex2i(xc - y, yc + x);
	glVertex2i(xc + y, yc - x);
	glVertex2i(xc - y, yc - x);
	glEnd();
}
void drawCircle(int xc, int yc)
{

	int radius = RADIUS;
	int x = 0;
	int y = radius;
	int d = 1 - radius;
	int deltaE = 3;
	int deltaSE = -2 * radius + 5;
	circlePoints(x, y, xc, yc);
	while (y>x)
	{
		if (d<0)
		{
			d = d + deltaE;
			deltaE = deltaE + 2;
			deltaSE = deltaSE + 2;
		}
		else
		{
			d = d + deltaSE;
			deltaE = deltaE + 2;
			deltaSE = deltaSE + 4;
			y--;
		}
		x++;
		circlePoints(x, y, xc, yc);
	}
	glFlush();
	//_sleep(500);
}
#pragma endregion circle
#pragma region PointsReordering

void FloydWarshall()
{
	for (int i = 0; i <= num; i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			dist[i].push_back(graph[i][j]);
		}
	}
	for (int intermediateNode = 1; intermediateNode <= num; intermediateNode++)
	{
		for (int i = 1; i <= num; i++)
		{
			for (int j = 1; j <= num; j++)
			{
				if (dist[i][intermediateNode] + dist[intermediateNode][j] < dist[i][j]){
					dist[i][j] = dist[i][intermediateNode] + dist[intermediateNode][j];
				}
			}
		}
	}
}

int findDiameter(){
	int diameter = -1;
	for (int i = 1; i <= num; i++){
		for (int j = 1; j <= num; j++){
			if (dist[i][j]>diameter && dist[i][j] != INF)
				diameter = dist[i][j];
		}
	}
	return diameter;
}

void setLDist(double L){
	for (int i = 0; i <= num; i++){
		for (int j = 0; j <= num; j++){
			if (dist[i][j] == INF){
				LDist[i].push_back(0);
			}
			else{
				LDist[i].push_back(L* dist[i][j]);
			}
		}
	}
}

void setKDist(){
	for (int i = 0; i <= num; i++){
		for (int j = 0; j <= num; j++){
			if (i == j){
				KDist[i].push_back(0);
			}
			else if (dist[i][j] == INF){
				KDist[i].push_back(0);
			}
			else{
				KDist[i].push_back(K / (double)(dist[i][j] * dist[i][j]));
			}

		}
	}
}

void setGraph(){
	//x.push_back(0);
	//y.push_back(0);
	for (int i = 0; i <= N; i++){
		for (int j = 0; j <= N; j++){
			if (i == j)
				graph[i].push_back(0);
			else
				graph[i].push_back(INF);
		}
	}
	/*graph[1][2] = 1;
	graph[2][1] = 1;
	graph[2][3] = 1;
	graph[3][2] = 1;
	graph[2][5] = 1;
	graph[5][2] = 1;

	graph[4][5] = 1;
	graph[5][4] = 1;

	graph[6][5] = 1;
	graph[5][6] = 1;

	graph[6][7] = 1;
	graph[7][6] = 1;

	x[3] = 1; y[3] = 1;
	x[1] = 20; y[1] = 25;
	x[2] = 40; y[2] = 21;
	x[4] = 52; y[4] = 33;
	x[5] = 30; y[5] = 45;
	x[6] = 60; y[6] = 54;
	x[7] = 70; y[7] = 72;
	*/

}

double computeFormulaX(int m, int i){
	if (m == i)
		return 0.0;

	double posX = x[m - 1] - x[i - 1];
	double posY = y[m - 1] - y[i - 1];
	double denominator = sqrt(posX*posX + posY*posY);
	double numerator = LDist[m][i] * posX;
	double res = posX - (numerator / denominator);
	return res*KDist[m][i];

}

double computeFormulaY(int m, int i){
	if (m == i)
		return 0.0;

	double posX = x[m - 1] - x[i - 1];
	double posY = y[m - 1] - y[i - 1];
	double denominator = sqrt(posX*posX + posY*posY);
	double numerator = LDist[m][i] * posY;
	double res = posY - (numerator / denominator);
	return res*KDist[m][i];

}

double computeFormulaXX(int m, int i){
	if (m == i)
		return 0.0;

	double posX = x[m - 1] - x[i - 1];
	double posY = y[m - 1] - y[i - 1];

	double denominator = sqrt(pow(posX*posX + posY*posY, 3));

	double numerator = LDist[m][i] * posY*posY;
	double res = 1 - (numerator / denominator);
	return res*KDist[m][i];
}

double computeFormulaYY(int m, int i){
	if (m == i)
		return 0.0;

	double posX = x[m - 1] - x[i - 1];
	double posY = y[m - 1] - y[i - 1];

	double denominator = sqrt(pow(posX*posX + posY*posY, 3));

	double numerator = LDist[m][i] * posX*posX;
	double res = 1 - (numerator / denominator);
	return res*KDist[m][i];
}

double computeFormulaXY(int m, int i){
	if (m == i)
		return 0.0;

	double posX = x[m - 1] - x[i - 1];
	double posY = y[m - 1] - y[i - 1];

	double denominator = sqrt(pow(posX*posX + posY*posY, 3));

	double numerator = LDist[m][i] * posX*posY;
	double res = (numerator / denominator);
	return res*KDist[m][i];
}

/*void findFirstDiff(){
for (int i = 1; i <= N; i++){
DiffX[i] = resX;
DiffY[i] = resY;
Delta[i] = sqrt(resX*resX + resY*resY);
}
}*/

void findSecondDiff(int m){
	// a1 x + b1 y = c1
	// a2 x + b2 y = c2
	// Solution :
	// x = (c1b2 - c2b1) / (a1b2 - a2b1)
	// y = (a2c1 - a1c2) / (a2b1 - a1b2)
	double resX = 0.0;
	double resY = 0.0;

	double resXX = 0.0;
	double resXY = 0.0;
	double resYY = 0.0;
	double resYX = 0.0;

	double deltaX = 0.0;
	double deltaY = 0.0;

	for (int i = 1; i <= num; i++){
		resXX += computeFormulaXX(m, i);
		resYY += computeFormulaYY(m, i);
		resXY += computeFormulaXY(m, i);
		resX += computeFormulaX(m, i);
		resY += computeFormulaY(m, i);
	}
	resYX = resXY;
	double a1 = resXX;
	double a2 = resXY;
	double b1 = resXY;
	double b2 = resYY;
	double c1 = -1 * resX;
	double c2 = -1 * resY;

	if ((a1*b2 - a2*b1) == 0){
		return;
	}
	if ((a2*b1 - a1*b2) == 0){
		return;
	}
	deltaX = (c1*b2 - c2*b1) / (a1*b2 - a2*b1);
	deltaY = (a2*c1 - a1*c2) / (a2*b1 - a1*b2);
	deltaX += 0.5;
	deltaY += 0.5;
	x[m - 1] += deltaX;
	y[m - 1] += deltaY;

}

double computeCurrentDelta(int m){
	double resX = 0.0;
	double resY = 0.0;
	for (int i = 1; i <= num; i++){
		resX += computeFormulaX(m, i);
		resY += computeFormulaY(m, i);
	}
	return sqrt(resX*resX + resY*resY);
}

int computeMaxDelta(){
	double resX = 0.0;
	double resY = 0.0;

	int maxIndex = -1;
	double maxDelta = -1;

	for (int p = 1; p <= num; p++){
		resX = 0.0;
		resY = 0.0;
		for (int i = 1; i <= num; i++){
			resX += computeFormulaX(p, i);
			resY += computeFormulaY(p, i);
		}
		double current = resX * resX + resY * resY;
		if (current > maxDelta){
			maxDelta = current;
			maxIndex = p;
		}
	}
	return maxIndex;
}
void drawEdge()
{
	int i, j;
	for (i = 1; i <= num; i++)
	{
		for (j = 1; j <= num; j++)
		{
			if (i<j)
			{
				if (graph[i][j] == 1)
				{
					drawLine(x[i - 1], y[i - 1], x[j - 1], y[j - 1]);
				}
			}
		}
	}
}

#pragma endregion PointsReordering

long long computeDistance(int x1, int y1, int x2, int y2){
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}

void drawEverything(){
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < num; i++)
	{
		drawCircle(x[i], y[i]);
	}
	glColor3f(EDGE_COLOR1, EDGE_COLOR2, EDGE_COLOR3);
	drawEdge();
	glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);
}

#pragma region ImageAdjusting
void moveRightWithinScreen(){
	for (int i = 0; i < x.size(); i++){
		if (x[i] < LEFT_MARGIN){
			int moveFront = LEFT_MARGIN - x[i] + EXTRA_MOVEMENT;
			for (int j = 0; j < x.size(); j++){
				x[j] = x[j] + moveFront;
			}
		}
	}
	drawEverything();
}
void moveLeftWithinScreen(){
	for (int i = 0; i < x.size(); i++){
		if (x[i] >= RIGHT_MARGIN){
			int moveFront = x[i] - RIGHT_MARGIN + EXTRA_MOVEMENT;
			for (int j = 0; j < x.size(); j++){
				x[j] = x[j] - moveFront;
			}
		}
	}
	drawEverything();
}
void moveTopWithinScreen(){
	for (int i = 0; i < y.size(); i++){
		if (y[i] >= BOTTOM_MARGIN){
			int moveFront = y[i] - BOTTOM_MARGIN + EXTRA_MOVEMENT;
			for (int j = 0; j < y.size(); j++){
				y[j] = y[j] - moveFront;
			}
		}
	}
	drawEverything();
}
void moveDownWithinScreen(){
	for (int i = 0; i < y.size(); i++){
		if (y[i] <= TOP_MARGIN){
			int moveFront = TOP_MARGIN - y[i] + EXTRA_MOVEMENT;
			for (int j = 0; j < y.size(); j++){
				y[j] = y[j] + moveFront;
			}
		}
	}
	drawEverything();
}
void initiateImageStabilizers(){
	moveRightWithinScreen();
	moveLeftWithinScreen();
	moveTopWithinScreen();
	moveDownWithinScreen();
}
#pragma endregion ImageAdjusting

#pragma region EventHandlers

void mouseClicked(int button, int state, int px, int py)
{
	if (superflag)return;
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		if (clicked == 1){
			for (int i = 0; i < x.size(); i++){
				if (computeDistance(x[i], y[i], px, py) <= RADIUS_SQ){
					glColor3f(EDGE_COLOR1, EDGE_COLOR2, EDGE_COLOR3);
					drawLine(x[i], y[i], storeX, storeY);
					graph[i + 1][storeIndex + 1] = 1;
					graph[storeIndex + 1][i + 1] = 1;
					clicked = 0;
					break;
				}
			}
			if (clicked == 0){
				glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);
				drawCircle(storeX, storeY);
			}
		}
		else{
			for (int i = 0; i < x.size(); i++){
				//	if (max(abs(x[i] - px), abs(y[i] - py)) <= RADIUS){
				if (computeDistance(x[i], y[i], px, py) <= RADIUS_SQ){
					clicked = 1;
					storeX = x[i];
					storeY = y[i];
					storeIndex = i;
					break;
				}
			}
			if (clicked == 1)
			{
				glColor3f(HIGHLIGHT_CIRCLE1, HIGHLIGHT_CIRCLE2, HIGHLIGHT_CIRCLE3);
				drawCircle(storeX, storeY);

			}
		}
	}
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)){
		glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);
		drawCircle(px, py);
		x.push_back(px);
		y.push_back(py);
	}
}
void keyPressed(unsigned char key, int a, int b) {
	superflag = 1;
	glClear(GL_COLOR_BUFFER_BIT);

	if (key == 'c'){
		//glClear(GL_COLOR_BUFFER_BIT);
		x_copy.resize(x.size());
		y_copy.resize(y.size());
		for (int i = 0; i < x.size(); i++){
			x_copy[i] = x[i];
			y_copy[i] = y[i];
		}
		x.clear();
		y.clear();
		for (int i = 0; i <= N; i++){
			LDist[i].clear();
			KDist[i].clear();
			dist[i].clear();
			graph[i].clear();

		}
		num = 0;
		setGraph();
		glFlush();
		//drawEverything();

	}
	if (key == 'r'){
		x.resize(x_copy.size());
		y.resize(y_copy.size());
		for (int i = 0; i < x.size(); i++){
			x[i] = x_copy[i];
			y[i] = y_copy[i];
		}
		num = x.size();
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);

		drawEverything();
		glFlush();
	}
#pragma region abutton
	if (key == 13){
		x_copy.resize(x.size());
		y_copy.resize(y.size());
		for (int i = 0; i < x.size(); i++){
			x_copy[i] = x[i];
			y_copy[i] = y[i];
		}
		num = x.size();
		int i;
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);

		FloydWarshall();
		globflag = 1;
		int diameter = findDiameter();
		double L = L0 / (double)diameter;

		setLDist(L);
		setKDist();

		int maxDeltaIndex = -1;
		double maxDelta;
		double currentDelta;

		maxDeltaIndex = computeMaxDelta();
		maxDelta = computeCurrentDelta(maxDeltaIndex);
		int iter1 = 0;
		while (maxDelta > EPS)
		{
			int m = maxDeltaIndex;
			currentDelta = maxDelta;
			int iter2 = 0;
			while (currentDelta > EPS)
			{

				findSecondDiff(m);
				currentDelta = computeCurrentDelta(m);
				if (iter2 >= ITERATIONS_LIMIT2)break;
				iter2++;
				if (MORE_MOTION && ANIMATION){
					drawEverything();
				}
			}
			if (globflag == 0)return;
			maxDeltaIndex = computeMaxDelta();
			maxDelta = computeCurrentDelta(maxDeltaIndex);
			if (iter1 >= ITERATIONS_LIMIT1)break;
			iter1++;
			if (ANIMATION){
				drawEverything();
			}
		}
		if (ANIMATION){
			drawEverything();
		}

	}

	drawEverything();
	initiateImageStabilizers();

	superflag = 0;
	for (int i = 0; i <= N; i++){
		LDist[i].clear();
		KDist[i].clear();
		dist[i].clear();
	}

#pragma endregion abutton
#pragma region deleteLastCircle
	if (key == 8){
		//	glColor3f(BACKGROUND_RED / 255.0, BACKGROUND_GREEN / 255.0, BACKGROUND_BLUE / 255.0);
		if (x.empty())return;
		int px = x[x.size() - 1];
		int py = y[y.size() - 1];
		//	drawCircle(px, py);
		x.pop_back();
		y.pop_back();
		//glColor3f(DRAW_RED / 255.0, DRAW_GREEN / 255.0, DRAW_BLUE / 255.0);
		num = x.size();
		drawEverything();
	}
#pragma endregion deleteLastCircle

}

#pragma endregion EventHandlers

int main(int argc, char** argv)
{
	setGraph();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//	glutFullScreen();
	glutInitWindowPosition(0, 0);
	glClearColor(BACKGROUND_RED / 255.0, BACKGROUND_GREEN / 255.0, BACKGROUND_BLUE / 255.0, 1.0);
	glutCreateWindow("Algorithm 3");

	myInit();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);


	glutMainLoop();

	return 0;
}
