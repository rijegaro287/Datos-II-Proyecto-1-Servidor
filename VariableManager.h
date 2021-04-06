//
// Created by yordi on 29/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
#define DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H

#include <list>
#include "Scope/Scope.h"
#include <json/json.h>
#include <Communication/Socket.h>

class VariableManager {
    private:
        Scope* mainScope;
        static std::list<Scope *> scopes;
        static VariableManager* variableManager;
    public:
        static VariableManager* getInstance();
        VariableManager();
        void endScope();
        void newScope();
        Node* searchNode(std::string varName);
        static void add(void* ptr, std::string varType, std::string varName);
        void sendNode(Node*);
        void lisenForRequest();
        void static manageRequest(Json::Value);
};



#endif //DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
