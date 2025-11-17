#include "FileSource.h"
#include <iostream>

FileSource::FileSource(const char* filename) {
    file = fopen(filename, "r");
    hasData = false;
    nextValue = 0;
    
    if (file == NULL) {
        std::cerr << "Error: No se pudo abrir " << filename << std::endl;
        return;
    }
    
    // Leer el primer valor
    hasData = readNextValue();
}

FileSource::~FileSource() {
    if (file != NULL) {
        fclose(file);
    }
}

bool FileSource::readNextValue() {
    if (file == NULL) {
        return false;
    }
    
    if (fscanf(file, "%d", &nextValue) == 1) {
        return true;
    }
    
    return false;
}

int FileSource::getNext() {
    int current = nextValue;
    hasData = readNextValue();
    return current;
}

bool FileSource::hasMoreData() {
    return hasData;
}