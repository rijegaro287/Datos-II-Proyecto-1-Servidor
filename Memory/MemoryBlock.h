//
// Created by yordi on 28/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_MEMORYBLOCK_H
#define DATOS_II_PROYECTO_1_SERVIDOR_MEMORYBLOCK_H


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>

typedef unsigned char TByte;

/*
 * Interfaz del MemoryPool
 */
class MemoryBlock {
public:
    virtual ~MemoryBlock() {};
    virtual void *GetMemory(const std::size_t &sMemorySize) = 0;
    virtual void FreeMemory(void *ptrMemoryBlock) = 0;
};


#endif //DATOS_II_PROYECTO_1_SERVIDOR_MEMORYBLOCK_H
