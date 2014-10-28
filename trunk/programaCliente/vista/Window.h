#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <list>

#include "../modelo/WorldItem.h"
//#include "modelo/Vidas.h"
#include "Fondo.h"

class Window {

private:
	SDL_Window* window;
	SDL_Renderer* wRenderer;
	SDL_GLContext ctx;
	Fondo * background;
	//Vidas * vidas;

	//Escenario* wEscenario;
	//HandlerDeEventos wHandlerEventos;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	bool error;

	SDL_Surface* resizeSurface(SDL_Surface* surface, int t_height, int t_width);

	bool validarAnchoYAlto(int ancho, int alto);
	bool iniciarSDL();
	SDL_Window* crearVentana();
	SDL_Renderer* crearRenderer(SDL_Window*);
	SDL_GLContext initGL(SDL_Window* w);

public:

	static float wRatio, hRatio;

	// Constructor de la clase
	Window();
	// Cargar ventana
	bool init(int width, int height, std::string BGpath);
	// Cargar el fondo
	bool loadBackground(const char* pathToBG);
	// Inserta un personaje
	//Personaje * insertarPersonaje(float32 height, float32 width);
	// Inserta una figura
	//void insertarFigura(IDibujable* figura);
	// manejar evento
	//void handleEvent(SDL_Event&);
	// Update ventana
	bool updateWindow(std::list<WorldItem*> & itemList, float escala);
	// Destructor de la clase
	virtual ~Window();
};

#endif
