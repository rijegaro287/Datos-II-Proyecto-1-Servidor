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
}

void HTTPServer::crearVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response) {
    log(request.body());
    response.send(Http::Code::Ok, "Variable creada en...");
}