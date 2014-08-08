#ifndef MODEL_H
#define MODEL_H
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "shaders.h"
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

class Model {
public:
	Model(ShaderProgram* program);
	~Model();
	bool loadFile(string filepath);
	GLuint getVAO();
	int getCount();
private:
	//Program shader�w
	ShaderProgram* program;
	//Ile wierzcho�k�w
	int count;
	//Uchwyty na VAO i bufory wierzcho�k�w
	GLuint VAO;
	GLuint VBOVertices; //Uchwyt na bufor VBO przechowuj�cy tablic� wsp�rz�dnych wierzcho�k�w
	//GLuint VBOColors;  //Uchwyt na bufor VBO przechowuj�cy tablic� kolor�w
	GLuint VBONormals; //Uchwyt na bufor VBO przechowuj�cy tablic� wektor�w normalnych
	GLuint VBOTexCoords; //Uchwyt na bufor VBO przechowuj�cy tablic� wsp�rzednych teksturowania
	GLuint setupVBO(void *data, int vertexCount, int vertexSize);
	void setupVAO();
	void assignVBOtoAttribute(char* attributeName, GLuint VBO, int variableSize);
	void freeVBO();
	void freeVAO();
};
#endif