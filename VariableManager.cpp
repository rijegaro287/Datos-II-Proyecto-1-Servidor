//
// Created by yordi on 29/3/21.
//

#include "VariableManager.h"

/*
 * Gestiona la creacion y eliminación de scopes y sus variables.
 * Procesa lo que recibe la clase Socket.
 * FALTA:
 *      -Almacenar punteros y structs.
 *      -Identificar erroes.
 *      -Enviar mensajes de error por medio de la clase Socket.
 */

VariableManager* VariableManager::variableManager = nullptr;
std::list<Scope *> VariableManager::scopes;

VariableManager::VariableManager() {
    Scope* list = new Scope();
    scopes.push_front(list);
    mainScope = scopes.front();
}

VariableManager* VariableManager::getInstance() {
    if(variableManager == nullptr){
        variableManager = new VariableManager();
    }
    return variableManager;
}

/*
 * Añade una variable nueva al scope
 */
void VariableManager::add(void* ptr, std::string dataType,std::string varName) {
    Scope* list = scopes.front();
    list->add(ptr, dataType, varName);
}

/*
 * Elimina el scope más reciente
 */
void VariableManager::endScope(){
    Scope* lastScope = scopes.front();
    lastScope->freeAllMemory();
    if (lastScope != mainScope){
        delete lastScope;
        scopes.pop_front();
    }
}

/*
 * Crea un Scope
 */
void VariableManager::newScope() {
    Scope* list = new Scope();
    scopes.push_front(list);
}

/*
 * Busca un Node con el mismo nombre de variable que el de entreada.
 * Recorre todos los scopes hasta encontrarlo. Si no encuentra la variable
 * devuleve un nullptr.
 * FALTA:
 *      -Que el problema verifique si el nodo es nulo y envié
 *      un error al IDE.
 */
Node* VariableManager::searchNode(std::string varName) {
    Node* nodo = nullptr;
    for (auto list : scopes){
        nodo = list->searchNode(varName);
        if (nodo)
            break;
    }
    return nodo;
}

/*
 * Asigna cada atributo del nodo a un ObjetoJson y lo envía
 * por medio de Stack.
 * FALTA:
 *      -Que envie structs y punteros
 */
void VariableManager:: sendNode(Node* node){
    Json::Value jsonObject;
    jsonObject["type"] = node->getVarType();
    jsonObject["name"] = node->getVarName();
    uintptr_t addr = reinterpret_cast<uintptr_t>(node->getPtr());
    std::cout << addr << std::endl;
    jsonObject["address"] = addr;
    if(node->getVarType() == "int"){
        int* p =(int*) node->getPtr();
        jsonObject["value"] = *p;
    }else if (node->getVarType() == "char"){
        char* p =(char*) node->getPtr();
        jsonObject["value"] = *p;
    }else if (node->getVarType() == "long"){
        long* p =(long*) node->getPtr();
        jsonObject["value"] = *p;
    }else if (node->getVarType() == "float"){
        float* p =(float*) node->getPtr();
        jsonObject["value"] = *p;
    }else if (node->getVarType() == "double"){
        double* p =(double*) node->getPtr();
        jsonObject["value"] = *p;
    }
    std::cout << jsonObject << std:: endl;
    std::string jsonString = Socket::jsonToString(jsonObject);
    Socket::send(jsonString);
}

/*
 * Gestiona lo que recibe el socket. Recibe objetos Json y
 * los añade a un scope. De momento crea variables.
 * Falta:
 *      -Guardar datos tipo long.
 *      -Gestionar pedidos de variables existentes.
 *      -Gestionar pedidos de creación y cierre de scopes.
 *      -Gestionar pedidos de creacion de punteros y structs.
 */
void VariableManager::manageRequest(Json::Value jsonObject) {
    std::string name = jsonObject.get("name", "noName").asString();
    std::string dataType = jsonObject.get("type", "typeError").asString();
    void* ptr;
    if(dataType == "int"){
        int value = jsonObject.get("value", "ValueError").asInt();
        ptr = MemoryPool::getInstance()->GetMemory(4);
        (*(int*)ptr) = value;
    }else if (dataType == "char") {
        std::string stringChar = jsonObject.get("value", "ValueError").asString();
        const char *ptrChar = stringChar.c_str();
        char value = *ptrChar;
        ptr = MemoryPool::getInstance()->GetMemory(1);
        (*(char*)ptr) = value;
//    }else if (dataType == "long"){
//        int value = jsonObject.get("value", "ValueError").asL
    }else if (dataType == "float"){
        float value = jsonObject.get("value", "ValueError").asFloat();
        ptr = MemoryPool::getInstance()->GetMemory(4);
        (*(float*)ptr) = value;
    }else if (dataType == "double"){
        double value = jsonObject.get("value", "ValueError").asDouble();
        ptr = MemoryPool::getInstance()->GetMemory(8);
        (*(double*)ptr) = value;
    }
    add(ptr,dataType, name);
}


