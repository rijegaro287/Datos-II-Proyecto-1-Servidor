#include "Communication/Socket.h"
#include "VariableManager.h"
#include "Communication/HTTPServer.h"


using namespace std;
int main(){
    HTTPServer *mserver = HTTPServer::createServer(8888);

}