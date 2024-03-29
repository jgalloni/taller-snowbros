/*
 * Inicializador.cpp
 *
 *  Created on: Sep 21, 2014
 *      Author: rodrizapico
 */

#include "Inicializador.h"

float Inicializador::widthWorld;
float Inicializador::heightWorld;

bool loadInitialValues(std::string configFile, std::string& sConfig) {
	Logger& log = *Logger::Instancia();
	//abre el json y lo carga a un string
	fstream fConfig;
	fConfig.open(configFile.c_str(), ios_base::in);
	if (!fConfig.is_open()) {
		if (!log.abrirLog(MAINLOG)) {
			log.crearLogs();
			std::cout << "Error al abrir archivo de log " << MAINLOG
					<< ", creando..." << std::endl;
			if (!log.abrirLog(MAINLOG)) {
				std::cout << "No se pudo crear el archivo de log.";
				return false;
			}
		}

		log.escribirLog(WARNING,
				"No se pudo encontrar o abrir el archivo de configuracion. Cargando mapa por defecto.");
		log.cerrarLog();

		fConfig.open("defaultConfig.json", ios_base::in);
		if (!fConfig.is_open()) {
			if (!log.abrirLog(MAINLOG)) {
				log.crearLogs();
				std::cout << "Error al abrir archivo de log " << MAINLOG
						<< ", creando..." << std::endl;
				if (!log.abrirLog(MAINLOG)) {
					std::cout << "No se pudo crear el archivo de log.";
					return false;
				}
			}

			log.escribirLog(ERROR,
					"No se pudo encontrar o abrir el archivo de configuracion por defecto.");
			log.cerrarLog();
			return false;
		}

	}
	std::string newStr((std::istreambuf_iterator<char>(fConfig)),
			std::istreambuf_iterator<char>());
	sConfig = newStr;
	fConfig.close();

	if (!parsingOk(sConfig)) {
		fConfig.open("defaultConfig.json", ios_base::in);
		if (!fConfig.is_open()) {
			if (!log.abrirLog(MAINLOG)) {
				log.crearLogs();
				std::cout << "Error al abrir archivo de log " << MAINLOG
						<< ", creando..." << std::endl;
				if (!log.abrirLog(MAINLOG)) {
					std::cout << "No se pudo crear el archivo de log.";
					return false;
				}
			}

			log.escribirLog("ERROR",
					"No se pudo encontrar o abrir el archivo default.json");
			log.cerrarLog();
			return false;
		}
		std::string newStr((std::istreambuf_iterator<char>(fConfig)),
				std::istreambuf_iterator<char>());
		sConfig = newStr;
		fConfig.close();
	}

	return true;
}

void worldInit(b2World ** worldB2D, ContactListener * contactListener) {

	b2Vec2 gravedad(0, 35);
	*worldB2D = new b2World(gravedad);

	(*worldB2D)->SetContactListener(contactListener);

	return;
}

Camera * Inicializador::cameraInit(b2World ** worldB2D, b2Body * pj, ThreadSafeList<WorldItem*> & rList,
		float32 windowWidth, float32 windowHeight){

	Logger& log = *Logger::Instancia();

	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2PolygonShape polygon;

	// Parametros iniciales de la camara.
	b2dObjDef.type = b2_dynamicBody;
	b2dObjDef.gravityScale = 0;
	float32 cameraX = pj->GetPosition().x;
	float32 cameraY = pj->GetPosition().y;
	b2dObjDef.position.Set(cameraX, cameraY);
	b2dObjDef.angle = 0;
	b2dObjDef.fixedRotation = true;

	// Agrego la camara al mundo B2D.
	b2Body *cameraB2D = (*worldB2D)->CreateBody(&b2dObjDef);

	//le doy forma
	float32 halfWidth = (windowWidth / 2) * 0.05f; // TODO: 0.05f = windowToWorldScale. HARDCODEADO!!!
	float32 halfHeight = (windowHeight / 2) * 0.05f; // 0.05f = windowToWorldScale. HARDCODEADO!!!
	polygon.SetAsBox(halfWidth, halfHeight);
	myFixtureDef.shape = &polygon;
    myFixtureDef.isSensor = true;

	b2Fixture * bodyFixture = cameraB2D->CreateFixture(&myFixtureDef);
	bodyFixture->SetUserData( (void*)CAMARA );

	// 0.05f = windowToWorldScale. HARDCODEADO!!!
	Camera * camera = new Camera(rList, 720, 640, widthWorld, heightWorld, 0.05f);
	if(!camera) {
		log.log(WINDOWLOG,ERROR, "No se pudo asignar memoria para la camara.");
		return NULL;
	}

	cameraB2D->SetUserData(camera);
	camera->setB2DBody(cameraB2D);
	camera->setPJ(pj);

	return camera;
}

void Inicializador::enemysInit(b2World ** worldB2D,
		std::string configFile, ControladorEnemigos & army) {

	int cant = get_size("enemigos", configFile);
	for (int i = 0; i < cant; i++) {
		EnemigoEstandar* enemy = createEnemy(configFile, worldB2D, i);
		if(enemy) {
			army.controlUnit(enemy);
		} else {
			std::cout << "No se agrego un enemigo" << std::endl;
		}
	}
	return;
}

Personaje * Inicializador::pjInit(b2World ** worldB2D, ThreadSafeList<WorldItem*> & rList,
		int numeroUsuario, std::string configFile){
	Logger& log = *Logger::Instancia();
	std::string data;
	bool statusOK = true;
	statusOK = loadInitialValues(configFile, data);
	if (!statusOK) return NULL;

	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2PolygonShape polygon;

	// Parametros iniciales.
	b2dObjDef.type = b2_dynamicBody;
	std::string campoPJ = SSTR("personaje" << numeroUsuario);

	std::cout << "el campo es: " << campoPJ << std::endl;

	float32 pj_x = get_node("x", campoPJ, data, 5.0f);
	float32 pj_y = get_node("y", campoPJ, data, 10.0f);

	std::cout << "el valor de (x,y) es: (" << pj_x << ", " << pj_y << ")" << std::endl;

	float32 anchoMaximo = get_node("ancho-un", "escenario", data, 10.0f);
	float32 altoMaximo = get_node("alto-un", "escenario", data, 10.0f);

	// si la posicion del persona esta fuera del escenario, entonces lo posicionamos en el medio
	if( (pj_x > anchoMaximo) | (pj_y > altoMaximo) ){
		if (!log.abrirLog(WINDOWLOG)) {
			std::cout << "Error al abrir archivo de log" << std::endl;
			return NULL;
		}
		log.escribirLog(WARNING, "No se puede inicializar al personaje fuera del escenario. Seteado por default al medio del escenario.");
		log.cerrarLog();
		pj_x = ( anchoMaximo * 0.5 ); pj_y = ( altoMaximo * 0.5 );
	}

	b2dObjDef.position.Set(pj_x, pj_y);
	b2dObjDef.angle = 0;
	b2dObjDef.fixedRotation = true;
	b2dObjDef.bullet = true;

	//lo vinculo al mundo
	b2Body *pjB2D = (*worldB2D)->CreateBody(&b2dObjDef);

	//le doy forma
	float32 halfHeight = get_node("alto", campoPJ, data, 1.4f);
	float32 halfWidth = get_node("ancho", campoPJ, data, 1.2f);
	b2Vec2 v(0,(halfHeight*-0.1));
	polygon.SetAsBox(halfWidth*0.7, halfHeight*0.7,v,0); //le doy dimensiones
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.density =  get_node("masa", campoPJ, data, 20.0f); //le doy masa
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=0.2;
	b2Fixture * bodyFixture = pjB2D->CreateFixture(&myFixtureDef); //le asigno la forma
	bodyFixture->SetUserData( (void*)PERSONAJE );


	//costados sin friccion
	v.x=-halfWidth*0.7;
	polygon.SetAsBox(halfWidth*0.15, halfHeight*0.7,v,0); //le doy dimensiones
	myFixtureDef.shape = &polygon; //defino que es un poligono;
	myFixtureDef.density=0;
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=0;
	b2Fixture* fixture = pjB2D->CreateFixture(&myFixtureDef);
	fixture->SetUserData( (void*)PERSONAJE );

	//costados sin friccion
	v.x=halfWidth*0.7;
	polygon.SetAsBox(halfWidth*0.15, halfHeight*0.7,v,0); //le doy dimensiones
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=0;
	fixture = pjB2D->CreateFixture(&myFixtureDef);
	fixture->SetUserData( (void*)PERSONAJE );

	//pies
	v.x=0;
	v.y=halfHeight*0.75;
	polygon.SetAsBox(halfWidth*0.4, halfHeight*0.1, v, 0);
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.density = 1.0f; //le doy masa
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=2.0f;
	fixture = pjB2D->CreateFixture(&myFixtureDef);
	fixture->SetUserData( (void*)PERSONAJE );


    // Agrego el sensor para saltos
    polygon.SetAsBox(halfWidth * 0.3, 0.15f, b2Vec2(0,halfHeight), 0);
	myFixtureDef.shape = &polygon; //defino que es un poligono
    myFixtureDef.isSensor = true;
	myFixtureDef.density = 1.0f; //le doy masa
    b2Fixture* footSensorFixture = pjB2D->CreateFixture(&myFixtureDef);
    footSensorFixture->SetUserData( (void*)PIESPJ );

    // Agrego el sensor para empujar
    polygon.SetAsBox(0.1, 0.1f, b2Vec2(halfWidth*1.2,halfHeight*-0.1), 0);
    myFixtureDef.shape = &polygon; //defino que es un poligono
    myFixtureDef.isSensor = true;
    myFixtureDef.density = 1.0f; //le doy masa
    b2Fixture* pushSensor1 = pjB2D->CreateFixture(&myFixtureDef);
    pushSensor1->SetUserData( (void*)EMPUJE );

    polygon.SetAsBox(0.1, 0.1f, b2Vec2(-halfWidth*1.2,halfHeight*-0.1), 0);
    myFixtureDef.shape = &polygon; //defino que es un poligono
    myFixtureDef.isSensor = true;
    myFixtureDef.density = 1.0f; //le doy masa
    b2Fixture* pushSensor2 = pjB2D->CreateFixture(&myFixtureDef);
    pushSensor2->SetUserData( (void*)EMPUJE );

    // Crea la representacion del PJ fuera del mundo de B2D, para manejar su movimiento
    // y datos necesarios para renderizarlo en el cliente.
    Personaje * pj = new Personaje();
    pj->setB2DBody(pjB2D);
    pj->baseMayor = halfWidth * 2;
    pj->baseMenor= halfWidth * 2;
    pj->altura = halfHeight * 2;
    pj->desplazamiento = 0;
    pj->posicion.x = pj_x;
    pj->posicion.y = pj_y;
    pj->initPos=b2Vec2(pj_x,pj_y);
    pjB2D->SetUserData(pj);

    // Crea la camara para este personaje.
    // TODO: 720x640: resolucion pantalla. HARDCODEADO!!!
    pj->camera = cameraInit(worldB2D, pjB2D, rList, 720, 640);

    return pj;
}

int num_lados(std::string data) {
	if (!data.compare("circ"))
		return 1;
	if (!data.compare("rect"))
		return 4;
	if (!data.compare("poli"))
		return 3;
	if (!data.compare("irreg"))
		return 5;
	else
		return 0;
}

b2Body * createObject(std::string data, b2World ** wB2D, int num) {

	Logger& log = *Logger::Instancia();
	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2CircleShape circle;
	b2PolygonShape poligon;
	WorldItem * figura;

	if (get_node("estatico", "objetos", data, num, false))
		b2dObjDef.type = b2_staticBody;
	else {
		b2dObjDef.type = b2_dynamicBody;
		b2dObjDef.bullet = true;
	}

	//posicion inicial
	b2dObjDef.position.x = get_node("x", "objetos", data, num, 1.0f);
	b2dObjDef.position.y = get_node("y", "objetos", data, num, 1.0f);
	b2dObjDef.angle = get_node("rot", "objetos", data, num, 0.0f) * DEGTORAD;

	//lo vinculo al mundo
	b2Body *_shape = (*wB2D)->CreateBody(&b2dObjDef);

	//le doy forma
	int lados = num_lados(get_node("tipo", "objetos", data, num, "rect"));
	//nLados=lados;
	switch (lados) { //dependiendo del numero de lados
	case 0: //0 es error
		if (!log.abrirLog(SHAPESLOG)) {
			std::cout << "Error al abrir archivo de log" << std::endl;
			return NULL;
		}
		log.escribirLog(ERROR, "El tipo de forma '" + get_node("tipo", "objetos", data, num,"rect") + "' no existe.");
		log.cerrarLog();
		(*wB2D)->DestroyBody(_shape);
		return NULL;

	case 1: //1 solo lado  = circulo
	{
		circle.m_radius = get_node("escala", "objetos", data, num, 1.0f); //defino el tamaño
		myFixtureDef.shape = &circle; //defino que es un circulo

		// Determina el tipo de figura para poder dibujarla.
		Circulo * circ = new Circulo();
		circ->radio = circle.m_radius;
		figura = circ;

		break;
	}
	case 4: // 4 lados caja
	{
		float32 halfHeight = get_node("alto", "objetos", data, num, 1.0f) / 2;
		float32 halfWidth = get_node("ancho", "objetos", data, num, 1.0f) / 2;
		poligon.SetAsBox(halfWidth, halfHeight); //le doy dimensiones
		myFixtureDef.shape = &poligon; //defino que es un poligono

		// Determina el tipo de figura para poder dibujarla.
		Cuadrilatero * rect = new Cuadrilatero();
		rect->baseMayor =  halfWidth * 2;
		rect->baseMenor =  halfWidth * 2;
		rect->altura =  halfHeight * 2;
		rect->desplazamiento =  0;

		if( get_node("esLaguna", "objetos", data, num, false) ){
			_shape->SetBullet(false);
			myFixtureDef.isSensor = true;
			//Laguna * laguna = (Laguna*) rect;
			Laguna * laguna = new Laguna();
			laguna->baseMayor =  halfWidth * 2;
			laguna->baseMenor =  halfWidth * 2;
			laguna->altura =  halfHeight * 2;
			laguna->desplazamiento =  0;
			figura = laguna;
			break;
		}
		if( get_node("esAgujero", "objetos", data, num, false) ){
			_shape->SetBullet(false);
			myFixtureDef.isSensor = true;
			Agujero * agujero = new Agujero();
			agujero->baseMayor =  halfWidth * 2;
			agujero->baseMenor =  halfWidth * 2;
			agujero->altura =  halfHeight * 2;
			agujero->desplazamiento =  0;
			figura = agujero;
			break;
		}

			figura = rect;

		b2FixtureDef borderSensFix;
		b2PolygonShape polygon;

		break;
	}
	case 5: { // poligonos irregulares

		float base = get_node("base", "objetos", data, num, 1.0f);
		float tapa = get_node("tapa", "objetos", data, num, 1.0f);
		float alto = get_node("alto", "objetos", data, num, 1.0f);
		float desp = get_node("desp", "objetos", data, num, 1.0f); //desplazamiento

		b2Vec2 *_point = new b2Vec2[lados];
		float32 centroX = ((2 * tapa * desp + base * desp + base * tapa
				+ pow(base, 2) + pow(tapa, 2)) / (3 * (base + tapa)));
		float32 centroY = ((alto * (base + 2 * tapa)) / (3 * (base + tapa)));
		_point[0].Set(-centroX, centroY);
		_point[1].Set(-centroX + base, centroY);
		_point[2].Set(-centroX + desp, centroY - alto);
		_point[3].Set(-centroX + desp + tapa, centroY - alto);
		poligon.Set(_point, 4);
		myFixtureDef.shape = &poligon;

		// Determina el tipo de figura para poder dibujarla.
		Cuadrilatero * poli = new Cuadrilatero();
		poli->baseMayor =  base;
		poli->baseMenor =  tapa;
		poli->altura =  alto;
		poli->desplazamiento =  desp;
		figura = poli;

		break;
	}

	default: // 3, 5 o mas lados poligonos regulares

		lados = get_node("lados", "objetos", data, num, 3);
		float escala = get_node("escala", "objetos", data, num, 1.0f);

		b2Vec2 * _point = new b2Vec2[lados]; //creo un vector con los vertices
		for (int i = 0; i < lados; i++) {
			float angulo = (2 * i * PI / lados); //- (b2dObjDef.angle * 0.017469944f);
			_point[i].Set(escala * cos(angulo), escala * sin(angulo));
		} //les asigno la posicion

		poligon.Set(_point, lados);
		myFixtureDef.shape = &poligon;

		// Determina el tipo de figura para poder dibujarla.
		PoligonoRegular * poli = new PoligonoRegular();
		poli->nLados = lados;
		poli->radio = escala;
		figura = poli;

		break;

	}

	if (b2dObjDef.type == b2_dynamicBody) {
		myFixtureDef.density = get_node("masa", "objetos", data, num, 1.0f); //le doy masa
		myFixtureDef.restitution = 0.15f;
		//myFixtureDef.friction = 0.1f;
	}

	myFixtureDef.friction = get_node("friccion", "objetos", data, num, 0.3f);

	if(b2dObjDef.type == b2_staticBody)
		if(myFixtureDef.friction == 0)
			figura->hielo=true;
	b2Fixture * shapeFixture = _shape->CreateFixture(&myFixtureDef); //le asigno la forma
	if (b2dObjDef.type == b2_dynamicBody) shapeFixture->SetUserData((void*) DINAMICO);
	else {
		shapeFixture->SetUserData((void*) ESTATICO);
	}

	//Me fijo si el objeto que creo es atravesable.
	if (get_node("atravesable", "objetos", data, num, false)) shapeFixture->SetUserData((void*) ATRAVESABLE);

	// me fijo si es laguna
	if (get_node("esLaguna", "objetos", data, num, false)) shapeFixture->SetUserData((void*) sensorLAGUNA);

	// me fijo si es agujero
	if (get_node("esAgujero", "objetos", data, num, false)) shapeFixture->SetUserData((void*) sensorAGUJERO);

	// Setea los ultimos parametros de la figura y vincula al bodyB2D.
	figura->posicion.x = b2dObjDef.position.x;
	figura->posicion.y = b2dObjDef.position.y;
	figura->angulo = b2dObjDef.angle;
	_shape->SetUserData(figura);

	return _shape;
}

EnemigoEstandar * createEnemy(std::string data, b2World ** wB2D, int num) {
	Logger& log = *Logger::Instancia();
	std::string tipo = get_node("tipo", "enemigos", data, num, "est");
	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2PolygonShape polygon;
	b2dObjDef.type = b2_dynamicBody;
	float32 halfHeight = get_node("alto", "enemigos", data, num, 1.4f);
	float32 halfWidth = get_node("ancho", "enemigos", data, num, 1.2f);
	float32 en_x = get_node("x", "enemigos", data, num, 5.0f);
	float32 en_y = get_node("y", "enemigos", data, num, 10.0f);
	b2Body* enB2D = NULL;
	worlditem_t type = ENEMIGOESTANDAR;
	EnemigoEstandar * en;
	float32 anchoMaximo = get_node("ancho-un", "escenario", data, 10.0f);
	float32 altoMaximo = get_node("alto-un", "escenario", data, 10.0f);
	if( (en_x > anchoMaximo) | (en_y > altoMaximo) ){
		if (!log.abrirLog(WINDOWLOG)) {
			std::cout << "Error al abrir archivo de log" << std::endl;
			return NULL;
		}
		log.escribirLog(WARNING, "No se puede inicializar al personaje fuera del escenario. Seteado por default al medio del escenario.");
		log.cerrarLog();
		en_x = ( anchoMaximo * 0.5 ); en_y = ( altoMaximo * 0.5 );
	}
	b2dObjDef.position.Set(en_x, en_y);
	b2dObjDef.angle = 0;
	b2dObjDef.fixedRotation = true;
	b2dObjDef.bullet = true;
	//lo vinculo al mundo
	enB2D = (*wB2D)->CreateBody(&b2dObjDef);
	//le doy forma
	b2Vec2 v(0,(halfHeight*-0.1));

	polygon.SetAsBox(halfWidth*0.7, halfHeight*0.7,v,0); //le doy dimensiones
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.density =  get_node("masa", "enemigos", data, num, 20.0f); //le doy masa
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=0.2;
	b2Fixture * bodyFixture = enB2D->CreateFixture(&myFixtureDef); //le asigno la forma
	bodyFixture->SetUserData( (void*)ENEMIGO );

	//costados sin friccion
	v.x=-halfWidth*0.7;
	polygon.SetAsBox(halfWidth*0.15, halfHeight*0.7,v,0); //le doy dimensiones
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.density=0;
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=0;
	b2Fixture* fixture = enB2D->CreateFixture(&myFixtureDef);
	fixture->SetUserData( (void*)ENEMIGO ); // CAMBIAR ESTA DATA POR NUMEROS UNICOS POR CADA ENEMIGO NUEVO (ATRIBUTO DE CLASE)

	//costados sin friccion
	v.x=halfWidth*0.7;
	polygon.SetAsBox(halfWidth*0.15, halfHeight*0.7,v,0); //le doy dimensiones
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=0;
	fixture = enB2D->CreateFixture(&myFixtureDef);
	fixture->SetUserData( (void*)ENEMIGO );

	//pies
	v.x=0;
	v.y=halfHeight*0.75;
	polygon.SetAsBox(halfWidth*0.4, halfHeight*0.1, v, 0);
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.density = 1.0f; //le doy masa
	myFixtureDef.restitution = 0.0f;
	myFixtureDef.friction=20.0f;
	fixture = enB2D->CreateFixture(&myFixtureDef);
	fixture->SetUserData( (void*)PIESEN  );

	// Agrego el sensor para saltos
	polygon.SetAsBox(halfWidth * 0.3, 0.15f, b2Vec2(0,halfHeight), 0);
	myFixtureDef.shape = &polygon; //defino que es un poligono
	myFixtureDef.isSensor = true;
	myFixtureDef.density = 1.0f; //le doy masa
	b2Fixture* footSensorFixture = enB2D->CreateFixture(&myFixtureDef);
	footSensorFixture->SetUserData( (void*)PIESEN );
	if (tipo == "est") {
	    type = ENEMIGOESTANDAR;
	    en = new EnemigoEstandar(num);
	} else if(tipo == "tfue") {
		type = ENEMIGOTIRAFUEGO;
		en = new EnemigoTiraFuego(num);
	} else {
		std::cout << "No es un tipo reconocible" << std::endl;
		//No deberia entrar, pero hay que contemplarlo
		//en = new EnemigoEstandar(num);
	}
    // Crea la representacion del enemigo fuera del mundo de B2D, para manejar su movimiento
    // y datos necesarios para renderizarlo en el cliente.

    en->setB2DBody(enB2D);
    en->baseMayor = halfWidth * 2;
    en->baseMenor= halfWidth * 2;
    en->altura = halfHeight * 2;
    en->desplazamiento = 0;
    en->posicion.x = en_x;
    en->posicion.y = en_y;
    en->tipo = type;
    enB2D->SetUserData(en);

    return en;

}

void checkBoundsAndOverlap(b2World ** worldB2D, float32 widthWorld, float32 heightWorld){

	Logger& log = *Logger::Instancia();

	bool overlap;
	b2Body *b = (*worldB2D)->GetBodyList();
	while (b){
		if(b->GetPosition().x > widthWorld || b->GetPosition().y > heightWorld
				|| b->GetPosition().x < 0.0 || b->GetPosition().y < 0.0){
			void* fixData = b->GetFixtureList()->GetUserData();
			if(*(int*)&fixData == sensorAGUJERO) { // Si es un agujero, puede estar por fuera
				b = b->GetNext();
				continue;
			}

			ostringstream message;   // stream used for the conversion
			message << "El objeto en la posicion " << b->GetPosition().x << "," <<
					b->GetPosition().y << " se encuentra fuera del mapa. Borrado.";

			log.log(WINDOWLOG,WARNING,message.str());

			(*worldB2D)->DestroyBody(b);
			b = b->GetNext();
			continue;
		}

		bool skip = false;
		for (b2Body *b2 = b->GetNext(); b2; b2 = b2->GetNext()) {
			overlap = b2TestOverlap(b->GetFixtureList()->GetShape(), 0,
					b2->GetFixtureList()->GetShape(), 0,
					b->GetFixtureList()->GetBody()->GetTransform(),
					b2->GetFixtureList()->GetBody()->GetTransform());
			if (overlap) {

				ostringstream message;
				message << "Objeto en la posicion " << b->GetPosition().x << "," <<
						b->GetPosition().y << " borrado por solapamiento.";

				log.log(WINDOWLOG,WARNING,message.str());

				(*worldB2D)->DestroyBody(b);
				b = (*worldB2D)->GetBodyList();
				skip = true;
				break;
			}
		}
		if (!skip) b = b->GetNext();
	}
}

bool Inicializador::init(std::string configFile, b2World ** worldB2D,
	ContactListener * contactListener, ControladorEnemigos & army, std::string & nextLevel) {
	Logger& log = *Logger::Instancia();
	std::string sConfig;
	bool statusOK = true;
	statusOK = loadInitialValues(configFile, sConfig);
	if (!statusOK)
		return false;

	nextLevel = get_node("prox-nivel", "escenario", sConfig, "defaultConfig.json");

	int heightScreen = get_node("alto-px", "escenario", sConfig, 480);
	int widthScreen = get_node("ancho-px", "escenario", sConfig, 640);

	heightWorld = get_node("alto-un", "escenario", sConfig, 10.0f);
	widthWorld = get_node("ancho-un", "escenario", sConfig, 10.0f);

	std::string path_fondo = get_node("imagen-fondo", "escenario", sConfig,
			"imagenes/fondo2.png");

	// Inicializa el mundo de B2D.
	worldInit(worldB2D, contactListener);

	// Carga todos los objetos del mapa.
	int formas = get_size("objetos", sConfig);
	for (int i = 0; i < formas; i++) {
		createObject(sConfig, worldB2D, i);
	}

	// Carga los enemigos
	this->enemysInit(worldB2D, sConfig, army);

	// Remueve los objetos superpuestos.
	checkBoundsAndOverlap(worldB2D, widthWorld, heightWorld);

	// Agrega los limites del mundo para propositos del movimiento de las camaras.
	//addWorldBorders(worldB2D, widthWorld, heightWorld);

	log.log(MAINLOG,OK,"Se ha inicializado correctamente el mundo.");

	return true;
}

