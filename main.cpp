#include "Memory/InclusionesMP.h"
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
    Socket::listen();
    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl;
    Node* node = VariableManager::getInstance()->searchNode("num1");
    std::cout<< *(int*)node->getPtr()<<std::endl;
    VariableManager::getInstance()->endScope();
    Node* node2 = VariableManager::getInstance()->searchNode("num1");
    if (!node2)
        std::cout<<"no hay"<<std::endl;
    std::cout<<MemoryPool::getInstance()->ObjectCount<<std::endl;
    return 0;
}