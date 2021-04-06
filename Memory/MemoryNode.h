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

/*
 * Nodo vinculado a un bloque de memoria, cada nodo almacena el puntero del bloque
 * y la dirección del siguiente puntero.
 */
typedef struct MemoryNode {
    TByte *Data;            //El dato (puntero)
    std::size_t DataSize;   //Tamaño del bloque
    std::size_t UsedSize;   //Tamaño usado
    bool IsAllocationNode; // Nodo con el puntero del bloque entero de memoia
    MemoryNode *Next;     // Puntero al siguiente MemoryNode
} MemoryNode;


#endif //DATOS_II_PROYECTO_1_SERVIDOR_MEMORYNODE_H
