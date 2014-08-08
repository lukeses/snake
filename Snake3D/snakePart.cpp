#include "snakePart.h"

void SnakePart::draw(ShaderProgram* shaderProgram, Model* model, GLuint skin) {
	//Zbinduj macierz M cz�ci snake'a do aktualnie rysowanej cz�ci
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(M));
	//zbinduj tekstur� do shadera
	glUniform1i(shaderProgram->getUniformLocation("textureMap"), 0);

	//Aktywacja tekstury 0 i przypisanie do niej skinu snake'a
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skin);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi�za� slot�w atrybut�w z tablicami z danymi
	glBindVertexArray(model->getVAO());

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES, 0, model->getCount());

	//Posprz�tanie po sobie (niekonieczne w sumie je�eli korzystamy z VAO dla ka�dego rysowanego obiektu)
	glBindVertexArray(0);
}