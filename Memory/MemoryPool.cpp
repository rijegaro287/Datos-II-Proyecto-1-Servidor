//
// Created by yordi on 28/3/21.
//

#include <cstring>
#include "MemoryNode.h"
#include "MemoryPool.h"

    static const int FREEED_MEMORY_CONTENT        = 0xAA ;
    static const int NEW_ALLOCATED_MEMORY_CONTENT = 0xFF ;
    MemoryPool* MemoryPool::memoryPool = nullptr;


    MemoryPool::MemoryPool(const std::size_t &MemoryPoolSize,
                             const std::size_t &MemoryNodeSize,
                             const std::size_t &MinimalMemorySizeToAllocate,
                             bool SetMemoryData){
        ptrFirstNode  = NULL ;
        ptrLastNode   = NULL ;
        ptrCursorNode = NULL ;

        TotalMemoryPoolSize = 0 ;
        UsedMemoryPoolSize  = 0 ;
        FreeMemoryPoolSize  = 0 ;

        MemoryPool::MemoryNodeSize   = MemoryNodeSize ;
        MemoryNodeCount = 0 ;
        ObjectCount      = 0 ;

        MemoryPool::SetMemoryData               = SetMemoryData ;
        MemoryPool::MinimalMemorySizeToAllocate = MinimalMemorySizeToAllocate ; //ESTO SE USA PARA PEDIRLE MÁS ESPACIO AL S.O, YO NO LO USO

        // Allcate del big memory block
        AllocateMemory(MemoryPoolSize);
//        std::cout<<"Servidor eschuchando el puerto: " <<9999 << " Espacio en memeoria creado: "<< std::to_string((int) MemoryPoolSize) << std::endl;
    }

    MemoryPool::~MemoryPool()
    {
        freeAllAllocatedMemory() ;
        DeallocateAllNodes() ;

        // Verifica memory-leaks
        assert((ObjectCount == 0) && "WARNING : Memory-Leak : No se ha liberado toda la memoria") ;
    }

    MemoryPool* MemoryPool::getInstance() {
        if(memoryPool == nullptr){
            memoryPool = new MemoryPool();
        }
        return memoryPool;
    }

/*
 * Busca un espacio en el MemoryPool que pueda almacenar la memeoria pedida.
 * Devuelve un puntero vacio a la dirección de memoria del MemoryPool que puede
 * almacenar esa memoria.
*/
    void *MemoryPool::getMemory(const std::size_t &MemorySize)
    {
        std::size_t sBestMemBlockSize = CalculateBestMemoryBlockSize(MemorySize) ;
        MemoryNode *ptrNode = NULL ;
        if(!ptrNode){
            // Hay un nodo disponible que pueda almacenar esa memoria?
            ptrNode = FindNodeSuitableToHoldMemory(sBestMemBlockSize) ;
            if(!ptrNode){
                //sBestMemBlockSize = MaxValue(sBestMemBlockSize, CalculateBestMemoryBlockSize(MinimalMemorySizeToAllocate)) ;
                //AllocateMemory(sBestMemBlockSize) ;
                std::cout<<"Se mamut, no hay nodos que puedan almacenar ese espacio de memoria"<<std::endl;
            }
        }

        // Se encuentra un nodo contenedor
        // Ajusta los valores de memorya usada y memoria libre
        UsedMemoryPoolSize += sBestMemBlockSize ;
        FreeMemoryPoolSize -= sBestMemBlockSize ;
        ObjectCount++ ;
        SetMemoryNodeValues(ptrNode, sBestMemBlockSize) ;

        // Devuelve el puntero al espacio de memoria
        return ((void *) ptrNode->data) ;
    }

/*
 * Libera la memoria del MemoryPool, haciendo que esté disponible para futuras
 * requests.
 * Recibe el puntero de memoria donde se almacena la memoria por borrar.
 */
    void MemoryPool::freeMemory(void *ptrMemoryBlock) //, const std::size_t &sMemoryBlockSize)
    {
        //Busca el nodo que contiene el puntero por borrar
        MemoryNode *ptrNode = FindNodeHoldingPointerTo(ptrMemoryBlock) ;
        if(ptrNode)
        {
            freeNodes(ptrNode) ;
        }
        else
        {
            assert(false && "ERROR : El puntero no se encuantra en el MemoryPool") ;
        }
        assert((ObjectCount >= 0) && "ERROR : Request to delete more Memory then allocated.") ;
//        ObjectCount-- ;
    }

    bool MemoryPool::AllocateMemory(const std::size_t &MemorySize)
    {
        unsigned int NeededNodes = CalculateNeededNodes(MemorySize) ;
        std::size_t BestMemBlockSize = CalculateBestMemoryBlockSize(MemorySize) ;

        TByte *ptrNewMemBlock = (TByte *) malloc(BestMemBlockSize) ; // Malloc inicial
        MemoryNode *ptrNewNodes = (MemoryNode *) malloc((NeededNodes * sizeof(MemoryNode))) ; // Asigna el espacio para los MemoryNodes
        assert(((ptrNewMemBlock) && (ptrNewNodes)) && "Error : System ran out of Memory") ;

        // Ajusta los valores de memoria (Total/Free Memory, etc.)
        TotalMemoryPoolSize += BestMemBlockSize ;
        FreeMemoryPoolSize += BestMemBlockSize ;
        MemoryNodeCount += NeededNodes ;

        // Para el debugging
        if(SetMemoryData)
        {
            memset(((void *) ptrNewMemBlock), NEW_ALLOCATED_MEMORY_CONTENT, BestMemBlockSize) ;
        }

        // Asocia la memoria asignada al MemoryBlock con los MemoryNodes
        return LinkNodeToData(ptrNewNodes, NeededNodes, ptrNewMemBlock) ; ;
    }

/*
 * Calcula el numero de MemoryNodes necesarios para alamacenar el espacio de memoria
 * de entrada.
 */
    unsigned int MemoryPool::CalculateNeededNodes(const std::size_t &MemorySize)
    {
        float f = (float) (((float)MemorySize) / ((float)MemoryNodeSize)) ;
        return ((unsigned int) ceil(f)) ;
    }

    std::size_t MemoryPool::CalculateBestMemoryBlockSize(const std::size_t &RequestedMemoryBlockSize)
    {
        unsigned int NeededNodes = CalculateNeededNodes(RequestedMemoryBlockSize) ;
        return std::size_t((NeededNodes * MemoryNodeSize)) ;
    }

/*
 * Hace que el MemoryNode esté disponible para almacenar otra vez.
 * Recibe un puntero de tipo MemoryNode.
 */
    void MemoryPool::freeNodes(MemoryNode *ptrNode)
    {
        // Hacer que el nodo de memoria esté disponible de nuevo

        MemoryNode *ptrCurrentNode = ptrNode ;
        unsigned int uiChunkCount = CalculateNeededNodes(ptrCurrentNode->usedSize);
        for(unsigned int i = 0; i < uiChunkCount; i++)
        {
            if(ptrCurrentNode)
            {
                // Step 1 : Set la llocated memory to 'FREEED_MEMORY_CONTENT'
                // Note : Opcional pero sirve para el debugging
                if(SetMemoryData)
                {
                    memset(((void *) ptrCurrentNode->data), FREEED_MEMORY_CONTENT, MemoryNodeSize) ;
                }

                // Step 2 : Set espacio usado a 0
                ptrCurrentNode->usedSize = 0 ;
//                ptrCurrentNode->referenceCount = 0 ;

                // Step 3 : Ajustar los valores de memoria de la pool y pasa al siguiente Node
                UsedMemoryPoolSize -= MemoryNodeSize ;
                ObjectCount--;
                ptrCurrentNode = ptrCurrentNode->next ;
            }
        }
    }


/*
 * Busca un(unos) MemoryNodes que puedan almacenar la memoria.
 * Recibe el tamaño necesitado en bytes.
 */
    MemoryNode *MemoryPool::FindNodeSuitableToHoldMemory(const std::size_t &MemorySize)
    {
        // Encuentra un nodo que al menos pueda almacenar MemorySize
        unsigned int NodesToSkip = 0 ;
        bool ContinueSearch = true ;
        MemoryNode *ptrNode = ptrCursorNode ; // Empiece a buscar desde el ptrCursorNode
        for(unsigned int i = 0; i < MemoryNodeCount; i++)
        {
            if(ptrNode)
            {
                if(ptrNode == ptrLastNode) // Final de la lista : Comience desde el inicio
                {
                    ptrNode = ptrFirstNode ;
                }

                if(ptrNode->dataSize >= MemorySize)
                {
                    if(ptrNode->usedSize == 0)
                    {
                        ptrCursorNode = ptrNode ;
                        return ptrNode ;
                    }
                }
                NodesToSkip = CalculateNeededNodes(ptrNode->usedSize) ;
                if(NodesToSkip == 0) NodesToSkip = 1 ;
                ptrNode = SkipNodes(ptrNode, NodesToSkip) ;
            }
            else
            {
                ContinueSearch = false ;
            }
        }
        return NULL ;
    }

/*
 * Omite cierto numero de MemoryNodes. Recibe un puntero de MemoryNode y el número
 * de MemoryNodes a omitir.
 */
    MemoryNode *MemoryPool::SkipNodes(MemoryNode *ptrStartNode, unsigned int NodesToSkip)
    {
        MemoryNode *ptrCurrentNode = ptrStartNode ;
        for(unsigned int i = 0; i < NodesToSkip; i++)
        {
            if(ptrCurrentNode)
            {
                ptrCurrentNode = ptrCurrentNode->next ;
            }
            else
            {
                // Will occur, if you try to Skip more Chunks than actually available
                // from your "ptrStartNode"
                assert(false && "Error : Chunk == NULL was not expected.") ;
                break ;
            }
        }
        return ptrCurrentNode ;
    }

/*
 *Settear los valores en un MemoryNode.
 */
    void MemoryPool::SetMemoryNodeValues(MemoryNode *ptrNode, const std::size_t &MemBlockSize)
    {
        if((ptrNode)) // && (ptrNode != ptrLastNode))
        {
            ptrNode->usedSize = MemBlockSize;
//            ptrNode->referenceCount++;
        }
        else
        {
            assert(false && "Error : Invalid NULL-Pointer passed") ;
        }
    }

/*
 * Conecta cada MemoryNode a la MemoryPool.
 */
    bool MemoryPool::LinkNodeToData(MemoryNode *ptrNewNodes, unsigned int NodeCount, TByte *ptrNewMemBlock)
    {
        MemoryNode *ptrNewChunk = NULL ;
        unsigned int uiMemOffSet = 0 ;
        bool bAllocationNodeAssigned = false ;
        for(unsigned int i = 0; i < NodeCount; i++){
            if(!ptrFirstNode){
                ptrFirstNode = SetNodeDefaults(&(ptrNewNodes[0])) ;
                ptrLastNode = ptrFirstNode ;
                ptrCursorNode = ptrFirstNode ;
            }
            else{
                ptrNewChunk = SetNodeDefaults(&(ptrNewNodes[i])) ;
                ptrLastNode->next = ptrNewChunk ;
                ptrLastNode = ptrNewChunk ;
            }

            uiMemOffSet = (i * ((unsigned int) MemoryNodeSize)) ;
            ptrLastNode->data = &(ptrNewMemBlock[uiMemOffSet]) ;

            //El primer nodo asignado al MemoryPool es el
            //AllocartioNode. Este nodo almacena el puntero del malloc.
            //es el responsable de liberar la memoria luego.
            if(!bAllocationNodeAssigned){
                ptrLastNode->isAllocationNode = true ;
                bAllocationNodeAssigned = true ;
            }
        }
        return RecalcNodeMemorySize(ptrFirstNode, MemoryNodeCount) ;
    }

/*
 *
 */
    bool MemoryPool::RecalcNodeMemorySize(MemoryNode *ptrNodes, unsigned int NodeCount)
    {
        unsigned int uiMemOffSet = 0 ;
        for(unsigned int i = 0; i < NodeCount; i++)
        {
            if(ptrNodes)
            {
                uiMemOffSet = (i * ((unsigned int) MemoryNodeSize)) ;
                ptrNodes->dataSize = (((unsigned int) TotalMemoryPoolSize) - uiMemOffSet) ;
                ptrNodes = ptrNodes->next ;
            }
            else
            {
                assert(false && "Error : ptrChunk == NULL") ;
                return false ;
            }
        }
        return true ;
    }

/*
 *
 */
    MemoryNode *MemoryPool::SetNodeDefaults(MemoryNode *ptrNode)
    {
        if(ptrNode)
        {
            ptrNode->data = NULL ;
            ptrNode->dataSize = 0 ;
            ptrNode->usedSize = 0 ;
            ptrNode->isAllocationNode = false ;
            ptrNode->next = NULL ;
        }
        return ptrNode ;
    }

/*
 *
 */
    MemoryNode *MemoryPool::FindNodeHoldingPointerTo(void *ptrMemoryBlock){
        MemoryNode *ptrTempNode = ptrFirstNode ;
        while(ptrTempNode)
        {
            if(ptrTempNode->data == ((TByte *) ptrMemoryBlock))
            {
                break ;
            }
            ptrTempNode = ptrTempNode->next ;
        }
        return ptrTempNode ;
    }

/*
 *
 */
    void MemoryPool::freeAllAllocatedMemory()
    {
        MemoryNode *ptrNode = ptrFirstNode ;
        while(ptrNode)
        {
            if(ptrNode->isAllocationNode)
            {
                free(((void *) (ptrNode->data))) ;
            }
            ptrNode = ptrNode->next ;
        }
    }

/*
 *
 */
    void MemoryPool::DeallocateAllNodes()
    {
        MemoryNode *ptrChunk = ptrFirstNode ;
        MemoryNode *ptrChunkToDelete = NULL ;
        while(ptrChunk)
        {
            if(ptrChunk->isAllocationNode)
            {
                if(ptrChunkToDelete)
                {
                    free(((void *) ptrChunkToDelete)) ;
                }
                ptrChunkToDelete = ptrChunk ;
            }
            ptrChunk = ptrChunk->next ;
        }
    }

///*
// *
// */
//    bool MemoryPool::IsValidPointer(void *ptrPointer)
//    {
//        MemoryNode *ptrChunk = ptrFirstNode ;
//        while(ptrChunk)
//        {
//            if(ptrChunk->data == ((TByte *) ptrPointer))
//            {
//                return true ;
//            }
//            ptrChunk = ptrChunk->next ;
//        }
//        return false ;
//    }

//void MemoryPool::reduceRefenceCount(void* ptrNodes){
//    MemoryNode *ptrNode = FindNodeHoldingPointerTo(ptrNodes);
//    if(ptrNode)
//    {
//        ptrNode->referenceCount--;
//        assert ((ptrNode->referenceCount < 0) && "ERROR : El puntero no se encuantra en el MemoryPool");
//    }
//    else
//        assert(false && "ERROR : El puntero no se encuantra en el MemoryPool") ;
//    }

//void MemoryPool::checkReferences(){
//    MemoryNode *ptrTempNode = ptrFirstNode ;
//    while(ptrTempNode){
//        if(ptrTempNode->referenceCount == 0 and ptrTempNode->usedSize != 0)
//            freeMemory(ptrTempNode->data);
//    }
//    }

void MemoryPool::setAllNodesEmpty() {
        MemoryNode* tmpNode = ptrFirstNode;
        while(tmpNode != nullptr){
            freeNodes(tmpNode);
            tmpNode = tmpNode->next;
        }

}
