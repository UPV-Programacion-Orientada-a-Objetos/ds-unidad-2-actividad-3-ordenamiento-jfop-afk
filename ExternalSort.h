#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#include "DataSource.h"
#include "CircularBuffer.h"

// Clase que maneja el proceso completo de ordenamiento externo
class ExternalSort {
private:
    int bufferSize;
    int chunkCount;
    
    // Genera el nombre del archivo chunk
    void generateChunkName(int index, char* output);
    
public:
    // Constructor
    ExternalSort(int bufSize);
    
    // Destructor
    ~ExternalSort();
    
    // Fase 1: Adquisicion y segmentacion
    int phase1_acquisition(DataSource* source);
    
    // Fase 2: Fusion externa (K-Way Merge)
    void phase2_merge(const char* outputFile);
    
    // Obtiene el numero de chunks generados
    int getChunkCount();
};

#endif