#include <json/json.h>
#include "Memory/MemoryPool.h"
#include "MemoryManagement/VariableManager.h"
#include "Communication/HTTPServer.h"
#include <list>


using namespace std;
int main(){
  HTTPServer *mserver = HTTPServer::createServer(8080);

}