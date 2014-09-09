#include "Window.h"
#include <iostream>
//#include "SDL_image.h"

#include <SDL2/SDL_image.h>

Window2::Window2() {
	window = NULL;
	wSurface = NULL;
	error = false;
	SCREEN_WIDTH = 0;
	SCREEN_HEIGHT = 0;
	BGimage = NULL;
}

bool Window2::init(int width, int height) {
	if (height > 0 && width > 0){
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
	}
	else {
		// TODO: Error hangling, log
		error = true;
		return error;
	}
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "No se pudo inicializar SDL!" << std::endl;
		// TODO: Catch Error and handle log.
		error = true;
	}
	else {
		 window = SDL_CreateWindow("Snow Bros", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(!window) {
			// TODO: Catch Error and handle log.
			std::cout << "No se pudo crear la ventana." << std::endl;
			error = true;
		}
		else {
			wSurface = SDL_GetWindowSurface(window);
		}
	}
	return !error;

}

bool Window2::updateWindow() {
	 if (SDL_UpdateWindowSurface(window) != 0 ) {
		 // TODO: Catch Error and handle log.
		 error = true;
	 }
	 return !error;
}

bool Window2::loadBackground(const char* pathToBG) {
	BGimage = IMG_Load(pathToBG);
	if (!BGimage) {
		// TODO: Catch Error and handle log.
		error = true;
		return error;
	}
	SDL_BlitSurface(BGimage, NULL, wSurface, NULL);
	return !error;
}

Window2::~Window2(){
	if(BGimage) {
		SDL_FreeSurface(BGimage);
		BGimage = NULL;
	}
	if(window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();
}
