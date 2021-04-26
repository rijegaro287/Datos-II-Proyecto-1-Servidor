#ifndef DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
#define DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H

#include <list>
#include "Scope/Scope.h"
#include <json/json.h>
#include "Communication/HTTPServer.h"


class VariableManager {
    private:
        static VariableManager* variableManager;
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
        static void* allocateValue(std::string dataType, Json::Value jsonObject);
        static Json::Value getPointerValue(Json::Value jsonObject, std::string variableType, void *pointer);
        void searchAndReduceReferenceCount(void*);

public:
    static const std::list<Scope *> &getScopes();

    static VariableManager* getInstance();
        static Node* searchNode(std::string varName);
        static std::string createVariable(std::string jsonString);
        std::string returnVariableValue(std::string);
        std::string createStruct(std::string);
        static std::string assignAddress(std::string jsonString);
        static std::string dereferencePointer(std::string jsonString);
        std::string updateScopes(std::string scope);


        static std::string jsonToString(Json::Value);

//        static std::string jsonToString(Json::Value);
        static Json::Value stringToJson(std::string);

        std::string dereferenceAndSetPointerValue(std::string jsonString);

        std::string updateVariableValue(std::string jsonString);

    std::string returnAddress(std::string jsonString);

    void endRun();

//    static void *spaceForPrimitiveData(std::string jsonString);

    static void *spaceForPrimitiveData(Json::Value jsonObject);

    std::string returnStructAttribute(std::string jsonString);


    static Json::Value jsonTimeline;
    static std::list<Scope *> scopes;

    std::string overview(std::string jsonString);
};



#endif //DATOS_II_PROYECTO_1_SERVIDOR_VARIABLEMANAGER_H
