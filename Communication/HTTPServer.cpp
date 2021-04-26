#include "HTTPServer.h"

HTTPServer* HTTPServer::createServer(int port) { // -> Inicia el server que escucha en http://localhost:port/
    Address addr(Ipv4::any(), port);
    HTTPServer *stats = new HTTPServer(addr);
    stats->init();
    stats->start();
    return stats;
}

void HTTPServer::init(size_t thr) {
    auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void HTTPServer::start() {
    httpEndpoint->setHandler(HTTPServer::router.handler());
    httpEndpoint->serve();
}

void HTTPServer::setupRoutes() { // -> Agrega las rutas para los servicios
    // -> Escucha por una petición de tipo POST en http://localhost:port/crearVariable
    // -> Rest::Routes::bind indica la función que se llama para encargarse de la petición
    HTTPServer::router.addRoute(Http::Method::Post, "/crearVariable", Rest::Routes::bind(&HTTPServer::crearVariable, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/conexionInicial", Rest::Routes::bind(&HTTPServer::conexionInicial, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/actualizarValorVariable", Rest::Routes::bind(&HTTPServer::actualizarValorVariable, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/createStruct", Rest::Routes::bind(&HTTPServer::crearStruct, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/retornarAtributoDeStruct", Rest::Routes::bind(&HTTPServer::retornarAtributoDeStruct, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/devolverVariable", Rest::Routes::bind(&HTTPServer::devolverVariable, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/devolverDireccion", Rest::Routes::bind(&HTTPServer::returnAddress, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/asignarDireccion", Rest::Routes::bind(&HTTPServer::asignarDireccion, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/desreferenciarPuntero", Rest::Routes::bind(
            &HTTPServer::desreferenciarPuntero, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/actualizarScopes", Rest::Routes::bind(&HTTPServer::actualizarScopes, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/primerRecorrido", Rest::Routes::bind(&HTTPServer::primerRecorrido, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/finalizarEjecucion", Rest::Routes::bind(&HTTPServer::finalizarEjecucion, this));
}

void HTTPServer::conexionInicial(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    response.send(Http::Code::Ok, "Conexion establecida correctamente, servidor eschuchando el puerto: 9999.Espacio en memeoria creado: 1000 bytes");
}

void HTTPServer::crearVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->createVariable(request.body());
    std::cout << jsonString << std::endl;
    if(jsonString == "La variable ya existe"){
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else {
        response.send(Http::Code::Ok, jsonString);
    }
}

void HTTPServer::devolverVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->returnVariableValue(request.body());
    if(jsonString == "La variable no existe"){
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::crearStruct(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->createStruct(request.body());
    if (jsonString == "Nombre Invalido para Struct"){
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else {
        response.send(Http::Code::Ok, jsonString);
    }
}

void HTTPServer::retornarAtributoDeStruct(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->returnStructAttribute(request.body());
    if (jsonString == "Variable no encontrada"){
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else {
        response.send(Http::Code::Ok, jsonString);
    }
}

void HTTPServer::asignarDireccion(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->assignAddress(request.body());
    if(jsonString == "Puntero no encontrado" or jsonString == "Tipo de puntero no compatible" or jsonString == "Variable no encontrada") {
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::desreferenciarPuntero(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->dereferencePointer(request.body());
    if (jsonString == "Puntero no encontrado"){
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::actualizarScopes(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->updateScopes(request.body());
    if (jsonString == "Error al actualizar scopes" or jsonString == "Scope Error") {
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::actualizarValorVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->updateVariableValue(request.body());
    if(jsonString == "Variable no encontrada") {
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::returnAddress(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->returnAddress(request.body());
    if (jsonString == "Variable no encontrada") {
        VariableManager::getInstance()->endRun();
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::primerRecorrido(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
//    VariableManager::getInstance()->overview();
    response.send(Http::Code::Ok);
}


void HTTPServer::finalizarEjecucion(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    VariableManager::getInstance()->endRun();
    response.send(Http::Code::Ok);
}

