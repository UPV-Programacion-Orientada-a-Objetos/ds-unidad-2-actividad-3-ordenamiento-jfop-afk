#include <iostream>
#include "SerialSource.h"
#include "ExternalSort.h"

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "Sistema de Ordenamiento Externo E-Sort" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    // Configuracion
    const char* portName = "COM3";  // Cambiar segun tu puerto
    const int bufferSize = 1000;    // Tamano del buffer circular
    const char* outputFile = "output.sorted.txt";
    
    // Conectar a Arduino
    std::cout << "\nConectando a puerto " << portName << " (Arduino)... ";
    SerialSource* serialSource = new SerialSource(portName);
    
    if (!serialSource->isConnected()) {
        std::cerr << "Error: No se pudo conectar al Arduino." << std::endl;
        std::cerr << "Verifica que:" << std::endl;
        std::cerr << "  1. El Arduino este conectado al puerto correcto" << std::endl;
        std::cerr << "  2. El puerto " << portName << " sea el correcto" << std::endl;
        std::cerr << "  3. No haya otro programa usando el puerto" << std::endl;
        delete serialSource;
        return 1;
    }
    
    // Esperar un momento para estabilizar la conexion
    std::cout << "Conectado." << std::endl;
    std::cout << "Esperando datos del Arduino..." << std::endl;
    std::cout << "(Presiona RESET en el Arduino para comenzar)" << std::endl;
    
    // Crear el sistema de ordenamiento externo
    ExternalSort* sorter = new ExternalSort(bufferSize);
    
    // FASE 1: Adquisicion y Segmentacion
    int chunksCreated = sorter->phase1_acquisition(serialSource);
    
    // Cerrar conexion serial
    delete serialSource;
    
    if (chunksCreated == 0) {
        std::cout << "No se recibieron datos del Arduino." << std::endl;
        delete sorter;
        return 1;
    }
    
    // FASE 2: Fusion Externa (K-Way Merge)
    sorter->phase2_merge(outputFile);
    
    // Limpieza
    std::cout << "\nLiberando memoria... ";
    delete sorter;
    std::cout << "Sistema apagado." << std::endl;
    
    std::cout << "\n=========================================" << std::endl;
    std::cout << "Proceso completado exitosamente." << std::endl;
    std::cout << "Revisa el archivo: " << outputFile << std::endl;
    std::cout << "=========================================" << std::endl;
    
    return 0;
}