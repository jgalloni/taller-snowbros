/*
 * ConnectionHandler.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: matias
 */

#include "ConnectionHandler.h"
#include "../utiles/tipos.h"
#include "../utiles/Timer.h"
#include <iostream>

ConnectionHandler::ConnectionHandler(ControladorUsuarios & c, TCPStream * stream) :
	controlador(c), m_stream(stream){}

/*void rechazarConexion(){

	//TODO: implementar.
	std::cout << "conexion rechazada" << std::endl;

}*/

bool ConnectionHandler::logIn(std::string username){

	// Si el usuario ya se habia loggeado durante la partida, lo reconecta.
	if (controlador.usuarioExiste(username)){
		Usuario* user = controlador.obtenerUsuario(username);
		if(user->online) {
			return false;
		} else {
			user->setOnline(true);
		}

	// En caso contrario, si hay lugares vacantes en el escenario, lo agrega
	// como un nuevo PJ.
	} else {
		if (controlador.escenarioLleno()) return false;
		controlador.registrarUsuario(username);
	}
	return true;
}

void* ConnectionHandler::run() {
	// Remove 1 item at a time and process it. Blocks if no items are
	// available to process.
	printf("Conexion con: %s:%d establecida.\n", m_stream->getPeerIP().c_str() , m_stream->getPeerPort());

	//The frames per second timer
	Timer fpsTimer;
	//Start counting frames per second
	int countedFrames = 0;
	fpsTimer.start();

	// Inicia la comunicacion recibiendo el nombre de usuario e intentando conectarlo.
	m_stream->receive(username);
	if (!logIn(username)) {
		std::string outMsg = "RECHAZADA";
		size_t len = m_stream->send(outMsg);
		printf ("Conexion rechazada %s:%d \n", m_stream->getPeerIP().c_str(), m_stream->getPeerPort());
		//delete m_stream;
		return NULL;
	}else{
		std::string outMsg = "OK";
		size_t len = m_stream->send(outMsg);
		printf ("Conexion aceptada %s:%d \n", m_stream->getPeerIP().c_str(), m_stream->getPeerPort());
	}

	ssize_t len;
	bool quit = false;
	std::string inMessage, outMessage;

	while (!quit){

		// Recibe todos los eventos sucedidos en el cliente.
		inMessage = "NOTDONE";
		while (inMessage != "DONE"){
			len = m_stream->receive(inMessage);
			if (len <= 0) {
				quit = true;
				break;
			}
			if (inMessage != "DONE") {
				teclas_t IDEvento = (teclas_t) atol(inMessage.c_str());
				controlador.obtenerUsuario(username)->encolarNotificacion(IDEvento);
			}
		}
		if (quit) break;

		// Obtiene la representacion serializada de la pantalla, y la envia.
		outMessage =
				controlador.obtenerUsuario(username)->obtenerPantallaSerializada();

		if (outMessage.empty()) outMessage = "EMPTY";
		len = m_stream->send(outMessage);
		if (len <= 0) {
			quit = true;
			break;
		}

		// Calcula un 'promedio' de FPS.
		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if (avgFPS > 2000) {
			fpsTimer.stop();
			fpsTimer.start();
			countedFrames = 0;
		}
		std::cout << avgFPS << " FPS avg" << std::endl;
		++countedFrames;

		controlador.obtenerUsuario(username)->enviarSenial();
	}

	controlador.obtenerUsuario(username)->setOnline(false);
	controlador.obtenerUsuario(username)->enviarSenial();

	printf("Conexion con: %s:%d terminada.\n", m_stream->getPeerIP().c_str() , m_stream->getPeerPort());

	delete m_stream;

	return NULL;
}
