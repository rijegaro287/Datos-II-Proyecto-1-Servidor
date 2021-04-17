#pragma once
//#include <pistache/http.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include "../VariableManager.h"

#define log(msg) std::cout << msg << std::endl

using namespace Pistache;
class HTTPServer
{
public:
    static HTTPServer* createServer(int port);
private:
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
    explicit HTTPServer(Address addr) : httpEndpoint(std::make_shared<Http::Endpoint>(addr)){}
    void init(size_t thr = 1);
    void start();
    void setupRoutes();
    void crearVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response);
    void devolverVariable(const Rest::Request &request, Pistache::Http::ResponseWriter response);
    void createStruct(const Rest::Request &request, Pistache::Http::ResponseWriter response);
    void createReference(const Rest::Request &request, Pistache::Http::ResponseWriter response);
    void asignarDireccion(const Rest::Request &request, Pistache::Http::ResponseWriter response);

    void dellocatePuntero(const Rest::Request &request, Http::ResponseWriter response);

    void actualizarScopes(const Rest::Request &request, Http::ResponseWriter response);
};