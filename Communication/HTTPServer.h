#pragma once
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
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
};