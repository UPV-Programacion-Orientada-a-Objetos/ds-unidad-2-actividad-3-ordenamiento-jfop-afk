#include "SerialSource.h"
#include <iostream>
#include <cstdlib>

SerialSource::SerialSource(const char* portName) {
    connected = false;
    bufferIndex = 0;
    dataAvailable = true;
    
    // Construir el nombre completo del puerto (ej: "\\\\.\\COM3")
    char fullPortName[20];
    int i = 0;
    fullPortName[i++] = '\\';
    fullPortName[i++] = '\\';
    fullPortName[i++] = '.';
    fullPortName[i++] = '\\';
    
    // Copiar el nombre del puerto
    int j = 0;
    while (portName[j] != '\0' && i < 19) {
        fullPortName[i++] = portName[j++];
    }
    fullPortName[i] = '\0';
    
    // Intentar abrir el puerto serial
    hSerial = CreateFileA(fullPortName,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: No se pudo abrir " << portName << std::endl;
        return;
    }
    
    // Configurar parametros del puerto serial
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error al obtener estado del puerto" << std::endl;
        CloseHandle(hSerial);
        return;
    }
    
    // Configurar: 9600 baud, 8 bits, sin paridad, 1 bit de parada
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error al configurar puerto" << std::endl;
        CloseHandle(hSerial);
        return;
    }
    
    // Configurar timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error al configurar timeouts" << std::endl;
        CloseHandle(hSerial);
        return;
    }
    
    connected = true;
    std::cout << "Conectado a " << portName << std::endl;
}

SerialSource::~SerialSource() {
    if (connected) {
        CloseHandle(hSerial);
    }
}

bool SerialSource::readLine(char* output, int maxLen) {
    char temp;
    DWORD bytesRead;
    int index = 0;
    
    while (index < maxLen - 1) {
        if (ReadFile(hSerial, &temp, 1, &bytesRead, NULL)) {
            if (bytesRead == 1) {
                if (temp == '\n') {
                    output[index] = '\0';
                    return true;
                }
                if (temp != '\r') {
                    output[index++] = temp;
                }
            }
        } else {
            return false;
        }
    }
    
    output[index] = '\0';
    return false;
}

int SerialSource::getNext() {
    if (!connected || !dataAvailable) {
        return 0;
    }
    
    char line[256];
    if (readLine(line, 256)) {
        // Convertir string a entero usando atoi
        return atoi(line);
    }
    
    dataAvailable = false;
    return 0;
}

bool SerialSource::hasMoreData() {
    return connected && dataAvailable;
}

bool SerialSource::isConnected() {
    return connected;
}