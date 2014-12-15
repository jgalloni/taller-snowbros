/*
 * Jugador.h
 *
 *  Created on: Dec 5, 2014
 *      Author: rodrizapico
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include <string>
#include "../ObjetoJuego.h"
#include "../../servicios/LocalizadorDeServicios.h"
#include "../../vista/elementosMenues/TextoGUI.h"
#include "../../vista/elementosMenues/ImagenEstaticaGUI.h"
#include "../../utiles/tipos.h"
#include "../../parserMensajes/protocoloServidorCliente.h"

class Jugador: public ObjetoJuego {
public:

	// Crea un nuevo jugador.
	Jugador();

	// Destruye un jugador.
	virtual ~Jugador();

	// Crea un jugador nuevo en base a los datos serializados.
	static Jugador * deserializar(std::string serializado);

	// Serializa la informacion de un jugador para su almacenamiento.
	std::string serializar();

	// Informa al objeto que sucedio un evento del tipo 'notificacion'.
	virtual void notificar(tipo_notificacion_t notificacion);

	// Inicializa al jugador.
	void inicializar();

	// Elimina lo que sea necesario.
	void limpiar();

	// Actualiza la informacion del jugador para poder dibujarla.
	void actualizar();

	// Dibuja la informacion del jugador.
	void dibujar();

	// Aplica una modificacion.
	virtual void aplicarModificacion(ControladorJuego * juego);

	enum estado_t{
		CONECTADO = 0,
		DESCONECTADO = 1,
		NOEXISTE = 2,
	};

	std::string nombre;
	int numero;
	int vidas;
	int puntaje;
	estado_t estado;

	estado_juego_t estadoJuego;

private:

	TextoGUI * nombreGUI;
	TextoGUI * vidasGUI;
	TextoGUI * puntajeGUI;
	ImagenEstaticaGUI * corazonGUI;
};

#endif /* JUGADOR_H_ */