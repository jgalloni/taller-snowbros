#include "shapes.h"

shapes::shapes() {
	_shape = NULL;
	nLados=0;
	world=NULL;
	// TODO Auto-generated constructor stub
}

shapes::shapes(std::string data, b2World *mundo, int num,Window* w) {
	this->world=world;
	if (!b2d_objet(data, mundo, num,w))
		_shape = NULL;
}

shapes::~shapes() {
	//world->DestroyBody(_shape);
	// TODO Auto-generated destructor stub
}

bool shapes::b2d_objet(std::string data, b2World *mundo, int num,Window* w) {

	Logger& log= *Logger::Instancia();
	b2BodyDef b2dObjDef;
	b2FixtureDef myFixtureDef;
	b2CircleShape circle;
	b2PolygonShape poligon;
	parametros param;
	param.heightScreen=get_node("alto-px","escenario",data,480);
	param.widthScreen=get_node("ancho-px","escenario",data,640);

	param.heightWorld=get_node("alto-un","escenario",data,10.0f);
	param.widthWorld=get_node("ancho-un","escenario",data,10.0f);

	param.heightRatio=param.heightScreen/param.heightWorld;
	param.widthRatio=param.widthScreen/param.widthWorld;

	if (get_node("estatico", "objetos", data, num,false)){
		b2dObjDef.type = b2_staticBody;
		param.estatico=true;
	}
	else{
		b2dObjDef.type = b2_dynamicBody;
		param.estatico=false;
	}
	//posicion inicial
	param.posicion.x = get_node("x", "objetos", data, num,1.0f);
	param.posicion.y = get_node("y", "objetos", data, num,1.0f);
	param.angulo = get_node("rot", "objetos", data, num,0.0f);
	b2dObjDef.position.x = param.posicion.x;
	b2dObjDef.position.y = param.posicion.y;
	b2dObjDef.angle = param.angulo*DEGTORAD;

	//lo vinculo al mundo
	_shape = mundo->CreateBody(&b2dObjDef);

	//le doy forma
	param.lados=num_lados(get_node("tipo", "objetos", data, num,"rect"));
	param.tipo=param.lados;
	if (param.lados == 3)
		param.lados = get_node("lados", "objetos", data, num,3);

	switch (param.tipo) {//dependiendo del numero de lados
	case 0: //0 es error
		if (!log.abrirLog(SHAPESLOG)) {
					std::cout << "Error al abrir archivo de log" << std::endl;
					return false;
				}
				log.escribirLog("ERROR", "No se definio ninguna forma");
				log.cerrarLog();
		return false;

	case CIRCULO: //1 solo lado  = circulo

		myFixtureDef.shape = &circle; //defino que es un circulo
		myFixtureDef.density =get_node("masa", "objetos", data, num,1.0f);//le doy masa
		param.escala=get_node("escala", "objetos", data, num,1.0f);
		circle.m_radius = param.escala;
		//y el tamaño
		_shape->CreateFixture(&myFixtureDef); //le asigno la forma
		break;

	case RECTANGULO: // 4 lados caja
		param.alto=get_node("alto", "objetos", data, num,1.0f);
		param.ancho=get_node("ancho", "objetos", data, num,1.0f);
		poligon.SetAsBox(param.ancho/ 2,param.alto/ 2); //le doy dimenciones
		myFixtureDef.shape = &poligon; //defino que es un poligono
		_shape->CreateFixture(&myFixtureDef); //le asigno la forma
		break;

	case IRREGULAR:{
		param.lados=4;
		param.puntos = new b2Vec2[param.lados];
		float base=get_node("base","objetos",data,num,1.0f);
		float tapa=get_node("tapa","objetos",data,num,1.0f);
		float alto=get_node("alto","objetos",data,num,1.0f);
		float desp=get_node("desp","objetos",data,num,1.0f);//desplazamiento
		param.puntos[0].Set(-base/2.0,0.0);
		param.puntos[1].Set(base/2.0,0.0);
		param.puntos[2].Set((tapa/2.0)+(desp),alto);
		param.puntos[3].Set((tapa/-2.0)+(desp),alto);
		poligon.Set(param.puntos, param.lados);
		myFixtureDef.shape = &poligon;
		myFixtureDef.density =get_node("masa", "objetos", data, num,1.0f);
		_shape->CreateFixture(&myFixtureDef);
		break;}

	default: // 3, 5 o mas lados poligonos regulares

		param.puntos= new b2Vec2[param.lados]; //creo un vector con los vertices
		for (int i = 0; i < param.lados; i++) {
			param.puntos[i].Set(cos(2 * i * PI / param.lados), sin(2 * i * PI / param.lados));
		} //les asigno la posicion

		poligon.Set(param.puntos, param.lados);
		myFixtureDef.shape = &poligon;
		myFixtureDef.density =get_node("masa", "objetos", data, num,1.0f); //le asigno la masa
		_shape->CreateFixture(&myFixtureDef); //le asigno la forma
		break;

	}
	// Determina el color de la figura.
		std::string hexT;
		hexT.assign(get_node("color", "objetos", data, num, "FF0000"));
		std::stringstream s(hexT.substr(0, 2));
		uint16 R; s >> std::hex >> R;
		uint16 G; s.clear(); s.str(hexT.substr(2, 2)); s >> std::hex >> G;
		uint16 B; s.clear(); s.str(hexT.substr(4, 2)); s >> std::hex >> B;
		param.color = { R , G , B , 255 };

	dibujarSdl(&param,_shape,w);
	return true;
}

b2Body& shapes::getShape() {
	return *_shape;
}// retorna una referencia al objeto de box2d


//TODO no se si estas funciones van aca o ponerlas en una clase parser

int shapes::num_lados(std::string data) {
	if (!data.compare("circ")){
		return CIRCULO;
	}
	if (!data.compare("rect")){
				return RECTANGULO;
			}
	if (!data.compare("poli")){;
				return POLIGONO;
			}
	if (!data.compare("irreg")){
		return IRREGULAR;
			}
	else
		return 0;

}

bool shapes::dibujarSdl(parametros *param,b2Body *b,Window* w){
	int numero_de_vertices;
	float* vx;
	float* vy;
	switch(param->tipo){
	case CIRCULO:
		circulo = new CirculoDibujable();
		circulo->setRadio(param->escala);
		circulo->escalax(param->widthRatio);
		circulo->escalay(param->heightRatio);
		circulo->color(param->color);
		circulo->posicion((param->posicion.x),param->posicion.y-param->heightWorld);
		circulo->angulo(-param->angulo);
		b->SetUserData(circulo);
		w->insertarFigura(circulo);
		break;
	case RECTANGULO:
		poligono = new PoligonoDibujable();
		numero_de_vertices = param->lados;
		vx = new float[numero_de_vertices]; vx[0] = -(param->ancho)/2; vx[1] = (param->ancho)/2; vx[2] = (param->ancho)/2; vx[3] = -(param->ancho)/2;
		vy = new float[numero_de_vertices]; vy[0] = -param->alto/2; vy[1] = -param->alto/2; vy[2] = param->alto/2; vy[3] = param->alto/2;
		poligono->escalax(param->widthRatio);
		poligono->escalay(-param->heightRatio);
		poligono->setVertices(vx, vy, numero_de_vertices);
		poligono->posicion(param->posicion.x,param->posicion.y-param->heightWorld);
		poligono->angulo(-param->angulo);
		poligono->color(param->color);
		b->SetUserData(poligono);
		w->insertarFigura(poligono);
		break;
	case POLIGONO:
			poligono = new PoligonoDibujable();
			numero_de_vertices = param->lados;
			vx = new float[numero_de_vertices];
			vy = new float[numero_de_vertices];
			poligono->escalax(param->widthRatio);
			poligono->escalay(-param->heightRatio);

			for(int i=0;i<numero_de_vertices;i++){
				vx[i]=param->puntos[i].x;
				vy[i]=param->puntos[i].y;
			}
			poligono->setVertices(vx, vy, numero_de_vertices);
			poligono->color(param->color);
			poligono->posicion(param->posicion.x,param->posicion.y-param->heightWorld);
			poligono->angulo(-param->angulo);
			b->SetUserData(poligono);
			w->insertarFigura(poligono);
			break;
	case IRREGULAR:
				poligono = new PoligonoDibujable();
				numero_de_vertices = param->lados;
				float* vx = new float[numero_de_vertices];
				float* vy = new float[numero_de_vertices];
				poligono->escalax(param->widthRatio);
				poligono->escalay(-param->heightRatio);
				for(int i=0;i<numero_de_vertices;i++){
					vx[i]=(param->puntos[i].x);
					vy[i]=-(param->puntos[i].y);
				}
				poligono->setVertices(vx, vy, numero_de_vertices);
				poligono->color(param->color);
				poligono->posicion(param->posicion.x,param->posicion.y-param->heightWorld);
				poligono->angulo(-param->angulo);
				b->SetUserData(poligono);
				w->insertarFigura(poligono);
				break;
	}
	return true;
}



