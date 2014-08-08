#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL/glew.h"
#include "stdio.h"

class ShaderProgram {
private:
	GLuint shaderProgram; //Uchwyt reprezentuj�cy program cieniujacy
	GLuint vertexShader; //Uchwyt reprezentuj�cy vertex shader
	GLuint geometryShader; //Uchwyt reprezentuj�cy geometry shader
	GLuint fragmentShader; //Uchwyt reprezentuj�cy fragment shader
	char* readFile(char* fileName); //metoda wczytuj�ca plik tekstowy do tablicy znak�w
	GLuint loadShader(GLenum shaderType,char* fileName); //Metoda wczytuje i kompiluje shader, a nast�pnie zwraca jego uchwyt
public:
	ShaderProgram(char* vertexShaderFile,char* geometryShaderFile,char* fragmentShaderFile);
	~ShaderProgram();
	void use(); //W��cza wykorzystywanie programu cieniuj�cego
	GLuint getUniformLocation(char* variableName); //Pobiera numer slotu zwi�zanego z dan� zmienn� jednorodn�
	GLuint getAttribLocation(char* variableName); //Pobiera numer slotu zwi�zanego z danym atrybutem
};



#endif