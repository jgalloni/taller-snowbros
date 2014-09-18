/*
 * Personaje.cpp
 *
 *  Created on: 3/9/2014
 *      Author: manuel
 */

#include "Personaje.h"

Personaje::Personaje() : pEstado(ESTADO_PARADO){}

Personaje::~Personaje() {}

void Personaje::eventoArriba()
{
	if( pEstado != ESTADO_SALTANDO )
	{
		pEstado = ESTADO_SALTANDO;
		printf("EVENTO ARRIBA: SALTO\n");
	}
	if(cuer->GetLinearVelocity().y!=0){
		return;
	}
	b2Vec2 vel = cuer->GetLinearVelocity();
	float impulse = cuer->GetMass() * 10-vel.y;
	cuer->ApplyLinearImpulse( b2Vec2(0,impulse), cuer->GetWorldCenter(),true );
}

void Personaje::eventoDerecha()
{
	if( pEstado != ESTADO_DERECHA )
	{
		pEstado = ESTADO_DERECHA;
		cargarImagen("imagenes/derecha.png");
	}
	b2Vec2 vel = cuer->GetLinearVelocity();
		 vel.x=5-vel.x;
	cuer->ApplyLinearImpulse( b2Vec2(vel.x,0), cuer->GetWorldCenter(),false );
}

void Personaje::eventoIzquierda()
{
	if( pEstado != ESTADO_IZQUIERDA )
	{
		pEstado = ESTADO_IZQUIERDA;
		cargarImagen("imagenes/izquierda.png");
	}
	b2Vec2 vel = cuer->GetLinearVelocity();
		 vel.x=-5-vel.x;
	cuer->ApplyLinearImpulse( b2Vec2(vel.x,0), cuer->GetWorldCenter(),true );
}

void Personaje::eventoAbajo()
{
	if( pEstado != ESTADO_ABAJO )
	{
		pEstado = ESTADO_ABAJO;
		printf("EVENTO ABAJO: MOVER ABAJO\n");
	}
	cuer->ApplyLinearImpulse( b2Vec2(0,-5), cuer->GetWorldCenter(),true );
}

void Personaje::eventoRESTART()
{
	if( pEstado !=  ESTADO_PARADO)
	{
		pEstado = ESTADO_PARADO;
		cargarImagen("imagenes/parado.png");
	}
	posicion(100.0 ,100.0);
}

bool Personaje::saltar()
{
	bool pudoSaltar = true;

	if( pEstado == ESTADO_SALTANDO )
		return !pudoSaltar;

	//TODO aplicar el salto con box2d
	pEstado = ESTADO_SALTANDO;
	printf("SALTAR!\n");

	return pudoSaltar;
}
