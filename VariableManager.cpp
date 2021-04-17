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

void VariableManager::addStruct(void * ptr, std::string dataType, std::string name, std::string structName) {
    Scope* list = scopes.front();
    list->addStruct(ptr, dataType, name, structName);
}

void VariableManager::addPointer(void *ptr, std::string dataType, std::string name, std::string pointerType) {
    Scope* list = scopes.front();
    list->addPointer(ptr, dataType, name, pointerType);
}

/*
 * Elimina el scope más reciente
 */
std::string VariableManager::endScope(){
    Scope* lastScope = scopes.front();
    if (lastScope != mainScope){
        lastScope->freeAllMemory();
        delete lastScope;
        scopes.pop_front();
        return "";
    }else{
        return "Scope Error";
    }
}

/*
 * Crea un Scope
 */
std::string VariableManager::newScope() {
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
 * Crea variables de referencia y datos primitivos
 * Recibe un string de un json con las caracteristicas
 * de la variable.
 */
std::string VariableManager::createVariable(std::string jsonString) {
    Json::Value jsonObject = stringToJson(jsonString);

    std::string name = jsonObject.get("nombre", "noName").asString();
    std::string dataType = jsonObject.get("tipoDeDato", "typeError").asString();

    void* ptr;
    //Verifica si la variable es un puntero
    if(dataType == "referencia"){
        std::string pointerType = jsonObject.get("tipoDeReferencia", "typeError").asString();
        ptr = MemoryPool::getInstance()->GetMemory(4);
        void* pVoid;
        *((void**)ptr) = pVoid;
        addPointer(ptr,dataType, name, pointerType);
    }else {
        //Asigna al valor un puntero disponible
        ptr = setVariableValueToMemomery(dataType, stringToJson(jsonString));
        add(ptr,dataType, name);
    }

    //Devulve dirección
    Json::Value jsonVariable;
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    jsonVariable["direccion"] = addr;
    return jsonToString(jsonVariable);
}

std::string VariableManager::returnVariableValue(std::string jsonString) {
    Json::Value jsonRequest = stringToJson(jsonString);
    std::string variableName = jsonRequest.get("nombre", "valueError").asString();
    Node* variableNode = searchNode(variableName);
    if(!variableNode) {
        perror("Variable Value Error");
        return "Variable Value Error";
    }
    Json::Value jsonVariable;
    jsonVariable["tipoDeDato"] = variableNode->getVariableType();
    uintptr_t addr = reinterpret_cast<uintptr_t>(variableNode->getPointer());
    jsonVariable["direccion"] = addr;

    jsonVariable = getPointerValue(jsonVariable, variableNode->getVariableType(), variableNode->getPointer());

    std::cout << jsonVariable << std:: endl;
    return jsonToString(jsonVariable);
}

std::string VariableManager::createStruct(std::string jsonString) {
    Json::Value jsonObject = jsonToString(jsonString);
    std::string structName = jsonObject.get("nombreDeStruct", "noName").asString();
    Json::Value variables = jsonObject["variables"];
    for (int i = 0; i < variables.size(); ++i )
    {
        std::string name = variables[i].get("nombre", "noName").asString();
        std::string dataType = variables[i].get("tipoDeDato", "typeError").asString();
        void* ptr = setVariableValueToMemomery(dataType, variables[i]);
        addStruct(ptr,dataType, name, structName);
    }
    return std::string();
}



std::string VariableManager::assignAddress(std::string jsonString) {
    Json::Value jsonObject = stringToJson(jsonString);

    //Encontrar el nodo del puntero
    std::string pointerName = jsonObject.get("nombreDePuntero", "NameError").asString();
    Node* nodeOfPointer = searchNode(pointerName);

    //verifica que la variable del puntero exista
    if (!nodeOfPointer){
        perror("Puntero no encontrado");
        return "Puntero no encontrado";
    }
    std::string pointerType = nodeOfPointer->getPointerType();

    //Encontrar el nodo de la variable
    std::string variableToAssign = jsonObject.get("nombreDeVariable", "NameError").asString();
    Node* nodeOfVariableToAssign = searchNode(variableToAssign);
    if (!nodeOfVariableToAssign){
        perror("Variable no encontrada");
        return "Variable no encontrada";
    }

    //Verifica si la variable es otro puntero
    if( nodeOfVariableToAssign->getPointerType() != ""){
        std::string pointerType2 =  nodeOfVariableToAssign->getPointerType();
        if (pointerType2 == pointerType){
            nodeOfPointer->setPointerPointer(nodeOfVariableToAssign->getPointerPointer());
            return "";
        }else{
            perror("Tipo de puntero no compatible");
            return "Tipo de puntero no compatible";
        }

    }else {
        std::string variableType = nodeOfVariableToAssign->getDataType();

        if (variableType == pointerType) {
            //Asignar direccion al puntero del nodo
            nodeOfPointer->setPointerPointer(nodeOfVariableToAssign->getPointer());
        } else {
            perror("Tipo de puntero no compatible");
            return "Tipo de puntero no compatible";
        }
        return "";
    }
}

std::string VariableManager::dellocatePointer(std::string jsonString){
    Json::Value jsonObject = stringToJson(jsonString);
    std::string pointerName = jsonObject.get("nombre", "NameError").asString();

    Node* nodeOfPointer = searchNode(pointerName);
    if (!nodeOfPointer){
        perror("Variable no encontrada");
        return "Variable no encontrada";
    }
    std::string pointerType = nodeOfPointer->getPointerType();

    Json::Value jsonDellocatedPointer;
    uintptr_t addr = reinterpret_cast<uintptr_t>(nodeOfPointer->getPointer());
    std::cout << addr << std::endl;
    jsonDellocatedPointer["direccion"] = addr;
    jsonDellocatedPointer["tipoDeDato"] = pointerType;

    void* ptr = nodeOfPointer->getPointerPointer();

    jsonDellocatedPointer = getPointerValue(jsonDellocatedPointer, pointerType, ptr);

    return jsonToString(jsonDellocatedPointer);
}

void *VariableManager::setVariableValueToMemomery(std::string dataType, Json::Value jsonObject) {
    void* ptr;
    if(dataType == "int"){
        int value = jsonObject.get("valor", "ValueError").asInt();
        ptr = MemoryPool::getInstance()->GetMemory(4);
        (*(int*)ptr) = value;
    }else if (dataType == "char") {
        std::string stringChar = jsonObject.get("valor", "ValueError").asString();
        const char *ptrChar = stringChar.c_str();
        char value = *ptrChar;
        ptr = MemoryPool::getInstance()->GetMemory(1);
        (*(char*)ptr) = value;
    }else if (dataType == "long"){
        long value = jsonObject.get("valor", "ValueError").asLargestInt();
        ptr = MemoryPool::getInstance()->GetMemory(4);
        (*(long*)ptr) = value;
    }else if (dataType == "float"){
        float value = jsonObject.get("valor", "ValueError").asFloat();
        ptr = MemoryPool::getInstance()->GetMemory(4);
        (*(float*)ptr) = value;
    }else if (dataType == "double"){
        double value = jsonObject.get("valor", "ValueError").asDouble();
        ptr = MemoryPool::getInstance()->GetMemory(8);
        (*(double*)ptr) = value;
    }
    return ptr;
}

Json::Value VariableManager::getPointerValue(Json::Value jsonObject, std::string variableType, void* pointer) {
    if (variableType == "int") {
        int *p = (int *) pointer;
        jsonObject["value"] = *p;
    } else if (variableType == "char") {
        char *p = (char *) pointer;
        std::string letra(1, *p);
        jsonObject["value"] = letra;
    } else if (variableType == "long") {
        long *p = (long *) pointer;
        jsonObject["value"] = *p;
    } else if (variableType == "float") {
        float *p = (float *) pointer;
        jsonObject["value"] = *p;
    } else if (variableType == "double") {
        double *p = (double *) pointer;
        jsonObject["value"] = *p;
    }
    return jsonObject;
}

std::string VariableManager::updateScopes(std::string scope){
    if ( scope == "{"){
        return newScope();
    }else if (scope == "}"){
        return endScope();
    }else{
        perror("Error al actualizar scopes");
    }
}

std::string VariableManager::jsonToString(Json::Value jsonObject) {
    Json::FastWriter writer;
    std::string jsonString = writer.write(jsonObject);
    std::cout << jsonString << std::endl;  //Pa probar
    return jsonString;
}

Json::Value VariableManager::stringToJson(std::string jsonString) {
    Json::Reader reader;
    Json::Value jsonObject;
    bool parsingSuccessful = reader.parse(jsonString, jsonObject, false);
    if (!parsingSuccessful){
        perror("stringToJson");
    }
    return jsonObject;
}

