#include <json/json.h>
#include "Memory/MemoryPool.h"
#include "VariableManager.h"
#include "Communication/HTTPServer.h"


using namespace std;
int main(){
//  HTTPServer *mserver = HTTPServer::createServer(9999);

    Json::Value jsonObject;
    jsonObject["nombre"] = "num";
    jsonObject["valor"] = 23.23;
    jsonObject["tipoDeDato"] = "float";
    std::string jsonString = VariableManager::jsonToString(jsonObject);
    VariableManager::getInstance()->createVariable(jsonString);

    Json::Value jsonObject1;
    jsonObject1["nombre"] = "pointer";
    jsonObject1["tipoDeReferencia"] = "float";
    jsonObject1["tipoDeDato"] = "referencia";
    jsonObject1["espacioEnMemoriaDeTipoDeReferencia"] = 4;
    std::string jsonString1 = VariableManager::jsonToString(jsonObject1);
    VariableManager::getInstance()->createVariable(jsonString1);

    Json::Value jsonObject2;
    jsonObject2["nombre"] = "pointer1";
    jsonObject2["tipoDeReferencia"] = "float";
    jsonObject2["tipoDeDato"] = "referencia";
    jsonObject2["espacioEnMemoriaDeTipoDeReferencia"] = 4;
    std::string jsonString2 = VariableManager::jsonToString(jsonObject2);
    VariableManager::getInstance()->createVariable(jsonString2);

    //asignar valor
    Json::Value jsonObject3;
    jsonObject3["nombreDePuntero"] = "pointer";
    jsonObject3["nombreDeVariable"] = "";
    jsonObject3["valor"] = 782.235;
    jsonObject3["tipoDeDato"] = "float";
    std::string jsonString3 = VariableManager::jsonToString(jsonObject3);
    VariableManager::getInstance()->assignAddress(jsonString3);

//    //asignar valor
//    Json::Value jsonObject5;
//    jsonObject5["nombreDePuntero"] = "pointer1";
//    jsonObject5["nombreDeVariable"] = "pointer";
//    std::string jsonString5 = VariableManager::jsonToString(jsonObject5);
//    VariableManager::getInstance()->assignAddress(jsonString5);

    Json::Value jsonObject4;
    jsonObject4["nombre"] = "pointer";
    std::string jsonString4 = VariableManager::jsonToString(jsonObject4);
    std::cout <<VariableManager::dereferencePointer(jsonString4) << std::endl;
}