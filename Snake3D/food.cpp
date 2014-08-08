#include "food.h"

void Food::draw(ShaderProgram* shaderProgram) {
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(M));
	glUniform1i(shaderProgram->getUniformLocation("textureMap"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi¹zañ slotów atrybutów z tablicami z danymi
	glBindVertexArray(model->getVAO());

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES, 0, model->getCount());

	//Posprz¹tanie po sobie (niekonieczne w sumie je¿eli korzystamy z VAO dla ka¿dego rysowanego obiektu)
	glBindVertexArray(0);
}