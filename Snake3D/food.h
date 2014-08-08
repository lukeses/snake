#include <iostream>
#include "model.h"
#include "bmp.h"

class Food {
private:
	Model* model;
	GLuint texture;
public:
	Food(int x,int y,Model* model,GLuint tex) {
		M = glm::translate(glm::mat4(1.0f),glm::vec3(2*x,0,2*y));
		this->model = model;
		texture = tex;
	}
	~Food() {
		model = NULL;
	}
	glm::mat4 M;
	void draw(ShaderProgram* shaderProgram);
};