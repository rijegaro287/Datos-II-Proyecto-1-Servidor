//
// Created by yordi on 1/4/21.
//

#include "Socket.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <iostream>

/*
 * Envía objetos Json(strings) al cliente (IDe).
 * FALTA:
 *  -Que no solo envia datos de variables, si no errores, avisos, mensajes etc.
 */
void Socket::send(std::string jsonString) {

    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        //return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) < 0) {
        printf("\nInvalid address/ Address not supported \n");
        //return -1;
    }
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        //return -1;
    }

    char temp_buff[500];

    if (strcpy(temp_buff, jsonString.c_str()) == NULL) {
        perror("strcpy");
        //return EXIT_FAILURE;
    }

    if (write(sock, temp_buff, strlen(temp_buff)) == -1) {
        perror("write");
        //return EXIT_FAILURE;
    }

}

/*
 * Recibe Jsons(strings) del cliente (IDe).
 * FALTA:
 *      -El listen(), no se ejecuta al iniciar el programa.
 *      -Solo recibe datos para crear variables, no peticiones
 *      para crear scopes, ni el valor de variables existentes.
 */
    void Socket::listen(){
        int server_fd = 0, connfd = 0;
        struct sockaddr_in address;
        int opt = 1;

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (::listen(server_fd, 3) < 0){
            perror("listen");
            exit(EXIT_FAILURE);
        }

        connfd = accept(server_fd, (struct sockaddr*)NULL, NULL);

        ssize_t r;

        char buff[500];

        for (;;){
            r = read(connfd, buff, 500);
            if (r == -1)
            {
                perror("read");
            }
            if (r == 0) {
                Json::Value jsonObject = stringToJson(buff);
                VariableManager::manageRequest(jsonObject);
                break;
            }
        }
        printf("READ: %s\n", buff);
    }

std::string Socket::jsonToString(Json::Value jsonObject) {
    Json::FastWriter writer;
    std::string jsonString = writer.write(jsonObject);
    std::cout << jsonString << std::endl;
    return jsonString;
}

Json::Value Socket::stringToJson(std::string jsonString) {
    Json::Reader reader;
    Json::Value jsonObject;
    bool parsingSuccessful = reader.parse(jsonString, jsonObject, false);
    if (!parsingSuccessful){
        perror("stringToJson");
    }
    return jsonObject;
}






