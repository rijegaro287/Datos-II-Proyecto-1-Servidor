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
Json::Value VariableManager::jsonTimeline;

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
        Json::Value jsonObject = lastScope->freeAllMemory();
        scopes.pop_front();
        delete lastScope;
        return jsonToString(jsonObject);
    }else{
        return "Scope Error";
    }
}

/*
 * Crea un Scope
 */
std::string VariableManager::newScope() {
    MemoryPool::getInstance();
    Scope* list = new Scope();
    scopes.push_front(list);
    return "";
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
    std::cout<<scopes.size()<<std::endl;
    for (auto list : scopes){
        nodo = list->searchNode(varName);
        if (nodo)
            break;
    }
    return nodo;
}

Node* VariableManager::searchNode(void* ptr) {
    Node* nodo = nullptr;
    for (auto list : scopes){
        nodo = list->searchNode(ptr);
        if (nodo)
            break;
    }
    return nodo;
}

void* VariableManager::spaceForPrimitiveData(Json::Value jsonObject) {
    std::string name = "";
    std::string dataType = jsonObject.get("tipoDeDato", "typeError").asString();
    void* ptr;
    ptr = allocateValue(dataType, jsonObject);
    add(ptr,dataType, name);
    return ptr;
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

    Node *nodeIfVariableAlreadyExist = searchNode(name);
    if (nodeIfVariableAlreadyExist) {//Verifica si ya existe una variable con ese nombre
        perror("\"La variable ya existe\"");
        return "La variable ya existe";
    }

    void* ptr;
    //Verifica si la variable es un puntero
    if(dataType == "referencia"){
        std::string pointerType = jsonObject.get("tipoDeReferencia", "typeError").asString();
        ptr = MemoryPool::getInstance()->getMemory(4);
        void* pVoid;
        *((void**)ptr) = pVoid;
        addPointer(ptr,dataType, name, pointerType);
        std::string value = jsonObject.get("valor", "typeError").asString();
        if(value != "typeError"){
            Json::Value tmpObject;
            tmpObject["nombreDePuntero"] = name;
            tmpObject["nombreDeVariable"] = value;
            jsonObject = stringToJson(assignAddress(jsonToString(tmpObject)));
        }
    }else { //Si la variable es de un dato primitivo
        //Asigna al valor un puntero disponible
        ptr = allocateValue(dataType, stringToJson(jsonString));
        add(ptr,dataType, name);
        }

    //Devulve dirección
    std::ostringstream address;
    address << (void const *)ptr;
    std::string direccion = address.str();
    jsonObject["direccion"] = direccion;
    return jsonToString(jsonObject);
}

std::string VariableManager::returnVariableValue(std::string jsonString) {
    Json::Value jsonRequest = stringToJson(jsonString);
    std::string variableName = jsonRequest.get("nombre", "valueError").asString();
    Node* variableNode = searchNode(variableName);
    if(!variableNode) {
        perror("La variable no existe");
        return "La variable no existe";
    }
    Json::Value jsonVariable;
    jsonVariable["tipoDeDato"] = variableNode->getVariableType();
    std::ostringstream address;
    address << (void const *)variableNode->getPointer();
    std::string direccion = address.str();
    jsonVariable["direccion"] = direccion;

    jsonVariable = getPointerValue(jsonVariable, variableNode->getVariableType(), variableNode->getPointer());

    std::cout << jsonVariable << std:: endl;
    return jsonToString(jsonVariable);
}

std::string VariableManager::createStruct(std::string jsonString) {
    Json::Value jsonObject = stringToJson(jsonString);
    std::string structName = jsonObject.get("nombreDeStruct", "noName").asString();

    Node* nodeIfVariableAlreadyExist = searchNode(structName);
    if(nodeIfVariableAlreadyExist){ //Verifica si ya existe una variable con ese nombre
        perror("\"La variable ya existe\"");
        return "Nombre Invalido para Struct";
    }

    add(nullptr, "struct", structName);

    std::string otherStruct = jsonObject.get("asignarUnStruct", "noName").asString();
    Node* nodeOfOtherStruct = searchNode(otherStruct);
    if(nodeOfOtherStruct) {
        for (auto list : scopes) {
            Node *tmp = list->getHead();
            while (tmp != nullptr) {
                for (auto structs : tmp->getStructName()) {
                    if (structs == otherStruct)
                        tmp->setStructName(structName);

                    std::ostringstream address;
                    address << (void const *) tmp->getPointer();
                    std::string direccion = address.str();
                    jsonObject["variables"]["direccion"] = direccion;
                    jsonObject["variables"]["conteoDeReferencia"] = searchNode(tmp->getPointer())->getReferenceCount();
                    // No se si se aumenta el conteo de referencias
                }
                tmp = tmp->next;
            }
        }
        return jsonToString(jsonObject);
    }

    Json::Value variables = jsonObject["variables"];
    for (int i = 0; i < variables.size(); ++i ) {
        std::string dataType = variables[i].get("tipoDeDato", "typeError").asString();
        std::string nombre = variables[i].get("nombre", "typeError").asString();
        void* ptr = allocateValue(dataType, variables[i]);
        addStruct(ptr,dataType, nombre, structName);

        std::ostringstream address;
        address << (void const *)ptr;
        std::string direccion = address.str();
        jsonObject["variables"][i]["direccion"] = direccion;
        jsonObject["variables"][i]["conteoDeReferencias"] = searchNode(ptr)->getReferenceCount();
    }
    return jsonToString(jsonObject);
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
    //Guarda el puntero de la dirección a la que apunta el puntero antes de que se realice el cambio
    void* oldPointerAddress = nodeOfPointer->getPointerPointer(); //Dirección a la que apuntaba el puntero
    std::string pointerType = nodeOfPointer->getPointerType();

    //Encontrar el nodo de la variable
    std::string variableToAssign = jsonObject.get("nombreDeVariable", "NameError").asString();
    if (variableToAssign == "") { //Si no hay variable y asigna un dato.
        std::string dataType = jsonObject.get("tipoDeDato", "TypeError").asString();
        void* ptr = spaceForPrimitiveData(jsonObject);
        add(ptr, dataType, variableToAssign);
        nodeOfPointer->setPointerPointer(ptr);

        Node* primitiveData = searchNode(ptr);
        jsonObject["datoPrimitivoNuevo"]["name"] = "";
        jsonObject["datoPrimitivoNuevo"]["conteoDeReferencias"] = primitiveData->getReferenceCount();
        jsonObject["datoPrimitivoNuevo"]["valor"] = jsonObject.get("valor","ValueError");
        std::ostringstream address;
        address << (void const *)ptr;
        std::string direccion = address.str();
        jsonObject["datoPrimitivoNuevo"]["direccion"] = direccion;
        return jsonToString(jsonObject);
    }

    Node *nodeOfVariableToAssign = searchNode(variableToAssign);
    //Verifica que la variable exista
    if (!nodeOfVariableToAssign) {
        perror("Variable no encontrada");
        return "Variable no encontrada";
    }

    //Dos casos:
    if( nodeOfVariableToAssign->getPointerType() == "") { //Si la variable es de datos primitivos
        std::string variableType = nodeOfVariableToAssign->getDataType();

        if (variableType == pointerType) { //Si los tipos de datos son compatibles
            //Asignar direccion al puntero del nodo
            nodeOfPointer->setPointerPointer(nodeOfVariableToAssign->getPointer());
            nodeOfVariableToAssign->increaseReferenceCount();
            jsonObject["conteoDeReferenciasDeVariable"] = nodeOfVariableToAssign->getReferenceCount();
            std::ostringstream address;
            address << (void const *)nodeOfVariableToAssign->getPointer();
            std::string direccion = address.str();
            jsonObject["direccionDeVariable"] = direccion;
        } else {
            perror("Tipo de puntero no compatible");
            return "Tipo de puntero no compatible";
        }
    }else { //Si la variable es un puntero
        std::string VariablepointerType = nodeOfVariableToAssign->getPointerType();
        if (VariablepointerType == VariablepointerType) {
            //Se asigna la direccion a la que apunta el puntero2 al puntero1.
            nodeOfPointer->setPointerPointer(nodeOfVariableToAssign->getPointerPointer());
            Node *nodeOfVariablePointed = searchNode(nodeOfVariableToAssign->getPointerPointer());
            if (nodeOfVariablePointed) { //Si la variable almacena una variable o dato primitivo
                //Aumenta el conteo de referencias de la variable a la que se apuntaba
                nodeOfVariablePointed->increaseReferenceCount();
                jsonObject["conteoDeReferenciasDeVariable"] = nodeOfVariablePointed->getReferenceCount();
                std::ostringstream address;
                address << (void const *)nodeOfVariablePointed->getPointer();
                std::string direccion = address.str();
                jsonObject["direccionDeDatoApuntado"] = direccion;
            }
        }
    }

    //Reducir el conteo del dato que se dejó de apuntar
    Node* nodeOfOldPointerAddress = searchNode(oldPointerAddress);
    if(nodeOfOldPointerAddress) {
        nodeOfOldPointerAddress->decreaseCount();
        jsonObject["datoAntiguo"]["nombre"] = nodeOfOldPointerAddress->getVariableName();
        jsonObject["datoAntiguo"]["conteoDeReferencias"] = nodeOfOldPointerAddress->getReferenceCount();
    }
    jsonObject["conteoDeReferenciasDePuntero"] = searchNode(pointerName)->getReferenceCount();
    return jsonToString(jsonObject);
}

std::string VariableManager::dereferencePointer(std::string jsonString){
    Json::Value jsonObject = stringToJson(jsonString);
    std::string pointerName = jsonObject.get("nombre", "NameError").asString();

    Node* nodeOfPointer = searchNode(pointerName);
    if (!nodeOfPointer){
        perror("Puntero no encontrado");
        return "Puntero no encontrado";
    }
    std::string pointerType = nodeOfPointer->getPointerType();

    Json::Value jsonDellocatedPointer;
    std::ostringstream address;
    address << (void const *)nodeOfPointer->getPointer();
    std::string direccion = address.str();
    jsonDellocatedPointer["direccion"] = direccion;
    jsonDellocatedPointer["tipoDeDato"] = pointerType;

    void* ptr = nodeOfPointer->getPointerPointer();

    jsonDellocatedPointer = getPointerValue(jsonDellocatedPointer, pointerType, ptr);

    return jsonToString(jsonDellocatedPointer);
}

void *VariableManager::allocateValue(std::string dataType, Json::Value jsonObject) {
    void* ptr;
    if(dataType == "int"){
        int value = jsonObject.get("valor", "ValueError").asInt();
        ptr = MemoryPool::getInstance()->getMemory(4);
        (*(int*)ptr) = value;
    }else if (dataType == "char") {
        std::string stringChar = jsonObject.get("valor", "ValueError").asString();
        const char *ptrChar = stringChar.c_str();
        char value = *ptrChar;
        ptr = MemoryPool::getInstance()->getMemory(1);
        (*(char*)ptr) = value;
    }else if (dataType == "long"){
        long value = jsonObject.get("valor", "ValueError").asLargestInt();
        ptr = MemoryPool::getInstance()->getMemory(4);
        (*(long*)ptr) = value;
    }else if (dataType == "float"){
        float value = jsonObject.get("valor", "ValueError").asFloat();
        ptr = MemoryPool::getInstance()->getMemory(4);
        (*(float*)ptr) = value;
    }else if (dataType == "double"){
        double value = jsonObject.get("valor", "ValueError").asDouble();
        ptr = MemoryPool::getInstance()->getMemory(8);
        (*(double*)ptr) = value;
    }
    return ptr;
}

std::string VariableManager::updateVariableValue(std::string jsonString){
    Json::Value jsonObject = stringToJson(jsonString);
    std::string name = jsonObject.get("nombre", "NameError").asString();

    Node* variableNode = searchNode(name);
    if (!variableNode){
        return "Variable no encontrada";
    }
    variableNode->setPointerValue(jsonObject);
    return jsonToString(jsonObject);
}

Json::Value VariableManager::getPointerValue(Json::Value jsonObject, std::string variableType, void* pointer) {
    if (variableType == "int") {
        int *p = (int *) pointer;
        jsonObject["valor"] = *p;
    } else if (variableType == "char") {
        char *p = (char *) pointer;
        std::string letra(1, *p);
        jsonObject["valor"] = letra;
    } else if (variableType == "long") {
        long *p = (long *) pointer;
        jsonObject["valor"] = *p;
    } else if (variableType == "float") {
        float *p = (float *) pointer;
        jsonObject["valor"] = *p;
    } else if (variableType == "double") {
        double *p = (double *) pointer;
        jsonObject["valor"] = *p;
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

void VariableManager::searchAndReduceReferenceCount(void * ptrNode) {
    Node* node = searchNode(ptrNode);
    if (node)
        node->decreaseCount();
    }

std::string VariableManager::returnAddress(std::string jsonString) {
    Json::Value jsonObject = stringToJson(jsonString);
    std::string name = jsonObject.get("nombre", "NameError").asString();
    Node* variableNode = searchNode(name);
    if (!variableNode){
        return "Variable no encontrada";
    }
    void* ptr = variableNode->getPointer();
    std::ostringstream address;
    address << (void const *)ptr;
    std::string direccion = address.str();
    jsonObject["direccion"] = direccion;
    jsonObject["tipoDeDato"] = variableNode->getDataType();
    return jsonToString(jsonObject);
}

void VariableManager::endRun() {
    if (scopes.size() > 1){
        perror("Scope Error, más de un scope al final del programa");
    }
    Json::Value jsonObject = mainScope->freeAllMemory();
    MemoryPool::getInstance()->setAllNodesEmpty();
}

std::string VariableManager::returnStructAttribute(std::string jsonString) {
    Json::Value jsonObject = stringToJson(jsonString);
    std::string structName = jsonObject.get("nombreDeStruct", "NameError").asString();
    std::string name = jsonObject.get("nombre", "NameError").asString();
    Node* node = searchNode(name);

    if (!node){
        perror("Variable no encontrada");
        return "Variable no encontrada";
    }
    bool flag = false;
    for (auto name : node->getStructName()) {
        if (name == structName){
            flag = true;
            break;
        }
    }
    if(flag == false){
        perror("Variable no encontrada");
        return "Variable no encontrada";
    }


    std::string dataType = node->getDataType();
    void* ptr = node->getPointer();

    jsonObject = getPointerValue(jsonObject, dataType, ptr);

    return jsonToString(jsonObject);
}

//std::string VariableManager::overview(std::string jsonString) {
// overview =
//}



