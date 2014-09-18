/*
 * parametros.cpp
 *
 *  Created on: 15/09/2014
 *      Author: coco
 */

#include "parametros.h"

parametros::parametros() {
	posicion.x=0;
	posicion.y=0;
	angulo = 0;
	alto=0;
	ancho=0;
	escala=1;
	lados=0;
	tipo=CIRCULO;
	puntos=NULL;
	estatico=true;

}

parametros::parametros(const parametros &p) {
	this->posicion.x=p.posicion.x;
	this->posicion.y=p.posicion.y;
	this->angulo = p.angulo;
	this->alto=p.alto;
	this->ancho=p.ancho;
	this->escala=p.escala;
	this->lados=p.lados;
	this->tipo=p.tipo;
	this->puntos=p.puntos;
	this->estatico=p.estatico;

}

parametros::~parametros() {
	// TODO Auto-generated destructor stub
}

