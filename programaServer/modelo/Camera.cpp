/*
 * Camera.cpp
 *
 *  Created on: Sep 26, 2014
 *      Author: rodrizapico
 */

#include "Camera.h"
#include <iostream>

//float Camera::WINDOWTOWORLDSCALE, Camera::WORLDTOWINDOWSCALE;

Camera::Camera(ThreadSafeList<WorldItem*> & rList, int windowWidth, int windowHeight,
		float worldW, float worldH, float32 windowToWorldScale):
		renderList(rList){
	cameraB2D = NULL;
	width = windowWidth;
	height = windowHeight;
	WINDOWTOWORLDSCALE = windowToWorldScale;
	WORLDTOWINDOWSCALE = 1.0f / WINDOWTOWORLDSCALE;
	canMoveLeft = canMoveRight = canMoveUp = canMoveDown = true;
	PJ = NULL;
	zoomChanged = false;
	worldWidth = worldW;
	worldHeight = worldH;
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::calcularPosRelativaPJ(){
	// Determino la posicion del PJ relativo al origen de coordenadas de la camara.
	posPJCamera = PJ->GetPosition() - cameraB2D->GetPosition();
	posPJCamera.x += ( width / 2 ) * WINDOWTOWORLDSCALE;
	posPJCamera.y += ( height / 2 ) * WINDOWTOWORLDSCALE;
	posPJCamera.x = posPJCamera.x / (width * WINDOWTOWORLDSCALE);
	posPJCamera.y = posPJCamera.y / (height * WINDOWTOWORLDSCALE);
}

void Camera::eventoZoomIn(){

	if (WINDOWTOWORLDSCALE < 0.02) return;

	calcularPosRelativaPJ();
	WINDOWTOWORLDSCALE -= 0.001f;
	WORLDTOWINDOWSCALE = 1.0f / WINDOWTOWORLDSCALE;
	zoomChanged = true;
}

void Camera::eventoZoomOut(){

	if(posPJCamera.x<0||posPJCamera.x>1|| posPJCamera.y<0||posPJCamera.y>1) return;
	calculateBorderConstraints();
	bool minZoomReached = false;
	if(!canMoveRight && !canMoveLeft) minZoomReached = true;
	if(!canMoveUp && !canMoveDown) minZoomReached = true;

	if (minZoomReached) return;

	calcularPosRelativaPJ();
	WINDOWTOWORLDSCALE += 0.001f;
	WORLDTOWINDOWSCALE = 1.0f / WINDOWTOWORLDSCALE;
	zoomChanged = true;
}

void Camera::setB2DBody(b2Body * cB2D){
	cameraB2D = cB2D;
}

void Camera::setPJ(b2Body * pjB2D){
	PJ = pjB2D;
}

void Camera::setWINDOWTOWORLDSCALE(float32 windowToWorld){
	WINDOWTOWORLDSCALE = windowToWorld;
	WORLDTOWINDOWSCALE = 1.0f / WINDOWTOWORLDSCALE;
}

void Camera::calculateBorderConstraints(){

	canMoveLeft = canMoveRight = canMoveUp = canMoveDown = true;

	if (cameraB2D->GetPosition().x - (width * WINDOWTOWORLDSCALE / 2) <= 0) {
		canMoveLeft = false;
		b2Vec2 vec;
		vec.x = (width * WINDOWTOWORLDSCALE / 2);
		vec.y = cameraB2D->GetPosition().y;
		cameraB2D->SetTransform(vec, 0);
	}
	if (cameraB2D->GetPosition().x + (width * WINDOWTOWORLDSCALE / 2) >= worldWidth) {
		canMoveRight = false;
		b2Vec2 vec;
		vec.x = worldWidth - (width * WINDOWTOWORLDSCALE / 2);
		vec.y = cameraB2D->GetPosition().y;
		cameraB2D->SetTransform(vec, 0);
	}
	if (cameraB2D->GetPosition().y - (height* WINDOWTOWORLDSCALE / 2) <= 0) {
		canMoveUp = false;
		b2Vec2 vec;
		vec.x = cameraB2D->GetPosition().x;
		vec.y = (height * WINDOWTOWORLDSCALE / 2);
		cameraB2D->SetTransform(vec, 0);
	}
	if (cameraB2D->GetPosition().y + (height * WINDOWTOWORLDSCALE / 2) >= worldHeight) {
		canMoveDown = false;
		b2Vec2 vec;
		vec.x = cameraB2D->GetPosition().x;
		vec.y = worldHeight - (height * WINDOWTOWORLDSCALE / 2);
		cameraB2D->SetTransform(vec, 0);
	}

	if (!canMoveLeft && !canMoveRight) eventoZoomIn();
	if (!canMoveUp && !canMoveDown) eventoZoomIn();

}

void Camera::updateZoom(){

	zoomChanged = false;

	// Resizeo el objeto B2D asociado a la camara de acuerdo al nuevo zoom.
	cameraB2D->DestroyFixture(cameraB2D->GetFixtureList());
	b2FixtureDef myFixtureDef;
	b2PolygonShape polygon;
	float32 halfWidth = (width / 2) * WINDOWTOWORLDSCALE;
	float32 halfHeight = (height / 2) * WINDOWTOWORLDSCALE;
	polygon.SetAsBox(halfWidth, halfHeight);
	myFixtureDef.shape = &polygon;
	myFixtureDef.isSensor = true;

	b2Fixture * bodyFixture = cameraB2D->CreateFixture(&myFixtureDef);
	bodyFixture->SetUserData( (void*)4 );


	// Recalculo la posicion de la camara para mantener la posicion relativa del personaje.
	b2Vec2 newPosCamera;
	newPosCamera.x = PJ->GetPosition().x - posPJCamera.x * (width * WINDOWTOWORLDSCALE)
			+ (width / 2) * WINDOWTOWORLDSCALE;
	newPosCamera.y = PJ->GetPosition().y - posPJCamera.y * (height * WINDOWTOWORLDSCALE)
			+ (height / 2) * WINDOWTOWORLDSCALE;

	cameraB2D->SetTransform(newPosCamera, 0);
}

void Camera::updatePosition(){

	// Determino la posicion del PJ relativo al origen de coordenadas de la camara.
	b2Vec2 posPJCamera = PJ->GetPosition() - cameraB2D->GetPosition();
	posPJCamera.x += (width / 2) * WINDOWTOWORLDSCALE;
	posPJCamera.y += (height / 2) * WINDOWTOWORLDSCALE;

	//static bool moving = false;
	static int movingCounter = -1;
	if ( 0 <= movingCounter && movingCounter < 8) movingCounter++;
	else movingCounter = -1;

	// Determino el impulso necesario para mover en X la camara.
	float32 desiredXVel = 0;
	if ( (posPJCamera.x > width * WINDOWTOWORLDSCALE * 0.75 && canMoveRight && PJ->GetLinearVelocity().x > 0) ||
			(posPJCamera.x < width * WINDOWTOWORLDSCALE * 0.25 && canMoveLeft && PJ->GetLinearVelocity().x < 0) ){
		desiredXVel = 1.0f * PJ->GetLinearVelocity().x;
		movingCounter = 0;
	}

	if ( movingCounter != 0 && ( movingCounter != -1 && ( (canMoveRight && PJ->GetLinearVelocity().x > 0) ||
			(canMoveLeft && PJ->GetLinearVelocity().x < 0) ) ) ) desiredXVel = cameraB2D->GetLinearVelocity().x;
	else if (movingCounter != 0) desiredXVel = 0;

	b2Vec2 vel = cameraB2D->GetLinearVelocity();
	float32 velXChange = desiredXVel - vel.x;
	float impulseX = cameraB2D->GetMass() * velXChange;

	// Determino el impulso necesario para mover en Y la camara.
	float32 desiredYVel = 0;
	if ( (posPJCamera.y > height * WINDOWTOWORLDSCALE * 0.75 && canMoveDown && PJ->GetLinearVelocity().y > 0) ||
			(posPJCamera.y < height * WINDOWTOWORLDSCALE * 0.25 && canMoveUp && PJ->GetLinearVelocity().y < 0) ){
		desiredYVel = 1.0f * PJ->GetLinearVelocity().y;
		movingCounter = 0;
	}

	if ( movingCounter != 0 && ( movingCounter != -1 && ( (canMoveDown && PJ->GetLinearVelocity().y > 0) ||
			(canMoveUp && PJ->GetLinearVelocity().y < 0) ) ) ) desiredYVel = cameraB2D->GetLinearVelocity().y;
	else if (movingCounter != 0) desiredYVel = 0;


	float32 velYChange = desiredYVel - vel.y;
	float impulseY = cameraB2D->GetMass() * velYChange;

	// Aplico el impulso calculado para mover la camara.
	cameraB2D->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), cameraB2D->GetWorldCenter(), true);

}

void Camera::updateRenderList(){

	std::cout << "updateando render list" << std::endl;

	// Bloquea renderList, para evitar que se modifique mientras se la esta cargando.
	renderList.lock();

	// Vacia la lista.
	renderList.clear();

	Metadata * metadata = new Metadata();
	metadata->tipo = METADATAFONDO;
	metadata->escala = WORLDTOWINDOWSCALE;
	metadata->posXCamara = (cameraB2D->GetPosition().x - width * WINDOWTOWORLDSCALE / 2) / worldWidth;
	metadata->posYCamara = (cameraB2D->GetPosition().y - height * WINDOWTOWORLDSCALE / 2) / worldHeight;
	metadata->anchoCamara = width * WINDOWTOWORLDSCALE / worldWidth;
	metadata->altoCamara = height * WINDOWTOWORLDSCALE / worldHeight;
	metadata->tamanioXMundo = worldWidth;
	metadata->tamanioYMundo = worldHeight;
	renderList.push_back(metadata);

	// Itera sobre todos los objetos que esten en contacto con la camara,
	// obteniendo sus modelos y agregandolos a la lista de objetos a renderear
	// por el cliente.
	for (b2ContactEdge* ce = cameraB2D->GetContactList(); ce; ce = ce->next) {
		b2Body * body;

		// Obtiene la informacion sobre los fixtures que estan en contacto.
		// (la camara + el cuerpo)
		void* fixtureAUserData = ce->contact->GetFixtureA()->GetUserData();
		void* fixtureBUserData = ce->contact->GetFixtureB()->GetUserData();

		// Obtiene la informacion del cuerpo que esta en contacto con la camara.
		// TODO: sacar estas constantes hardcodeadas.
		if ( *((int*)(&fixtureAUserData)) != 4 ) body = ce->contact->GetFixtureA()->GetBody();
		else body = ce->contact->GetFixtureB()->GetBody();

		// Saltea los apendices de los personajes.
		// TODO: sacar estas constantes hardcodeadas.
		if ( *((int*)(&fixtureAUserData)) == 3 || *((int*)(&fixtureBUserData)) == 3 )
			continue;

		// Saltea las demas camaras.
		if ( *((int*)(&fixtureAUserData)) == 4 && *((int*)(&fixtureBUserData)) == 4 )
			continue;

		// Actualiza la posicion y angulo.

		((WorldItem*)body->GetUserData())->posicion = body->GetPosition();
		((WorldItem*)body->GetUserData())->angulo = body->GetAngle();

		// Agrega a la lista de cosas a renderear.
		renderList.push_back( ((WorldItem*)body->GetUserData()) );
	}

	// Ya finalizo de cargar la lista, la libera.
	renderList.unlock();
}

void Camera::update(){

	// Actualiza la lista de objetos en el scope de la camara, que deben
	// ser rendereados por el cliente.
	updateRenderList();

	// Actualiza el zoom.
	if (zoomChanged) updateZoom();

	// Calcula en que direcciones es posible mover la camara.
	calculateBorderConstraints();

	// Mueve la camara segun la posicion del PJ al que esta asociada.
	updatePosition();

}
