#ifndef DATASOURCE_H
#define DATASOURCE_H

// Clase Base Abstracta para fuentes de datos
class DataSource {
public:
    // Metodo virtual puro para obtener el siguiente dato
    virtual int getNext() = 0;
    
    // Metodo virtual puro para verificar si hay mas datos
    virtual bool hasMoreData() = 0;
    
    // Destructor virtual (OBLIGATORIO para herencia)
    virtual ~DataSource() {}
};

#endif