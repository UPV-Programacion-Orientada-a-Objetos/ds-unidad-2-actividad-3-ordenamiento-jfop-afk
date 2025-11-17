#ifndef FILESOURCE_H
#define FILESOURCE_H

#include "DataSource.h"
#include <cstdio>

// Clase para leer datos desde archivos
class FileSource : public DataSource {
private:
    FILE* file;
    bool hasData;
    int nextValue;
    
    // Lee el siguiente valor del archivo
    bool readNextValue();
    
public:
    // Constructor: abre el archivo
    FileSource(const char* filename);
    
    // Destructor: cierra el archivo
    ~FileSource();
    
    // Implementacion de metodos virtuales
    int getNext();
    bool hasMoreData();
};

#endif