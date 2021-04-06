#include "Communication/Socket.h"
#include <json/json.h>
#include "Memory/MemoryPool.h"
#include "VariableManager.h"


using namespace std;
int main(){
//    VariableManager list;
//    int* pInt, *pInt2;
//    double* pDouble;
//    long* pLong;
//    pInt = (int*)MemoryPool::getInstance()->GetMemory(4);
//    *pInt = 7823;
//    pDouble = (double*)MemoryPool::getInstance()->GetMemory(8);
//    pLong = (long*)MemoryPool::getInstance()->GetMemory(8);
//    list.add(pInt, "int", "num1");
//    list.add(pDouble, "double", "num2");
//    list.add(pLong, "long", "num3");

//
//    list.newScope();
//    pInt2 = (int*) MemoryPool::getInstance()->GetMemory(4);
//    *pInt2 = 923;
//    list.add(pInt2, "int", "num1");
//
//    Node* nodo = list.searchNode("num1");
//    if(!nodo){
//        perror("No se en contró la variable");
//    }
//    list.sendNode(nodo);
//
//    Json::Value JsonObjEscrito;
//    JsonObjEscrito["name"] = "num1";
//    JsonObjEscrito["type"] = "int";
//    JsonObjEscrito["value"] = 7;
//    VariableManager vm;
//    VariableManager::getInstance()->manageRequest(JsonObjEscrito);
//    Node* node =VariableManager::getInstance()->searchNode("num1");
//    std::cout<<*(int*)(node->getPtr())<<std::endl;


    VariableManager::getInstance();
    Socket::listen();  //Está pendiente de envios.
    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl; //si le llega una variable, automaticamente se guarda en el MemoryPool y el scope
                                                                  //Imprime el número de objetos guardados en el MemoryPool

    Node* node = VariableManager::getInstance()->searchNode("num1"); //Devuelve el nodo de la variable con ese nombre
    std::cout<< *(int*)node->getPtr()<<std::endl; //Printea el value del nodo
    VariableManager::getInstance()->endScope();  //Elimina las variables en el scope
    Node* node2 = VariableManager::getInstance()->searchNode("num1");
    if (!node2) //Devuleve un nullptr porque ya no existe un nodo con ese nombre de variable
        std::cout<<"no hay"<<std::endl;
    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl; //Printea el número de objetos en el MemoryPool.
    return 0;
}