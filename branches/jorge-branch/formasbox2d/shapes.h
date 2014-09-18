#ifndef SHAPES_H_
#define SHAPES_H_

#define PI 3.14159265
#include <iostream>
#include <sstream>
#include "parametros.h"
#include <Box2D/Box2D.h>
#include <list>
#include "../utiles/Logger.h"
#include "../parser/parser.h"
#include "../Window.h"
class parametros;
//diccionario
enum tipo{CIRCULO=1,RECTANGULO=4,POLIGONO=3,IRREGULAR=5};
const string formas[6]={"","circulo","","poligono","rectangulo","irregular"};

class shapes {
public:
	shapes();
	shapes(std::string data,b2World* mundo, int num,Window* w);
	virtual ~shapes();
	b2Body& getShape();
	int getLados();



private:
	bool b2d_objet(std::string data,b2World *mundo,int num,Window* w);
	int num_lados(std::string data);
	b2Body *_shape;
	int nLados;
	b2World *world;
	CirculoDibujable *circulo;
	PoligonoDibujable *poligono;
	bool dibujarSdl(parametros *param,b2Body *b,Window* w);
	bool update_Position();//cambia la posicion de la imagen
};

#endif /* SHAPES_H_ */


