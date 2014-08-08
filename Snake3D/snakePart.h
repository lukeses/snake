#ifndef SNAKE_H
#define SNAKE_H
#include <iostream>
#include "model.h"
#include "bmp.h"

class SnakePart {
public:
	glm::mat4 M;
	void draw(ShaderProgram* shaderProgram,Model* model,GLuint skin);
};
#endif