//
// Created by yordi on 1/4/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_SOCKET_H
#define DATOS_II_PROYECTO_1_SERVIDOR_SOCKET_H
#include <json/json.h>
#include <json//writer.h>
#include "VariableManager.h"

class Socket {
    private:
        static const int PORT = 2508; //Port. 25 del cumple de Yordi y 08 del cumple de Ricardo. UwU
    public:
    static void listen();
    static void send(std::string);
    static std::string jsonToString(Json::Value jsonObject);
    static Json::Value stringToJson(std::string);

};


#endif //DATOS_II_PROYECTO_1_SERVIDOR_SOCKET_H
