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
    std::string variableName;  //Nombre de la variable
    std::string structName;  //Nombre de la estructura a la que pertenece (si pertenece)
    std::string pointerType;  //Nombre de la estructura a la que pertenece (si pertenece)
    void* ptr;             //Puntero al dato almacenado
    Node *next;
public:
    Node(){
        structName = "";
        next = 0;
    }
    Node* getNext(){return next;}
    void setNext(Node* nodo){ next = nodo;}
    void* getPointer(){return ptr;}
    void setPtr(void* ptr){ Node::ptr = ptr;}
    std::string getVariableType() const {return dataType;}
    void setDataType(std::string varType) { Node::dataType = varType;}
    const std::string &getVarName() const {return variableName;}
    void setVariableName(const std::string &varName) { Node::variableName = varName;}
    void setStructName(std::string structName) {
        Node::structName= structName;
    }

    const std::string &getDataType() const {
        return dataType;
    }

    void setPointerType(std::string pointerType) {
        Node::pointerType = pointerType;
    }

    const std::string &getPointerType() const {
        return pointerType;
    }
    void setPointerPointer(void* pVoid){
        if (pointerType == "")
            perror("Puntero sin tipo, tal vez esta variable no sea un puntero");
        *((void**)ptr) = pVoid;
    }
    void* getPointerPointer(){
        if(pointerType=="")
            perror("Puntero sin tipo, tal vez esta variable no sea un puntero");
        void* pVoid = *(void**)ptr;
        return pVoid;
    }
};
#endif //DATOS_II_PROYECTO_1_SERVIDOR_NODE_H

