

#include "Scope.h"

/*
 * Lista Simplemente enlazada que se encarga de almacenar las variables creadas por scope.
 */

Scope::Scope(){
    head = 0;
    length = 0;
}

Scope::~Scope() {
    freeAllMemory();
}

/*
 * Agrega una variable nueva a la lista.
 * Recibe el puntero al dato, el tipo de dato y el nombre de la variable.
 */
void Scope::add(void* ptr, std::string dataType, std::string varName){
    Node* newNode = new Node();
    newNode->setPtr(ptr);
    newNode->increaseCount();
    newNode->setDataType(dataType);
    newNode->setVariableName(varName);
    newNode->setNext(nullptr);
    if (length == 0) {
        head = newNode;
    } else {
        newNode->setNext(head);
        head = newNode;
    }
    length++;
}

void Scope::freeMemory(Node* node){
    Node* tmp = head;
    while(tmp != nullptr){
        if(tmp == node)
            break;
        tmp->setNext(tmp->getNext());
    }
    if (tmp == nullptr)
        perror("Nodo no existe");
    else if (tmp == head) {
        head = head->getNext();
        tmp->setNext(NULL);
        delete tmp;
    }else if (tmp->getNext() == nullptr){
        Node* cur = head;
        while(cur->getNext() != tmp){
            cur->setNext(cur->getNext());
        }
        cur->setNext(nullptr);
        delete tmp;
    }
    else{
        Node* cur = head;
        while(cur->getNext() != tmp){
            cur->setNext(cur->getNext());
        }
        tmp->next = nullptr;
        cur->next = cur->next->next;
        delete tmp;
    }
}

void Scope::checkReferenceCount(){
    Node* tmp = head;
    while(tmp != nullptr){
        if(tmp->getReferenceCount() == 0){
            freeMemory(tmp);
        }
       tmp = tmp->next;
    }
}

/*
 * Imprime el nombre de las variables almacenadas en el scope.
 */
void Scope::print(){
    if (length == 0) {
        std::cout << "Lista vacía" << std::endl;
    } else {
        Node *temp = head;
        std::string text = "Elementos en la lista: [";
        for (int i = 0; i < length; i++) {
            text = text + " " + temp->getVariableName();
            temp = temp->getNext();
        }
        text = text + " ]";
        std::cout << text << std::endl;
    }
}

/*
 * Elimina el elemento en la posición de entrada.
 */
void Scope::deleteInPos(int pos){
    if (length != 0) {
        if (pos > (length - 1) or pos < 0) {
            std::cout << "Posición inválida" << std::endl;
        } else {
            if (pos == 0) {
                Node *tmp = head;
                head = head->getNext();
                tmp->setNext(NULL);
                delete tmp;
            } else if (pos == length - 1) {
                Node *tmp = head;
                for (int i = 0; i != pos - 1; i++) {
                    tmp = tmp->getNext();
                }
                delete tmp->getNext();
                tmp->setNext(NULL);
            } else {
                Node *tmp = head;
                for (int i = 0; i != pos - 1; i++) {
                    tmp = tmp->getNext();
                }
                Node* deleteNode = tmp->getNext();
                tmp->setNext(deleteNode->getNext());
                deleteNode->setNext(NULL);
                delete deleteNode;
            }
            length--;
        }
    } else {
        std::cout << "Lista Vacía" << std::endl;
    }
}

/*
 * Elimina todos los elementos de la lista.
 */
Json::Value Scope::freeAllMemory() {
    Json::Value jsonObject;
    Node* tmp = head;
    int i = 0;
    while (tmp != nullptr){
        if(tmp->getPointerType() != "") {
//            MemoryPool::getInstance()->reduceRefenceCount(tmp->getPointerPointer());
            Node *nodeOfVariablePointed = searchNode(tmp->getPointerPointer());
            if (nodeOfVariablePointed)
                nodeOfVariablePointed->decreaseCount();
            else {
                //Aquí se llega si la dirección a la que apunta el segundo puntero no es una variable
            }
        }
        jsonObject["nombreDeVariableEliminada"][i] = tmp->getVariableName();
        MemoryPool::getInstance()->freeMemory(tmp->getPointer());
        tmp->setPtr(nullptr);
        tmp = tmp->getNext();
        i++;
    }
    int initialLength = length;
    for (int i = 0; i < initialLength; ++i) {
        deleteInPos(0);
    }
    assert((length == 0) && "WARNING : Memory-Leak : You have not freed all allocated Memory");
    return jsonObject;
}

/*
 * Devuleve el Node donde con el nombre de la variable de entrada.
 * Si el nodo no existe devuelve un nullptr.
 */
Node* Scope::searchNode(std::string varName) {
    Node* tmp = head;
    Node* seekedNode = nullptr;
    for (int i = 0; i < length; ++i) {
        if (varName== tmp->getVariableName()){
            seekedNode = tmp;
            break;
        }
        tmp = tmp->getNext();
    }
    return seekedNode;
}

Node* Scope::searchNode(void* ptr) {
    Node* tmp = head;
    Node* seekedNode = nullptr;
    for (int i = 0; i < length; ++i) {
        if (ptr == tmp->getPointer()){
            seekedNode = tmp;
            break;
        }
        tmp = tmp->getNext();
    }
    return seekedNode;
}

int Scope::getLength(){
    return length;
}

void Scope::addStruct(void *ptr, std::string dataType, std::string name, std::string structName) {
    Node* tmp = head;
    for (int i = 0; i < length; ++i) {
        if(name == tmp->getVariableName()){
            perror("\"La variable ya existe\"");
        }
        tmp->setNext(tmp->getNext());
    }
    Node* newNode = new Node();
    newNode->setPtr(ptr);
    newNode->setDataType(dataType);
    newNode->setVariableName(name);
    newNode->increaseCount();
    newNode->setStructName(structName);
    newNode->setNext(nullptr);
    if (length == 0) {
        head = newNode;
    } else {
        newNode->setNext(head);
        head = newNode;
    }
    length++;
}

void Scope::addPointer(void *ptr, std::string dataType, std::string name, std::string pointerType) {
    Node* tmp = head;
    for (int i = 0; i < length; ++i) {
        if(name == tmp->getVariableName()){
            perror("\"La variable ya existe\"");
        }
        tmp->setNext(tmp->getNext());
    }
    Node* newNode = new Node();
    newNode->setPtr(ptr);
    newNode->setDataType(dataType);
    newNode->setVariableName(name);
    newNode->setPointerType(pointerType);
    newNode->increaseCount();
    newNode->setNext(nullptr);
    if (length == 0) {
        head = newNode;
    } else {
        newNode->setNext(head);
        head = newNode;
    }
    length++;
}

bool Scope::inn(std::string name){
    Node* tmp = head;
    bool inn = false;
    for (int i = 0; i < length; ++i) {
        if(name == tmp->getVariableName()){
            perror("\"La variable ya existe\"");
            inn = true;
            break;
        }
        tmp->setNext(tmp->getNext());
    }
    if (inn){
        return true;
    }else {
        return false;
    }
}





