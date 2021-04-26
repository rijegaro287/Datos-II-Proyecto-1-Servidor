//
// Created by yordi on 29/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_SCOPE_H
#define DATOS_II_PROYECTO_1_SERVIDOR_SCOPE_H

#include "Node.h"
#include "Memory/MemoryPool.h"
#include <string>
#include <iostream>
#include <cassert>
#include <json/value.h>


class Scope{
private:
    Node *head;
    int length;
    void freeMemory(Node*);
public:
    Scope();
    ~Scope();
    void add(void* ptr, std::string varType, std::string varName);
    void addStruct(void* ptr, std::string varType, std::string, std::string structName);
    void print();
    int getLength();
    void deleteInPos(int pos);
    Json::Value freeAllMemory();
    Node* searchNode(std::string varName);
    Node* searchNode(void* ptrNode);

    void addPointer(void *pVoid, std::string dataType, std::string name, std::string pointerType);

    void checkReferenceCount();

    bool inn(std::string name);

    Node *getHead() const;

    Json::Value timeline();
};


#endif //DATOS_II_PROYECTO_1_SERVIDOR_SCOPE_H
