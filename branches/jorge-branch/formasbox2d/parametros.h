/*
 * parametros.h
 *
 *  Created on: 15/09/2014
 *      Author: coco
 */

#ifndef PARAMETROS_H_
#define PARAMETROS_H_
#include <iostream>
#include "shapes.h"
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

class parametros {
public:
	parametros();
	parametros(const parametros &p);
	virtual ~parametros();
	struct posicion{ float x;float y;}posicion;
	float angulo;
	float alto;
	float ancho;
	float escala;

	int heightScreen;
	int widthScreen;
	float heightRatio;
	float widthRatio;
	float heightWorld;
	float widthWorld;
	int lados;
	int tipo;
	b2Vec2 *puntos;
	bool estatico;
	SDL_Color color;


};

#endif /* PARAMETROS_H_ */
