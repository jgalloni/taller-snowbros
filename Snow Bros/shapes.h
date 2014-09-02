/*
 * shapes.h
 *
 *  Created on: 30/08/2014
 *      Author: coco
 */

#ifndef SHAPES_H_
#define SHAPES_H_

#define PI 3.14159265
#include <Box2D/Box2D.h>
#include <string>
#include <cmath>

class shapes {
public:
	shapes();
	shapes(std::string data,b2World* mundo);
	virtual ~shapes();
	bool b2d_objet(std::string data,b2World *mundo);
	b2Body *_shape;


private:
	b2Vec2 * getPoligonPoint(int lados, float scale);
	float getScale(std::string data);
	bool getDynamic(std::string data);
	int getType(std::string data);
	b2Vec2 getPos(std::string data);
	float getMass(std::string data);
	b2Vec2 getSize(std::string data);
	bool update_Position();//cambia la posicion de la imagen
//	bool Dynamic; //si es dinamico o estatico
//	b2Vec2 pos; // posiocion
//	b2Vec2 dim; // dimencion
//	unsigned int color; //color
//	int rot_degree; //angulo de rotacion
//	int mass; //masa
//	int type;// 1=circulo 3= triangulo 4=rectangulo 5=pentagono...
//	int scale; //escala . se define 1 si es un cuadrado

};

#endif /* SHAPES_H_ */
