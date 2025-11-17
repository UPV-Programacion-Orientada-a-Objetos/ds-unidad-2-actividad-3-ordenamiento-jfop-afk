#include "ExternalSort.h"
#include "FileSource.h"
#include <iostream>
#include <cstdio>

ExternalSort::ExternalSort(int bufSize) {
    bufferSize = bufSize;
    chunkCount = 0;
}

ExternalSort::~ExternalSort() {
}

void ExternalSort::generateChunkName(int index, char* output) {
    // Generar nombre como "chunk_0.tmp", "chunk_1.tmp", etc.
    const char* prefix = "chunk_";
    const char* suffix = ".tmp";
    
    int i = 0;
    // Copiar prefijo
    const char* p = prefix;
    while (*p != '\0') {
        output[i++] = *p++;
    }
    
    // Convertir indice a string
    char numStr[10];
    int numLen = 0;
    int temp = index;
    
    if (temp == 0) {
        numStr[numLen++] = '0';
    } else {
        while (temp > 0) {
            numStr[numLen++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    // Invertir el numero
    for (int j = numLen - 1; j >= 0; j--) {
        output[i++] = numStr[j];
    }
    
    // Copiar sufijo
    p = suffix;
    while (*p != '\0') {
        output[i++] = *p++;
    }
    output[i] = '\0';
}

int ExternalSort::phase1_acquisition(DataSource* source) {
    std::cout << "\nIniciando Fase 1: Adquisicion de datos...\n" << std::endl;
    
    CircularBuffer* buffer = new CircularBuffer(bufferSize);
    chunkCount = 0;
    
    while (source->hasMoreData()) {
        int value = source->getNext();
        std::cout << "Leyendo -> " << value << std::endl;
        
        if (!buffer->insert(value)) {
            // Buffer lleno, ordenar y escribir
            std::cout << "Buffer lleno. ";
            
            char filename[50];
            generateChunkName(chunkCount, filename);
            
            buffer->sortAndWriteToFile(filename);
            buffer->clear();
            chunkCount++;
            
            std::cout << std::endl;
            
            // Insertar el valor actual en el buffer limpio
            buffer->insert(value);
        }
    }
    
    // Escribir el ultimo chunk si hay datos restantes
    if (!buffer->isEmpty()) {
        std::cout << "Buffer parcialmente lleno. ";
        
        char filename[50];
        generateChunkName(chunkCount, filename);
        
        buffer->sortAndWriteToFile(filename);
        chunkCount++;
    }
    
    delete buffer;
    
    std::cout << "\nFase 1 completada. " << chunkCount << " chunks generados.\n" << std::endl;
    return chunkCount;
}

void ExternalSort::phase2_merge(const char* outputFile) {
    std::cout << "Iniciando Fase 2: Fusion Externa (K-Way Merge)" << std::endl;
    
    if (chunkCount == 0) {
        std::cout << "No hay chunks para fusionar." << std::endl;
        return;
    }
    
    // Crear arreglo de punteros a DataSource (FileSource)
    DataSource** sources = new DataSource*[chunkCount];
    int* currentValues = new int[chunkCount];
    bool* hasData = new bool[chunkCount];
    
    // Abrir todos los archivos chunk
    std::cout << "Abriendo " << chunkCount << " archivos fuente..." << std::endl;
    for (int i = 0; i < chunkCount; i++) {
        char filename[50];
        generateChunkName(i, filename);
        
        sources[i] = new FileSource(filename);
        hasData[i] = sources[i]->hasMoreData();
        if (hasData[i]) {
            currentValues[i] = sources[i]->getNext();
        }
    }
    
    // Abrir archivo de salida
    FILE* outputF = fopen(outputFile, "w");
    if (outputF == NULL) {
        std::cerr << "Error: No se pudo crear " << outputFile << std::endl;
        return;
    }
    
    std::cout << "K=" << chunkCount << ". Fusion en progreso..." << std::endl;
    
    // K-Way Merge
    int totalWritten = 0;
    while (true) {
        // Buscar el minimo entre todos los valores actuales
        int minIndex = -1;
        int minValue = 0;
        
        for (int i = 0; i < chunkCount; i++) {
            if (hasData[i]) {
                if (minIndex == -1 || currentValues[i] < minValue) {
                    minValue = currentValues[i];
                    minIndex = i;
                }
            }
        }
        
        // Si no hay mas datos, terminar
        if (minIndex == -1) {
            break;
        }
        
        // Escribir el minimo al archivo de salida
        fprintf(outputF, "%d\n", minValue);
        totalWritten++;
        
        // Mostrar progreso cada ciertos elementos
        if (totalWritten <= 20 || totalWritten % 100 == 0) {
            std::cout << " - Escribiendo " << minValue << std::endl;
        }
        
        // Avanzar en el FileSource del cual se extrajo el minimo
        if (sources[minIndex]->hasMoreData()) {
            currentValues[minIndex] = sources[minIndex]->getNext();
        } else {
            hasData[minIndex] = false;
        }
    }
    
    fclose(outputF);
    
    // Liberar memoria de los FileSource
    for (int i = 0; i < chunkCount; i++) {
        delete sources[i];
    }
    delete[] sources;
    delete[] currentValues;
    delete[] hasData;
    
    std::cout << "\nFusion completada. Archivo final: " << outputFile << std::endl;
    std::cout << "Total de elementos escritos: " << totalWritten << std::endl;
}

int ExternalSort::getChunkCount() {
    return chunkCount;
}