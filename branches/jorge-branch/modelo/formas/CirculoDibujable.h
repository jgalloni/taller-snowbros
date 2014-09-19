#ifndef CIRCULODIBUJABLE_H_
#define CIRCULODIBUJABLE_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "../../utiles/Logger.h"
#include <math.h>

class CirculoDibujable: public Objeto, public IDibujable {
private:
	Sint16 rad; // radio
	int arcoMarca() {
		float radioArco  = (rad*0.50);
		Uint8 newRed = 255 - oColor.r;
		Uint8 newGreen = 255 - oColor.g;
		Uint8 newBlue = 255 - oColor.b;
		int32 anguloInit = this->angulo();
		float posx=(oPosicion.x*oEscalaX)-((radioArco)*cos(oAngulo)*oEscalaX)-((radioArco)*sin(oAngulo)*oEscalaX);
		float posy=(oPosicion.y*oEscalaY)-((radioArco)*cos(oAngulo)*oEscalaY)+((radioArco)*sin(oAngulo)*oEscalaY);
		if(anguloInit > 360) {
			anguloInit -= 360;
		}
		int32 anguloEnd = anguloInit + 50;
		return filledEllipseRGBA(dRenderer, posx,-posy, oEscalaX*rad/8,oEscalaY*rad/8, newRed, newGreen, newBlue, oColor.a);
	}

public:
	CirculoDibujable() {
		rad = 0;
	}
	~CirculoDibujable() {}

	virtual void render() {
		// Se redefine distinto, usa el renderer directo
		int status;
		status = filledEllipseRGBA(dRenderer, oPosicion.x*oEscalaX ,-oPosicion.y*oEscalaY, oEscalaX*rad,oEscalaY*rad, oColor.r, oColor.g, oColor.b, oColor.a);
		if(status != 0) {
			Logger& log = * Logger::Instancia();
			if(!log.abrirLog(DIBUJABLELOG)) {
				std::string err(SDL_GetError());
				log.escribirLog("WAR", "No se renderizo el circulo "+err);
				log.cerrarLog();
				return;
			}
		}
		status = arcoMarca();
		if(status != 0) {
			Logger& log = * Logger::Instancia();
			if(!log.abrirLog(DIBUJABLELOG)) {
				std::string err(SDL_GetError());
				log.escribirLog("WAR", "No se renderizar la marca del circulo "+err);
				log.cerrarLog();
				return;
			}
		}

	}

	void setRadio(Sint16 radio) {
		rad = radio;
	}

};



#endif
