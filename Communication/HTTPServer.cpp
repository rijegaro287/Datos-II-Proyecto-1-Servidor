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
    HTTPServer::router.addRoute(Http::Method::Post, "/createStruct", Rest::Routes::bind(&HTTPServer::createStruct, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/devolverVariable", Rest::Routes::bind(&HTTPServer::devolverVariable, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/asignarDireccion", Rest::Routes::bind(&HTTPServer::asignarDireccion, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/dellocarPuntero", Rest::Routes::bind(&HTTPServer::dellocatePuntero, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/dellocarPunteroYAsignarValor", Rest::Routes::bind(&HTTPServer::dellocarPunteroYAsignarValor, this));
    HTTPServer::router.addRoute(Http::Method::Post, "/actualizarScopes", Rest::Routes::bind(&HTTPServer::actualizarScopes, this));


}

void HTTPServer::crearVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->createVariable(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::devolverVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->returnVariableValue(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::createStruct(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->createStruct(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::asignarDireccion(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->assignAddress(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::dellocatePuntero(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->dellocatePointer(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::dellocarPunteroYAsignarValor(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->dellocateAndSetPointerValue(request.body());
    response.send(Http::Code::Ok, jsonString);
}

void HTTPServer::actualizarScopes(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    std::string jsonString = VariableManager::getInstance()->updateScopes(request.body());
    response.send(Http::Code::Ok, jsonString);
}