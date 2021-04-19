//
// Created by yordi on 29/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
#define DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H

#include <list>
#include "Scope/Scope.h"
#include <json/json.h>
#include "Communication/HTTPServer.h"

class VariableManager {
    private:
        static VariableManager* variableManager;
        static std::list<Scope *> scopes;
        Scope* mainScope;

        VariableManager();
        static void add(void*, std::string, std::string);
        void addStruct(void*, std::string, std::string, std::string);
        static void addPointer(void *ptr, std::string basicString, std::string basicString1, std::string basicString2);
        std::string endScope();
        std::string newScope();
    //        Node* searchNode(std::string variableName);
        static void* setVariableValueToMemomery(std::string dataType, Json::Value jsonObject);
        static Json::Value getPointerValue(Json::Value jsonObject, std::string variableType, void *pointer);

public:
        static VariableManager* getInstance();
        static Node* searchNode(std::string varName);
        static std::string createVariable(std::string);
        std::string returnVariableValue(std::string);
        std::string createStruct(std::string);
        std::string assignAddress(std::string jsonString);
        static std::string dellocatePointer(std::string jsonString);
        std::string updateScopes(std::string scope);


        static std::string jsonToString(Json::Value);

//        static std::string jsonToString(Json::Value);
        static Json::Value stringToJson(std::string);

    std::string dellocateAndSetPointerValue(std::string jsonString);
};



#endif //DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
