#include "Communication/Socket.h"
#include <json/json.h>
#include "Memory/MemoryPool.h"
#include "VariableManager.h"
#include "Communication/HTTPServer.h"


using namespace std;
int main() {
    //PRUEBA DE CREAR UNA VARIABLE
//    Json::Value jsonObject;
//    jsonObject["nombre"] = "num";
//    jsonObject["valor"] = "h";
//    jsonObject["tipoDeDato"] = "char";
//    std::string jsonString = VariableManager::jsonToString(jsonObject);
//
//    VariableManager::getInstance()->createVariable(jsonString);
//    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl;
//    Node* nodo = VariableManager::searchNode("num");
//    std::cout << (*(char*)nodo->getPointer())<< std::endl;

    // PRUEBA REFERENCIA
//    Json::Value jsonObject;
//    jsonObject["nombre"] = "pointer";
//    jsonObject["tipoDeReferencia"] = "int";
//    jsonObject["tipoDeDato"] = "referencia";
//    jsonObject["espacioEnMemoriaDeTipoDeReferencia"] = 4;
//    std::string jsonString = VariableManager::jsonToString(jsonObject);
//
//    VariableManager::getInstance()->createVariable(jsonString);
//    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl;
//    Node* nodo = VariableManager::searchNode("pointer");
//    if(nodo) {
//        std::cout << "Existe variable" << std::endl;
//    }


    //PRUEBA CREAR STRUCT
//    Json::Value jsonObject;
//    jsonObject["varibles"][0]["nombre"] = "num2";
//    jsonObject["varibles"][0]["valor"] = "h";
//    jsonObject["varibles"][0]["tipoDeDato"] = "char";
//
//    jsonObject["varibles"][1]["nombre"] = "num1";
//    jsonObject["varibles"][1]["valor"] = "89";
//    jsonObject["varibles"][1]["tipoDeDato"] = "int";
//
//    jsonObject["varibles"][2]["nombre"] = "num4";
//    jsonObject["varibles"][2]["valor"] = "34";
//    jsonObject["varibles"][2]["tipoDeDato"] = "long";
//    jsonObject["nombreDeStruct"] = "estructura";
//    std::string jsonString = VariableManager::jsonToString(jsonObject);
//
//    VariableManager::getInstance()->createStruct(jsonString);
//    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl;
//    Node* nodo = VariableManager::searchNode("num4");
//    std::cout << (*(int*)nodo->getPointer())<< std::endl;

    //PRUEBA DEVOLVER VALOR VARIABLE
    //crear la variabla a buscar
//    Json::Value jsonObject;
//    jsonObject["nombre"] = "num1";
//    jsonObject["valor"] = 45765797;
//    jsonObject["tipoDeDato"] = "long";
//    std::string jsonString = VariableManager::jsonToString(jsonObject);
//    VariableManager::getInstance()->createVariable(jsonString);

    //pide el valor de la variable
//    Json::Value jsonObject1;
//    jsonObject1["nombre"] = "num1";
//    std::string jsonString1 = VariableManager::jsonToString(jsonObject1);
//
//    std::string picha = VariableManager::getInstance()->returnVariableValue(jsonString1);

//    //PRUEBA ASIGNAR DIRECCIÓN A PUNTEROS Y DELLOCAR PUNTERO
//    //crear referencia
//    Json::Value jsonObject;
//    jsonObject["nombre"] = "pointer0";
//    jsonObject["tipoDeReferencia"] = "int";
//    jsonObject["tipoDeDato"] = "referencia";
//    jsonObject["espacioEnMemoriaDeTipoDeReferencia"] = 4;
//    std::string jsonString = VariableManager::jsonToString(jsonObject);
//    VariableManager::getInstance()->createVariable(jsonString);
//
//    Json::Value jsonObject4;
//    jsonObject4["nombre"] = "pointer1";
//    jsonObject4["tipoDeReferencia"] = "int";
//    jsonObject4["tipoDeDato"] = "referencia";
//    jsonObject4["espacioEnMemoriaDeTipoDeReferencia"] = 4;
//    std::string jsonString4 = VariableManager::jsonToString(jsonObject4);
//    VariableManager::getInstance()->createVariable(jsonString4);
//
//    //crear variable
//    Json::Value jsonObject2;
//    jsonObject2["nombre"] = "num1";
//    jsonObject2["valor"] = 2345;
//    jsonObject2["tipoDeDato"] = "int";
//    std::string jsonString2 = VariableManager::jsonToString(jsonObject2);
//    VariableManager::getInstance()->createVariable(jsonString2);
//
//    //asignar valor
//    Json::Value jsonObject1;
//    jsonObject1["nombreDePuntero"] = "pointer0";
//    jsonObject1["nombreDeVariable"] = "num1";
//    std::string jsonString1 = VariableManager::jsonToString(jsonObject1);
//    std::string texto = VariableManager::getInstance()->assignAddress(jsonString1);
//    std::cout << texto << std::endl;
//
//    //asignar direccion a la que apunta un puntero a otro
//    Json::Value jsonObject5;
//    jsonObject5["nombreDePuntero"] = "pointer1";
//    jsonObject5["nombreDeVariable"] = "pointer0";
//    std::string jsonString5 = VariableManager::jsonToString(jsonObject5);
//    std::string texto1 = VariableManager::getInstance()->assignAddress(jsonString5);
//    std::cout << texto1 << std::endl;
//
//    //Dellocar referencia
//    Json::Value jsonObject3;
//    jsonObject3["nombre"] = "pointer1";
//    std::string jsonString3 = VariableManager::jsonToString(jsonObject3);
//    std::cout << VariableManager::dellocatePointer(jsonString3) << std::endl;
}