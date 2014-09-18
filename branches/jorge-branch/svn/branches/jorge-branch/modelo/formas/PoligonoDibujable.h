#ifndef POLIGONODIBUJABLE_H
#define POLIGONODIBUJABLE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "../../utiles/Logger.h"
#include <math.h>
#include <iostream>

class PoligonoDibujable: public Objeto, public IDibujable{

private:
	Sint16 *vx, *vy;
	int ver; // Cantidad de vertices
public:
	PoligonoDibujable() {
		ver = 0;
		vx = vy = NULL;
	}
	~PoligonoDibujable() {
		if(vx)
			delete[] vx;
		if(vy)
			delete[] vy;
	}

	virtual void render() {
		// Se redefine distinto, usa el renderer directo
		int status;
		Sint16 *px =new Sint16 [ver];
		Sint16 *py =new Sint16 [ver];
		for(int i=0;i<ver;i++){
			px[i]=(vx[i]*cos(oAngulo))-(vy[i]*sin(oAngulo))+oPosicion.x;
			py[i]=(vy[i]*cos(oAngulo))+(vx[i]*sin(oAngulo))+oPosicion.y;
		}
		status = filledPolygonRGBA(dRenderer, px, py, ver, oColor.r, oColor.g, oColor.b, oColor.a);
		if(status != 0) {
			Logger& log = * Logger::Instancia();
			if(!log.abrirLog(DIBUJABLELOG)) {
				std::string err(SDL_GetError());
				log.escribirLog("WAR", "No se renderizo el poligono "+err);
				log.cerrarLog();
				delete px;
				delete py;
				return;
			}
		}
		delete px;
		delete py;
	}

	bool setVertices(Sint16* vx, Sint16* vy, int nvert) {
		if (vx != NULL) {
			this->vx = vx;
		} else {
			return false;
		}
		if (vy != NULL) {
			this->vy = vy;
		} else {
			return false;
		}
		if (nvert >= 3) {
			this->ver = nvert;
		} else {
			return false;
		}
		return true;
	}
};

#endif
