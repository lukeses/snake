#include "model.h"
#include "shaders.h"

Model::Model(ShaderProgram* program) {
	this->program = program;
	count = 0;
}
Model::~Model() {
	program = NULL;
	freeVAO();
	freeVBO();
}
bool Model::loadFile(string filepath) {
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> texCoords;
	ifstream modelData;
	//£adowanie modelu
	modelData.open(filepath.c_str());
	if (modelData.good()) {
		cout << "Ladowanie modelu \"" + filepath + "\"...\n";
		string line;
		vector<glm::vec3> vertices_tmp;
		vector<glm::vec3> normals_tmp;
		vector<glm::vec2> texture_tmp;
		while (modelData >> line) {
			if (line == "v") {
				float x, y, z;
				modelData >> x >> y >> z;
				vertices_tmp.push_back(glm::vec3(x, y, z));
			}
			else if (line == "vn") {
				float x, y, z;
				modelData >> x >> y >> z;
				normals_tmp.push_back(glm::vec3(x, y, z));
			}
			else if (line == "vt") {
				float x, y;
				modelData >> x >> y;
				texture_tmp.push_back(glm::vec2(x, y));
			}
			else if (line == "f") {
				string v1, v2, v3;
				modelData >> v1 >> v2 >> v3;

				vertices.push_back(vertices_tmp[atoi(v1.substr(0, v1.find("/")).c_str()) - 1]);
				vertices.push_back(vertices_tmp[atoi(v2.substr(0, v2.find("/")).c_str()) - 1]);
				vertices.push_back(vertices_tmp[atoi(v3.substr(0, v3.find("/")).c_str()) - 1]);

				v1 = v1.substr(v1.find("/") + 1, v1.length());
				v2 = v2.substr(v2.find("/") + 1, v2.length());
				v3 = v3.substr(v3.find("/") + 1, v3.length());

				texCoords.push_back(texture_tmp[atoi(v1.substr(0, v1.find("/")).c_str()) - 1]);
				texCoords.push_back(texture_tmp[atoi(v2.substr(0, v2.find("/")).c_str()) - 1]);
				texCoords.push_back(texture_tmp[atoi(v3.substr(0, v3.find("/")).c_str()) - 1]);

				v1 = v1.substr(v1.find("/") + 1, v1.length());
				v2 = v2.substr(v2.find("/") + 1, v2.length());
				v3 = v3.substr(v3.find("/") + 1, v3.length());

				normals.push_back(normals_tmp[atoi(v1.c_str()) - 1]);
				normals.push_back(normals_tmp[atoi(v2.c_str()) - 1]);
				normals.push_back(normals_tmp[atoi(v3.c_str()) - 1]);
			}
		}
		cout << "Zaladowano model \"" << filepath << "\".\n";
		count = vertices.size();
		VBOVertices = setupVBO(&vertices[0], count*3, sizeof(float));
		VBONormals = setupVBO(&normals[0], count*3, sizeof(float));
		VBOTexCoords = setupVBO(&texCoords[0], count*2, sizeof(float));
		setupVAO();
		return true;
	}
	else {
		cout << "Brak modelu \""+filepath+"\"!\n";
		return false;
	}
}

GLuint Model::getVAO() {
	return VAO;
}
int Model::getCount() {
	return count;
}
GLuint Model::setupVBO(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}
void Model::setupVAO() {
	//Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
	glGenVertexArrays(1, &VAO);

	//Uaktywnij nowo utworzony VAO
	glBindVertexArray(VAO);

	assignVBOtoAttribute("vertex", VBOVertices, 3); //"vertex" odnosi siê do deklaracji "in vec4 vertex;" w vertex shaderze
	//assignVBOtoAttribute("color", VBOColors, 3); //"color" odnosi siê do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute("normal", VBONormals, 3); //"normal" odnosi siê do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute("texCoord", VBOTexCoords, 2); //"texCoord" odnosi siê do deklaracji "in vec2 texCoord;" w vertex shaderze

	glBindVertexArray(0);
}
void Model::assignVBOtoAttribute(char* attributeName, GLuint VBO, int variableSize) {
	GLuint location = program->getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location, variableSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}
void Model::freeVBO() {
	glDeleteBuffers(1, &VBOVertices);
	//glDeleteBuffers(1, &VBOColors);
	glDeleteBuffers(1, &VBONormals);
	glDeleteBuffers(1, &VBOTexCoords);
}
void Model::freeVAO() {
	glDeleteVertexArrays(1, &VAO);
}