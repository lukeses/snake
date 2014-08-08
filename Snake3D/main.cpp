#include "main.h"
#include <ctime>

//Procedura rysuj¹ca
void displayFrame() {
	//Wyczyœæ bufor kolorów i bufor g³êbokoœci
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	matP=glm::perspective(80.0f, (float)windowWidth/(float)windowHeight, 0.1f, 200.0f);
	
	//Wylicz macierz widoku dla kamery dynamicznej(zza g³owy)
	if (dynamicCamera) {
		glm::vec4 matrix = head->M*glm::vec4(0, 0, 0, 1);
		glm::vec3 headPosition = glm::vec3(matrix.x, matrix.y+0.5, matrix.z);
		matrix = second->M*glm::vec4(0, 0, 0, 1);
		glm::vec3 camera = glm::vec3(matrix.x, 2.0+camPos, matrix.z);
		matV = glm::lookAt(camera, headPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	//Kamera od góry, domyœlna
	else matV = glm::lookAt(glm::vec3(0.0, 15.0 + camPos,0.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(matP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(matV));
	glUniform4f(shaderProgram->getUniformLocation("lightPosition"), 0, 10, 0, 1);
	//Co rysujemy, glowe, czesc snake'a czy ogon
	for (unsigned int i = 0; i<snakeList.size(); i++) snakeList[i]->draw(shaderProgram, ((head == snakeList[i]) ? snakeHead : (snakeList[i]==snakeList[snakeList.size()-1])?snakeTail:snakeModel),((head == snakeList[i]) ? snakeHeadID : snakeSkinID));
	if (food != NULL) food->draw(shaderProgram);
	//Tylny bufor na przedni
	glutSwapBuffers();
}

void update(int flags) {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastCheck >= snakeSpeed) { //Update co okreslona predkosc
		updateSnake();
		lastCheck = currentTime;
		if (leftKey) leftKey = false;
		if (rightKey) rightKey = false;
	}
	if (food == NULL) {
		cout << "Nowe jedzenie.\n";
		food = new Food(rand() % 5- rand() % 5, rand() % 5 - rand() % 5, foodModel, foodSkinID); //Losowa pozycja jedzenia
	}
	else {
		food->M = glm::rotate(food->M, 1.0f, glm::vec3(0.0, 1.0, 0.0)); //Krecimy jedzeniem
		glm::vec4 foodPos; //obliczamy kolizje z jedzeniem, a raczej patrzymy czy wystapila
		glm::vec4 snakePos;
		snakePos = head->M*glm::vec4(0, 0, 0, 1); //mnozenie macierzy M razy vec4 0,0,0,1 zwraca pozycje x,y,z w przestrzeni swiata
		foodPos = food->M*glm::vec4(0, 0, 0, 1);
		if (abs(foodPos.x - snakePos.x) < 1.0 && abs(foodPos.z - snakePos.z) < 1.0) {
			coughtFood = true;
			delete food;
			food = NULL;
		}
		glm::vec4 partPos;
		glm::vec4 headPos;
		headPos = head->M*glm::vec4(0, 0, 0, 1);
		for (unsigned int i = 3; i < snakeList.size(); i++) { //Patrzymy czy snake nie zjada siebie
			partPos = snakeList[i]->M*glm::vec4(0, 0, 0, 1);
			if (abs(partPos.x - headPos.x) < 1.0 && abs(partPos.z - headPos.z) < 1.0) { //jezeli tak to usuwam ogon odgryziony
				for (unsigned int j = snakeList.size() - 1; j >= i; j--) {
					delete snakeList[j];
					snakeList.pop_back();
				}
				break;
			}
		}
		
	}
	
	int interval = currentTime - lastUpdateTime; //Ustawiamy ostatni czas update'a
	lastUpdateTime = currentTime;
	glutTimerFunc(16, update, 0);
	glutPostRedisplay();
}

//MAIN
int main(int argc, char** argv) {
	initGLUT(&argc,argv);
	initGLEW();
	initOpenGL();
	if (!initAssets()) {
		system("pause");
		cleanShaders();
		return -1;
	}
	initSnake();

	glutTimerFunc(16, update, 0);
	glutMainLoop();

	clearSnake();
	glDeleteTextures(1, &snakeSkinID);
	glDeleteTextures(1, &snakeHeadID);
	glDeleteTextures(1, &foodSkinID);
	cleanShaders();
	return 0;
}

void keyDown(unsigned char c, int x, int y) {

}

void keyUp(unsigned char c, int x, int y) { //Obsluga roznych klawiszy
	switch (c) {
	case 'a': if (!leftKey) { leftKey = true; rightKey = false; head->M = glm::rotate(head->M, 90.0f, glm::vec3(0.0, 1.0, 0.0)); } break;
	case 'd': if (!rightKey) { rightKey = true; leftKey = false; head->M = glm::rotate(head->M, -90.0f, glm::vec3(0.0, 1.0, 0.0)); } break;
	case 'f': coughtFood = true; break;
	case '=':
	case '+': if (camPos > -5.0f) camPos -= 0.5; if (dynamicCamera && camPos < -1.0f) camPos = -1.0f; cout << "Zoom: " << camPos << endl; break;
	case '-': if (camPos < 0) camPos += 0.5; cout << "Zoom: " << camPos << endl; break;
	case 'c': dynamicCamera = !dynamicCamera; if (dynamicCamera && camPos < -1.0f) camPos = -1.0f; cout << "Przelaczono kamere.\n";  break;
	case 'q': if (snakeSpeed<1500) snakeSpeed += 100; cout << "Nowa predkosc: " << snakeSpeed << endl; break;
	case 'e': if (snakeSpeed>100) snakeSpeed -= 100; cout << "Nowa predkosc: " << snakeSpeed << endl; break;
	default: break;
	}
}


//Procedura wywo³ywana przy zmianie rozmiaru okna
void changeSize(int w, int h) {
	//Ustawienie wymiarow przestrzeni okna
	glViewport(0, 0, w, h);
	//Zapamiêtanie nowych wymiarów okna dla poprawnego wyliczania macierzy rzutowania
	windowWidth = w;
	windowHeight = h;
}

//Procedura inicjuj¹ca biblotekê glut
void initGLUT(int *argc, char** argv) {
	glutInit(argc, argv); //Zainicjuj bibliotekê GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //Alokuj bufory kolorów (podwójne buforowanie) i bufor kolorów

	glutInitWindowPosition(windowPositionX, windowPositionY); //Wska¿ pocz¹tkow¹ pozycjê okna
	glutInitWindowSize(windowWidth, windowHeight); //Wska¿ pocz¹tkowy rozmiar okna
	glutCreateWindow("OpenGL 3.3"); //Utwórz okno i nadaj mu tytu³

	glutReshapeFunc(changeSize); //Zarejestruj procedurê changeSize jako procedurê obs³uguj¹ca zmianê rozmiaru okna
	glutDisplayFunc(displayFrame); //Zarejestruj procedurê displayFrame jako procedurê obs³uguj¹ca odœwierzanie okna
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
}


//Procedura inicjuj¹ca bibliotekê glew
void initGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		exit(1);
	}

}

//procedura inicjuj¹ca ró¿ne sprawy zwi¹zane z rysowaniem w OpenGL
void initOpenGL() {
	setupShaders();
	glEnable(GL_DEPTH_TEST);
}

//Wczytuje vertex shader i fragment shader i ³¹czy je w program cieniuj¹cy
void setupShaders() {
	shaderProgram = new ShaderProgram("vshader.txt", NULL, "fshader.txt");
	shaderProgram->use();
}

//Zwolnij pamiêæ karty graficznej z shaderów i programu cieniuj¹cego
void cleanShaders() {
	delete shaderProgram;
}

bool initAssets() { //£adowanie modeli i tekstur
	snakeModel = new Model(shaderProgram);
	foodModel = new Model(shaderProgram);
	snakeHead = new Model(shaderProgram);
	snakeTail = new Model(shaderProgram);
	if (!snakeModel->loadFile("data/snakePart.obj")) return false;
	if (!snakeTail->loadFile("data/snakeTail.obj")) return false;
	if (!snakeHead->loadFile("data/snakeHead.obj")) return false;
	if (!snakeSkin.load("data/skin.bmp", false)) return false;
	if (!snakeHeadSkin.load("data/head.bmp", false)) return false;
	if (!foodModel->loadFile("data/food.obj")) return false;
	if (!foodSkin.load("data/food.bmp", false)) return false;
	snakeSkinID = initTexture(&snakeSkin);
	snakeHeadID = initTexture(&snakeHeadSkin);
	foodSkinID = initTexture(&foodSkin);
	return true;
}
GLuint initTexture(BMP* texture) { //Przygotowanie tekstury do u¿ytku
	GLuint ID = 0;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGB, texture->width, texture->height,
		0, GL_BGR, GL_UNSIGNED_BYTE,
		texture->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	return ID;
}
void initSnake() { //Inicjalizacja snejka
	snakeSpeed = 1000;
	srand((int)time(NULL));
	food = NULL;
	camPos = 0.0f;
	dynamicCamera = false;
	snakeList.push_back(new SnakePart()); //Dodaje g³owê
	head = snakeList[0];
	head->M = glm::mat4(1.0f);
	for (int i = 1; i <= 3; i++) { //i trzy pierwsze czesci
		snakeList.push_back(new SnakePart());
		snakeList[i]->M = glm::translate(snakeList[i - 1]->M, glm::vec3(2, 0.0, 0.0));
	}
	second = snakeList[1];
	coughtFood = false;
}
void clearSnake() {
	if (food != NULL) delete food;
	for (unsigned int i = 0; i < snakeList.size(); i++) delete snakeList[i]; //Usuwanie snake'a
	snakeList.clear();
	delete snakeHead;
	delete snakeTail;
	delete snakeModel;
}
void updateSnake() { //Update snejka
	//Dzia³am tu na takim stworku o nazwie dequeue, czyli double ended queue. Idealna do snejka. Usuwam element z ty³u i dodaje na przód. Reszta bez zmian.
	SnakePart* tmp;
	if (!coughtFood) {
		tmp = snakeList[snakeList.size() - 1];
		snakeList.pop_back();
	}
	else {
		tmp = new SnakePart();
		coughtFood = false;
	}
	tmp->M = glm::translate(head->M, glm::vec3(-2, 0.0, 0.0)); //Standardowy odstêp pomiedzy partami snake'a to 2 jednostki blenderowe
	snakeList.push_front(tmp);
	second = head;
	head = tmp;
}