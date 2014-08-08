#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lightPosition; //wspolrzedne homogeniczne swiatla w przestrzeni swiata

//Atrybuty
in vec3 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec3 color;  //kolor wierzcholka (na razie ignorowany)
in vec3 normal; //wektor normalny w wierzcholku (na razie ignorowany) - przestrzen modelu
in vec2 texCoord; //wspolrzedne teksturowania

out vec2 outTexCoord;
out vec4 outLight;
out vec4 outNormal;
out vec4 outVertex;

void main(void) {
	outLight = normalize(V*(lightPosition-M*(vec4(vertex,1.0))));
	outNormal = normalize(V*M*vec4(normal,0.0));
	outVertex = normalize(-V*M*vec4(vertex,0.0));
	outTexCoord=texCoord;
	gl_Position=P*V*M*vec4(vertex,1.0);
}