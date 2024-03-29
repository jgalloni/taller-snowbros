/*
 * Sonido.h
 *
 *  Created on: 6/11/2014
 *      Author: manuel
 */

#ifndef SONIDO_H_
#define SONIDO_H_

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "WorldItem.h"
#include "../utiles/tipos.h"

class Sonido : public WorldItem{

public:
	sonidos_t sonido;
	ALfloat velocidad;

	unsigned int id;
	unsigned int source;
	int estado;
	bool loop;

	Sonido() {
		sonido = VACIO;
		id = 0;
		estado = AL_STOPPED;
		loop = false;
		source = 0;
		velocidad = 1.0f;
	}
	virtual ~Sonido() {
		eliminar();
	}

	void eliminar(){
		alDeleteBuffers(1, &id);
	}

	bool generar(std::string path){
		id= alutCreateBufferFromFile(path.c_str());
		if(id == AL_NONE)
			return false;
		return true;
	}

	bool reproducir() {
		alGenSources(1, &source);

		alSourcei(source, AL_BUFFER, id);
		alSourcef(source, AL_PITCH, velocidad);
		alSourcei(source, AL_LOOPING, loop);

		alSourcePlay(source);
	}

	bool pausar() {
		alSourcePause(source);
	}

	bool detener() {
		alSourceStop(source);
	}

	bool apagar() {
		alDeleteSources(1, &source);
	}

	void actualizarEstado(){
		alGetSourcei(source, AL_SOURCE_STATE, &estado);
		if( estado == AL_STOPPED ) apagar();
	}

	bool estaSonando(){
		if( estado == AL_PLAYING)
			return true;
		else return false;
	}

	std::string serializar(){
		std::string buffer;
		buffer = SSTR(SONIDO << " " << sonido);
		return buffer;
	}

};



#endif /* SONIDO_H_ */
