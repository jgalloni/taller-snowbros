/*
 * shapes.cpp
 *
 *  Created on: 30/08/2014
 *      Author: coco
 */

#include "shapes.h"

shapes2::shapes2() {
	_shape=NULL;
	// TODO Auto-generated constructor stub

}
shapes2::shapes2(std::string data,b2World *mundo) {
	if(!b2d_objet(data,mundo))
		_shape=NULL;


}

shapes2::~shapes2() {
	// TODO Auto-generated destructor stub
}

bool shapes2::b2d_objet(std::string data, b2World *mundo){
	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2CircleShape circle;
	b2PolygonShape poligon;
	if(getDynamic(data)==true)
		b2dObjDef.type=b2_dynamicBody;
	else
		b2dObjDef.type=b2_staticBody;
	b2dObjDef.position=getPos(data);
	_shape=mundo->CreateBody(&b2dObjDef);
	switch(int lados= this->getType(data) ){
	case 0:
		return false;
	case 1: //1 solo lado  = circulo

		myFixtureDef.shape= &circle;
		circle.m_radius=getScale(data);
		_shape->CreateFixture(&myFixtureDef);
		break;
	case 4: // 4 lados caja
		poligon.SetAsBox(getSize(data).x/2,getSize(data).y/2);
		myFixtureDef.shape = &poligon;
		_shape->CreateFixture(&myFixtureDef);
		break;
	default: // mas lados poligonos regulares
	    poligon.Set(this->getPoligonPoint(lados,getScale(data)),lados);
		myFixtureDef.shape = &poligon;
		_shape->CreateFixture(&myFixtureDef);
		break;

	}
	return true;
}

//TODO no se si estas funciones van aca o ponerlas en una clase parser
int shapes2::getType(std::string data){
}

b2Vec2 shapes2::getPos(std::string data){
}

float getMass(std::string data){

}

b2Vec2 shapes2::getSize(std::string data){

}

b2Vec2 * shapes2::getPoligonPoint(int lados, float scale){
	b2Vec2 poly;
	for(int i=0;i<lados;i++){
		poly.x=cos(2*i/lados*PI);
		poly.x=sin(2*i/lados*PI);
	}

}

float shapes2::getScale(std::string data){
}

bool shapes2::getDynamic(std::string data){
}
