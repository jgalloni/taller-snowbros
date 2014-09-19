#include <iostream>
#include "Window.h"
#include <Box2D/Box2D.h>
#include <list>
#include "formasbox2d/shapes.h"
#include "utiles/Logger.h"
#include "parser/parser.h"

#define OKEXIT 0
#define LOADERROR -1

using namespace std;

bool loadInitialValues(std::string& sConfig);
bool windowInit(int widthScreen, int heightScreen);
void worldInit(float w, float h);
bool loopPrincipal(list<shapes> tempo);
void close(list<shapes> tempo);

Window* w;
b2World *worldB2D;

int heightScreen;
int widthScreen;
float heightWorld;
float widthWorld;
float heightRatio = heightScreen / heightWorld;
float widthRatio = widthScreen / widthWorld;

int main() {
	std::string sConfig;
	bool statusOK = true;
	statusOK = loadInitialValues(sConfig);
	if (!statusOK) {
		return LOADERROR;
	}
	if (!parsingOk(sConfig)) {
		return LOADERROR;
	}
	heightScreen = get_node("alto-px", "escenario", sConfig, 480);
	widthScreen = get_node("ancho-px", "escenario", sConfig, 640);

	heightWorld = get_node("alto-un", "escenario", sConfig, 10.0f);
	widthWorld = get_node("ancho-un", "escenario", sConfig, 10.0f);

	heightRatio = heightScreen / heightWorld;
	widthRatio = widthScreen / widthWorld;

	worldInit(widthWorld, heightWorld);
	statusOK = windowInit(widthScreen, heightScreen);
	if (!statusOK) {
		return -1;
	}
	int formas = get_size("objetos", sConfig);
	if( formas == -1){
		return -1;
	}
	list<shapes> tempo;
	for (int i = 0; i < formas; i++) {
		shapes temp(sConfig, worldB2D, i, w);
		tempo.push_back(temp);
	}

	loopPrincipal(tempo);
	close(tempo);

	return OKEXIT;
}

bool loadInitialValues(std::string& sConfig) {
	Logger& log = *Logger::Instancia();
	//abre el json y lo carga a un string
	fstream fConfig;
	fConfig.open("config.json", ios_base::in);
	if (!fConfig.is_open()) {
		if (!log.abrirLog(MAINLOG)) {
			std::cout << "Error al abrir archivo de log " << MAINLOG
					<< std::endl;
			return false;
		}

		log.escribirLog("ERROR",
				"No se pudo encontrar o abrir el archivo config.json");
		log.cerrarLog();
		return false;
	}
	std::string newStr((std::istreambuf_iterator<char>(fConfig)),
			std::istreambuf_iterator<char>());
	sConfig = newStr;
	fConfig.close();
	return true;
}

bool windowInit(int widthScreen, int heightScreen) {
	w = new Window();
	bool statusOK = w->init(widthScreen, heightScreen, "imagenes/fondo2.png");
	b2BodyDef characterDef;
	characterDef.type=b2_dynamicBody;
	characterDef.position.x = 3;
	characterDef.position.y = 3;
	b2Body* character =worldB2D->CreateBody(&characterDef);
	b2FixtureDef myFixtureDef;
	b2PolygonShape poligon;
	poligon.SetAsBox(0.5,0.75); //le doy dimenciones
	myFixtureDef.shape = &poligon; //defino que es un poligono
	myFixtureDef.friction=3;
	character->CreateFixture(&myFixtureDef);
	character->SetFixedRotation(true);
	statusOK = w->insertarPersonaje(0, 0, 75, 50,character);
	return statusOK;
}

bool loopPrincipal(list<shapes> tempo) {
	SDL_Event event;
	bool quit = false;
	bool statusOK = true;

	while (!quit) {
		worldB2D->Step(1 / 60.0f, 2, 4);
		for (b2Body* b = worldB2D->GetBodyList(); b; b = b->GetNext()) {
			((Objeto*) b->GetUserData())->posicion(
					(b->GetPosition().x) * widthRatio,
					b->GetPosition().y * heightRatio * (-1) + heightScreen);
			((Objeto*) b->GetUserData())->angulo(
					-b->GetAngle() * 57.295779513082320876f);
		}
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
				break;
			} else if (event.key.keysym.sym == SDLK_r) {
				close(tempo);
				std::string sConfig;
				loadInitialValues(sConfig);
				int heightScreen = get_node("alto-px", "escenario", sConfig,
						480);
				int widthScreen = get_node("ancho-px", "escenario", sConfig,
						640);
				w = new Window();
				windowInit(widthScreen, heightScreen);
				int formas = get_size("objetos", sConfig);
					if( formas == -1){
						return -1;
					}
					for (int i = 0; i < formas; i++) {
						shapes temp(sConfig, worldB2D, i, w);
						tempo.push_back(temp);
					}
			} else {
				w->handleEvent(event);
			}
		}
		statusOK = w->updateWindow();
		if (!statusOK) {
			if (w) {
				delete w;
				w = NULL;
			}
			return -1;
		}
		SDL_Delay(25);
	}

	return statusOK;
}

void close(list<shapes> tempo) {
	if (w) {
		delete w;
		w = NULL;
	}
	if (worldB2D) {
		for (b2Body* b = worldB2D->GetBodyList(); b; b = b->GetNext()) {
			worldB2D->DestroyBody(b);
		}
		tempo.clear();
	}
}

void worldInit(float w, float h) {
	b2Vec2 gravedad(0, -9.8);
	worldB2D = new b2World(gravedad);
	//crear paredes piso y techo
	return;
}
