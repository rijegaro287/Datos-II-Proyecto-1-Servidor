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
        void checkReferenceCount();
        std::string endScope();
        std::string newScope();
    //        Node* searchNode(std::string variableName);
        static Node* searchNode(void* ptr);
        static void* setVariableValueToMemomery(std::string dataType, Json::Value jsonObject);
        static Json::Value getPointerValue(Json::Value jsonObject, std::string variableType, void *pointer);
        void searchAndReduceReferenceCount(void*);

public:
    static const std::list<Scope *> &getScopes();

    static VariableManager* getInstance();
        static Node* searchNode(std::string varName);
        static std::string createVariable(std::string jsonString);
        std::string returnVariableValue(std::string);
        std::string createStruct(std::string);
        std::string assignAddress(std::string jsonString);
        static std::string dereferencePointer(std::string jsonString);
        std::string updateScopes(std::string scope);


        static std::string jsonToString(Json::Value);

//        static std::string jsonToString(Json::Value);
        static Json::Value stringToJson(std::string);

        std::string dereferenceAndSetPointerValue(std::string jsonString);

        std::string updateVariableValue(std::string jsonString);

    std::string returnAddress(std::string jsonString);

    void endRun();

    void *spaceForPrimitiveData(std::string jsonString);

    void *spaceForPrimitiveData(Json::Value jsonObject);
};



#endif //DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
