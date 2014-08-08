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
	//Program shaderów
	ShaderProgram* program;
	//Ile wierzcho³ków
	int count;
	//Uchwyty na VAO i bufory wierzcho³ków
	GLuint VAO;
	GLuint VBOVertices; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych wierzcho³ków
	//GLuint VBOColors;  //Uchwyt na bufor VBO przechowuj¹cy tablicê kolorów
	GLuint VBONormals; //Uchwyt na bufor VBO przechowuj¹cy tablicê wektorów normalnych
	GLuint VBOTexCoords; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzednych teksturowania
	GLuint setupVBO(void *data, int vertexCount, int vertexSize);
	void setupVAO();
	void assignVBOtoAttribute(char* attributeName, GLuint VBO, int variableSize);
	void freeVBO();
	void freeVAO();
};
#endif