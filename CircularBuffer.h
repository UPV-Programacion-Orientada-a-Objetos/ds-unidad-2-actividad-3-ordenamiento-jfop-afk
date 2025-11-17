#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

// Nodo de la lista circular doblemente enlazada
struct Node {
    int data;
    Node* next;
    Node* prev;
    
    Node(int val) {
        data = val;
        next = NULL;
        prev = NULL;
    }
};

// Buffer circular implementado como lista circular doblemente enlazada
class CircularBuffer {
private:
    Node* head;
    int capacity;
    int currentSize;
    
    // Ordena los datos dentro del buffer usando InsertionSort
    void sortInternal();
    
public:
    // Constructor
    CircularBuffer(int size);
    
    // Destructor
    ~CircularBuffer();
    
    // Inserta un elemento en el buffer
    bool insert(int value);
    
    // Verifica si el buffer esta lleno
    bool isFull();
    
    // Verifica si el buffer esta vacio
    bool isEmpty();
    
    // Obtiene el tamano actual
    int size();
    
    // Ordena y escribe el contenido a un archivo
    void sortAndWriteToFile(const char* filename);
    
    // Limpia el buffer
    void clear();
    
    // Imprime el contenido del buffer
    void print();
};

#endif