#include "Window.h"
#include <iostream>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_image.h>
#include "ItemRenderer.h"

float Window::wRatio, Window::hRatio;


Window::Window() {
	window = NULL;
	//wEscenario = NULL;
	error = false;
	wRenderer = NULL;
	SCREEN_WIDTH = 480;
	SCREEN_HEIGHT = 640;
	wRatio = hRatio = 1;
	background = NULL;
}

bool Window::init(int width, int height, std::string BGpath){
	if( !validarAnchoYAlto( width, height ) ){
		return !error;
	}
	if (!iniciarSDL()){
		return !error;
	}
	else{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		 window = crearVentana();
		if( !window ){
			 return !error;
		}
		else{
			wRenderer = crearRenderer(window);
			if( !wRenderer ) {
				error = true;
				return !error;
			}

			ctx = initGL(window);

			if( TTF_Init() < 0){
				printf("error al abrir ttf\n");
				return !error;
			}

			background = new Fondo(width, height);
			background->setRenderer(wRenderer);
			background->cargarImagen(BGpath);
		}
	}

	return !error;
}

bool Window::updateWindow(std::list<WorldItem*> & itemList, float escala){
	Logger& log = * Logger::Instancia();
	if(SDL_SetRenderDrawColor(wRenderer, 0x00, 0x00, 0x00, 0x00) != 0) {
		log.log(WINDOWLOG,ERROR,"Error al settear color de dibujo de renderer 'RenderDrawColor'");
		return false;
	}

	if(SDL_RenderClear(wRenderer) != 0 ) {
		log.log(WINDOWLOG,ERROR,"Error al querer limpiar el Render 'RenderClear'");
		return false;
	}

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//background->render();

	// Dibuja uno por uno los elementos en la lista.
	while(!itemList.empty()){
		WorldItem * item = itemList.front();
		itemList.pop_front();
		ItemRenderer::render(wRenderer, item, escala);
		delete item;
		item = NULL;
	}

	SDL_GL_SwapWindow(window);

	SDL_RenderPresent(wRenderer);

	return !error;
}


bool Window::validarAnchoYAlto(int width, int height)
{
	Logger& log = * Logger::Instancia();

	if (height > 0 && width > 0)
	{
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
	}
	else
	{
		if (!log.abrirLog(WINDOWLOG))
		{
			std::cout << "Error al abrir archivo de log " << WINDOWLOG << std::endl;
			return false;
		}

		log.escribirLog(ERROR, "Altura y/o ancho menor a 0");
		log.cerrarLog();
		error = true;
		return !error;
	}
	return !error;
}

bool Window::iniciarSDL()
{
	Logger& log = * Logger::Instancia();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		if (!log.abrirLog(WINDOWLOG)) {
			std::cout << "Error al abrir archivo de log " << WINDOWLOG << std::endl;
			return false;
		}
		std::string buf ("No se pudo inicializar SDL!");
		buf = buf + SDL_GetError();
		log.escribirLog(ERROR, buf);
		log.cerrarLog();
		error = true;
	}else{
		//Inicio la carga de imagenes
		int imgFlags = IMG_INIT_PNG;
		if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
			if (!log.abrirLog(WINDOWLOG)) {
				std::cout << "Error al abrir archivo de log " << WINDOWLOG << std::endl;
				return false;
			}
			std::string buf ("SDL_image no pudo iniciar! SDL_image Error: ");
			buf = buf + IMG_GetError();
			log.escribirLog(ERROR, buf);
			log.cerrarLog();
			error = true;
		}
	}
	return !error;
}

SDL_Window* Window::crearVentana()
{
	Logger& log = * Logger::Instancia();
	SDL_Window* w = SDL_CreateWindow("Snow Bros", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(!w) {
		if (!log.abrirLog(WINDOWLOG)) {
			std::cout << "Error al abrir archivo de log " << WINDOWLOG << std::endl;
			return w;
		}
		std::string buf("No se pudo crear la ventana. ");
		buf = buf + SDL_GetError();
		log.escribirLog(ERROR, buf);
		log.cerrarLog();
		error = true;
	}
	return w;
}

SDL_Renderer* Window::crearRenderer(SDL_Window* w)
{
	Logger& log = * Logger::Instancia();
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer( w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( !renderer )
	{
		if (!log.abrirLog(WINDOWLOG)) {
			std::cout << "Error al abrir archivo de log " << WINDOWLOG << std::endl;
			return renderer;
		}
		log.escribirLog(ERROR, "No se pudo crear el renderer!");
		log.cerrarLog();
		SDL_DestroyWindow(window);
		return renderer;
	}
	else SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	return renderer;
}
/*
void Window::insertarFigura(IDibujable* figura){
	figura->setRenderer(wRenderer);
}*/

Window::~Window(){

	std::cout << "destruyendo ventana" << std::endl;
	if(wRenderer) {
		SDL_DestroyRenderer(wRenderer);
		wRenderer = NULL;
	}
	if(ctx)
		SDL_GL_DeleteContext(ctx);
	if(window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();
}

SDL_GLContext Window::initGL(SDL_Window* w) {

	SDL_GLContext c = SDL_GL_CreateContext(w);
	SDL_GL_MakeCurrent(w, c);

	SDL_GL_SetSwapInterval(1);

// Set the OpenGL state after creating the context with SDL_SetVideoMode
	glClearColor(0, 0, 0, 1);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
	glMatrixMode( GL_MODELVIEW);

	glLoadIdentity();

	return c;
}