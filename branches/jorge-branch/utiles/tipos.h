/*
 * tipos.h
 *
 *  Created on: 2/9/2014
 *      Author: manuel
 */

#ifndef TIPOS_H_
#define TIPOS_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../modelo/interfaces/IDibujable.h"
#include "../control/interfaces/IObservador.h"

typedef std::vector<IObservador*> ListaObservadores;
typedef std::vector<IDibujable*> ListaDibujables;

class Tamano {
public:
	Tamano();
	inline Tamano(float alto, float ancho) : tAlto(alto) , tAncho(ancho) {};
	virtual ~Tamano(){};

	inline float alto(){
		return tAlto;
	}

	inline void alto(float a){
		tAlto = a;
	}

	inline float ancho(){
		return tAncho;
	}

	inline void ancho(float a){
		tAncho = a;
	}

	inline void operator=(Tamano otroTamano){
		tAlto = otroTamano.alto();
		tAncho = otroTamano.ancho();
	}

private:
	float tAlto, tAncho;
};

enum TECLAS
{
	ARRIBA,
	ABAJO,
	IZQUIERDA,
	DERECHA,
	RESTART, // RESTART = tecla r
};

#endif /* TIPOS_H_ */
