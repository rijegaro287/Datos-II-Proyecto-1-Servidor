//
// Created by yordi on 28/3/21.
//

#ifndef DATOS_II_PROYECTO_1_SERVIDOR_MEMORYPOOL_H
#define DATOS_II_PROYECTO_1_SERVIDOR_MEMORYPOOL_H

#include "MemoryBlock.h"
#include "MemoryNode.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>



    static const std::size_t DEFAULT_MEMORY_POOL_SIZE = 1000;                          //Bytes de memomoria default
    static const std::size_t DEFAULT_MEMORY_NODE_SIZE = 128;                           //Tamaño default de cada nodo de momoria
    static const std::size_t DEFAULT_MEMORY_SIZE_TO_ALLOCATE =
            DEFAULT_MEMORY_NODE_SIZE * 2;

    class MemoryPool : public MemoryBlock {
    private:
        static MemoryPool* memoryPool;
    public:
        MemoryNode *ptrFirstNode;  //Puntero al primer nodo de memoria (puntero del malloc inicial).
        MemoryNode *ptrLastNode;   //Puntero al último nodo de memoria.
        MemoryNode *ptrCursorNode; // Cursor-Node. Para agilizar el desplazamiento por la lista
        std::size_t TotalMemoryPoolSize;  //Memoria Total del MemoryPool
        std::size_t UsedMemoryPoolSize;   //Memoria usada
        std::size_t FreeMemoryPoolSize;   //Memoria libre
        std::size_t MemoryNodeSize;     //Memoria por nodo
        unsigned int MemoryNodeCount;  //Numero de nodos de momoria en el memori pool
        unsigned int ObjectCount;       //Numero de objetos en el memory pool (#getMemory()-#freeMemory())
        bool SetMemoryData;                      //Si es igual al true, al alocar o delocar setea la memoria a un valor definido (Para debugging)
        std::size_t MinimalMemorySizeToAllocate;

        MemoryPool(const std::size_t &MemoryPoolSize = DEFAULT_MEMORY_POOL_SIZE,
                   const std::size_t &MemoryNodeSize = DEFAULT_MEMORY_NODE_SIZE,
                   const std::size_t &MinimalMemorySizeToAllocate = DEFAULT_MEMORY_SIZE_TO_ALLOCATE,
                   bool SetMemoryData = false);
        virtual ~MemoryPool();
        static MemoryPool* getInstance();
        virtual void *GetMemory(const std::size_t &MemorySize);
        virtual void freeMemory(void *ptrMemoryBlock);
        bool IsValidPointer(void *ptrPointer);
        bool AllocateMemory(const std::size_t &MemorySize);
        void FreeAllAllocatedMemory();
        unsigned int CalculateNeededNodes(const std::size_t &MemorySize);
        std::size_t CalculateBestMemoryBlockSize(const std::size_t &RequestedMemoryBlockSize);
        MemoryNode *FindNodeSuitableToHoldMemory(const std::size_t &MemorySize);
        MemoryNode *FindNodeHoldingPointerTo(void *ptrMemoryBlock);
        MemoryNode *SkipNodes(MemoryNode *ptrStartNode, unsigned int NodesToSkip);
        MemoryNode *SetNodeDefaults(MemoryNode *ptrChunk);
        void FreeNodes(MemoryNode *ptrNode);
        void DeallocateAllChunks();
        bool LinkNodeToData(MemoryNode *ptrNewNodes, unsigned int uiNodeCount, TByte *ptrNewMemBlock);
        void SetMemoryNodeValues(MemoryNode *ptrNode, const std::size_t &MemBlockSize);
        bool RecalcNodeMemorySize(MemoryNode *ptrNodes, unsigned int uiNodeCount);
        void reduceRefenceCount(void *ptr);
        void checkReferences();
};

#endif //DATOS_II_PROYECTO_1_SERVIDOR_MEMORYPOOL_H
