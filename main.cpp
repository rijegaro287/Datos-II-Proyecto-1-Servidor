#include "Communication/Socket.h"
#include <json/json.h>
#include "Memory/MemoryPool.h"
#include "VariableManager.h"
#include "Communication/HTTPServer.h"


using namespace std;
int main(){
//    HTTPServer *mserver = HTTPServer::createServer(9999);

    //PRUEBA CREAR STRUCT
    Json::Value jsonObject;
    jsonObject["nombreDeStruct"] = "estructura";
    jsonObject["variables"][0]["nombre"] = "num2";
    jsonObject["variables"][0]["valor"] = "h";
    jsonObject["variables"][0]["tipoDeDato"] = "char";

    jsonObject["variables"][1]["nombre"] = "num1";
    jsonObject["variables"][1]["valor"] = 89;
    jsonObject["variables"][1]["tipoDeDato"] = "int";

    jsonObject["variables"][2]["nombre"] = "num4";
    jsonObject["variables"][2]["valor"] = 34;
    jsonObject["variables"][2]["tipoDeDato"] = "long";
    std::string jsonString = VariableManager::jsonToString(jsonObject);

    VariableManager::getInstance()->createStruct(jsonString);
    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl;
    Node* nodo = VariableManager::searchNode("num4");
    std::cout << (*(int*)nodo->getPointer())<< std::endl;

}