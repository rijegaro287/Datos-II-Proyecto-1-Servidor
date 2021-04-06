//
// Created by yordi on 29/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_SCOPE_H
#define DATOS_II_PROYECTO_1_SERVIDOR_SCOPE_H

#include "Node.h"
#include "Memory/MemoryPool.h"
#include <string>

class Scope{
private:
    Node *head;
    int length;
public:
    Scope();
    ~Scope();
    void add(void* ptr, std::string varType, std::string varName);
    void print();
    int getLength();
    void deleteInPos(int pos);
    void freeAllMemory();
    Node* searchNode(std::string varName);
};


#endif //DATOS_II_PROYECTO_1_SERVIDOR_SCOPE_H
