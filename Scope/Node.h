//
// Created by yordi on 29/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_NODE_H
#define DATOS_II_PROYECTO_1_SERVIDOR_NODE_H

//template <typename T>

#include <string>

class Node{
private:
    std::string dataType; //Tipo de dato
    std::string varName;  //Nombre de la variable
    void* ptr;             //Puntero al dato almacenado
    Node *next;
public:
    Node(){
        next = 0;
        ptr = 0;
    }
    Node* getNext(){return next;}
    void setNext(Node* nodo){ next = nodo;}
    void* getPtr(){return ptr;}
    void setPtr(void* ptr){ Node::ptr = ptr;}
    std::string getVarType() const {return dataType;}
    void setVarType(std::string varType) {Node::dataType = varType;}
    const std::string &getVarName() const {return varName;}
    void setVarName(const std::string &varName) {Node::varName = varName;}

};
#endif //DATOS_II_PROYECTO_1_SERVIDOR_NODE_H

