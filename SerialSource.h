#ifndef SERIALSOURCE_H
#define SERIALSOURCE_H

#include "DataSource.h"
#include <windows.h>

// Clase para leer datos desde el puerto serial (Arduino)
class SerialSource : public DataSource {
private:
    HANDLE hSerial;
    bool connected;
    char buffer[256];
    int bufferIndex;
    bool dataAvailable;
    
    // Lee una linea completa del serial
    bool readLine(char* output, int maxLen);
    
public:
    // Constructor: conecta al puerto COM
    SerialSource(const char* portName);
    
    // Destructor: cierra la conexion serial
    ~SerialSource();
    
    // Implementacion de metodos virtuales
    int getNext();
    bool hasMoreData();
    
    // Verifica si la conexion esta activa
    bool isConnected();
};

#endif