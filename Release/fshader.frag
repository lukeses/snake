#version 330

in vec4 outLight;
in vec4 outNormal;
in vec4 outVertex;
in vec2 outTexCoord;

uniform sampler2D textureMap;

void main(void) {

	//Parametry modelu oswietlenia - normalnie powinny byc przekazane jako zmienne jednorodne, ale dla uproszczenia sa zakodowane na twardo
	vec4 La=vec4(0,0,0,0); //Kolor swiatla otoczenia
	vec4 Ma=vec4(0,0,0,0); //Kolor materialu dla swiatla otoczenia

	vec4 Ld=vec4(1,1,1,1); //Kolor swiatla rozpraszanego
	vec4 Md=texture(textureMap,outTexCoord); //Kolor materialu dla swiatla rozpraszanego

	float shininess = 50;
	//Wektory potrzebne do obliczenia modelu oswietlenia
	vec4 ml=normalize(outLight);
	vec4 mn=normalize(outNormal);
	vec4 mv=normalize(outVertex);
	vec4 mr=reflect(-ml,mn);

	//Obliczenie modelu oswietlenia
	float nl=max(dot(ml,mn),0);
	float rv=pow(max(dot(mr,mv),0),shininess);
	
	gl_FragColor=vec4(1,1,1,1)*texture(textureMap,outTexCoord);//max(vec4(0.02,0.02,0.02,0.2),Ld*Md*vec4(nl,nl,nl,1)+vec4(1,1,1,1)*vec4(rv,rv,rv,0));
}
