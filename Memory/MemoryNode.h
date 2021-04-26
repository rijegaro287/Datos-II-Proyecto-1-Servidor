//
// Created by yordi on 28/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_MEMORYNODE_H
#define DATOS_II_PROYECTO_1_SERVIDOR_MEMORYNODE_H

#include "MemoryBlock.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>

/**
 * @file MemoryNode.h
 * @brief Estructura de los nodos que conforman el MemoryPool.
 * @version 1.0
 * @date 20/04/2021
 * @author Yordi Brenes
 * @title MemoryNode
 */
typedef struct MemoryNode {
    TByte *data;            //El dato (puntero)
    std::size_t dataSize;   //Tamaño del bloque
    std::size_t usedSize;   //Tamaño usado
    bool isAllocationNode; // Nodo con el puntero del bloque entero de memoria
    MemoryNode *next;     // Puntero al siguiente MemoryNode
} MemoryNode;


#endif //DATOS_II_PROYECTO_1_SERVIDOR_MEMORYNODE_H
