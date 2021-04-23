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
    HTTPServer::router.addRoute(Http::Method::Post, "/actualizarValorVariable", Rest::Routes::bind(&HTTPServer::actualizarValorVariable, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/createStruct", Rest::Routes::bind(&HTTPServer::crearStruct, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/devolverVariable", Rest::Routes::bind(&HTTPServer::devolverVariable, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/devolverDireccion", Rest::Routes::bind(&HTTPServer::returnAddress, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/asignarDireccion", Rest::Routes::bind(&HTTPServer::asignarDireccion, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/dellocarPuntero", Rest::Routes::bind(&HTTPServer::dellocarPuntero, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/actualizarScopes", Rest::Routes::bind(&HTTPServer::actualizarScopes, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/finalizarEjecucion", Rest::Routes::bind(&HTTPServer::finalizarEjecucion, this));

}

void HTTPServer::crearVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->createVariable(request.body());
    std::cout << jsonString << std::endl;
    if(jsonString == "La variable ya existe"){
        response.send(Http::Code::Bad_Request, jsonString);
    }else {
        response.send(Http::Code::Ok, jsonString);
    }
}

void HTTPServer::devolverVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->returnVariableValue(request.body());
    if(jsonString == "La variable no existe"){
        response.send(Http::Code::Bad_Request, jsonString);
    }else
        response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::crearStruct(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->createStruct(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::asignarDireccion(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->assignAddress(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::dellocarPuntero(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->dereferencePointer(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::dellocarPunteroYAsignarValor(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->dereferenceAndSetPointerValue(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::actualizarScopes(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->updateScopes(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::actualizarValorVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->updateVariableValue(request.body());
    response.send(Http::Code::Ok, jsonString);
}


void HTTPServer::returnAddress(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->returnAddress(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::finalizarEjecucion(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    VariableManager::getInstance()->endRun();
    response.send(Http::Code::Ok);
}
