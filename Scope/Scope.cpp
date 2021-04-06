//
// Created by yordi on 29/3/21.
//

#include <iostream>
#include <cassert>
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
    Node* tmp = head;
    for (int i = 0; i < length; ++i) {
        if(varName ==tmp->getVarName()){
            perror("\"La variable ya existe\"");
        }
        tmp->setNext(tmp->getNext());
    }
    Node* newNode = new Node();
    newNode->setPtr(ptr);
    newNode->setVarType(dataType);
    newNode->setVarName(varName);
    newNode->setNext(nullptr);
    if (length == 0) {
        head = newNode;
    } else {
        newNode->setNext(head);
        head = newNode;
    }
    length++;
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
            text = text + " " + temp->getVarName();
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
void Scope::freeAllMemory() {
    Node* tmp = head;
    while (tmp != nullptr){
        MemoryPool::getInstance()->FreeMemory(tmp->getPtr());
        tmp->setPtr(nullptr);
        tmp = tmp->getNext();
    }
    int initialLength = length;
    for (int i = 0; i < initialLength; ++i) {
        deleteInPos(0);
    }
    assert((length == 0) && "WARNING : Memory-Leak : You have not freed all allocated Memory");
}

/*
 * Devuleve el Node donde con el nombre de la variable de entrada.
 * Si el nodo no existe devuelve un nullptr.
 */
Node* Scope::searchNode(std::string varName) {
    Node* tmp = head;
    Node* seekedNode = nullptr;
    for (int i = 0; i < length; ++i) {
        if (varName==tmp->getVarName()){
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



