/*
 * BolaEnemigo.cpp
 *
 *  Created on: 24/11/2014
 *      Author: coco
 */

#include "BolaEnemigo.h"

BolaEnemigo::BolaEnemigo(b2Body * body){
	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2CircleShape circle;
	WorldItem * figura;
	this->tiempoDeVida=0;
	this->toDelete=false;

	b2dObjDef.type = b2_dynamicBody;
	b2dObjDef.bullet = true;

	b2dObjDef.position.x = body->GetPosition().x;
	b2dObjDef.position.y = body->GetPosition().y;

	circle.m_radius = 1.3f; //defino el tamaño
	myFixtureDef.shape = &circle; //defino que es un circulo

				// Determina el tipo de figura para poder dibujarla.
	this->radio = circle.m_radius;
	//lo vinculo al mundo
	bodyB2D = body->GetWorld()->CreateBody(&b2dObjDef);
	myFixtureDef.density =1.0f; //le doy masa
	myFixtureDef.restitution = 0.8f;

	myFixtureDef.friction = 0.3f;

	b2Fixture * shapeFixture = bodyB2D->CreateFixture(&myFixtureDef); //le asigno la forma
	if (b2dObjDef.type == b2_dynamicBody) shapeFixture->SetUserData((void*) BOLASNOW);
	else {
		shapeFixture->SetUserData((void*) BOLASNOW);
	}

	// Setea los ultimos parametros de la figura y vincula al bodyB2D.
	this->posicion.x = b2dObjDef.position.x;
	this->posicion.y = b2dObjDef.position.y;
	this->angulo = b2dObjDef.angle;
	bodyB2D->SetUserData(this);
	activeSprite=CONGELADO;
	tipo=BOLASNOW;
}

void BolaEnemigo::Impulso(orientation_t ori){
	if(ori==LEFT)
		bodyB2D->ApplyLinearImpulse(b2Vec2(-3000.0f,0), bodyB2D->GetWorldCenter(),true);
	else
		bodyB2D->ApplyLinearImpulse(b2Vec2(3000.0f,0), bodyB2D->GetWorldCenter(),true);
}

std::string BolaEnemigo::serializar(){

	std::string buffer;
	buffer = SSTR(BOLASNOW << " " << posicion.x << " " << posicion.y << " "
			<< angulo << " " << radio);
	return buffer;
}

BolaEnemigo::~BolaEnemigo() {
	this->bodyB2D->GetWorld()->DestroyBody(this->bodyB2D);
	//TODO: crear Sorpresa
}

void BolaEnemigo::aumentarTiempo(){
	tiempoDeVida++;
	if(tiempoDeVida>=100000)
		setDelete();
	return;
}
