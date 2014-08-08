#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "shaders.h"
#include "model.h"
#include "bmp.h"
#include "snakePart.h"
#include "food.h"
#include <deque>
using namespace std;

int lastCheck;
int lastUpdateTime;


//Macierze
glm::mat4  matP;//rzutowania
glm::mat4  matV;//widoku
float camPos;
bool dynamicCamera;

float snakeSpeed;

//Ustawienia okna i rzutowania
int windowPositionX = 283;
int windowPositionY = 20;
int windowWidth = 800;
int windowHeight = 600;
float cameraAngle = 90.0f;

//Uchwyty na shadery
ShaderProgram *shaderProgram; //Wskaünik na obiekt reprezentujπcy program cieniujπcy.

deque<SnakePart*> snakeList;
Model* snakeModel;
Model* snakeHead;
Model* snakeTail;
BMP snakeSkin;
BMP snakeHeadSkin;
GLuint snakeSkinID;
GLuint snakeHeadID;
SnakePart* head;
SnakePart* second;

Model* foodModel;
BMP foodSkin;
GLuint foodSkinID;

Food* food;

bool leftKey, rightKey,coughtFood;


void displayFrame();
void update(int flags);
void keyDown(unsigned char c, int x, int y);
void keyUp(unsigned char c, int x, int y);
void changeSize(int w, int h);
void initGLUT(int *argc, char** argv);
void initGLEW();
void initOpenGL();
bool initAssets();
void initSnake();
void setupShaders();
void cleanShaders();
GLuint initTexture(BMP* texture);
void clearSnake();
void updateSnake();