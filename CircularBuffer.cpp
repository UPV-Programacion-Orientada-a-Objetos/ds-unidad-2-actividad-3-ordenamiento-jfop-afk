#include "CircularBuffer.h"
#include <iostream>
#include <cstdio>

CircularBuffer::CircularBuffer(int size) {
    capacity = size;
    currentSize = 0;
    head = NULL;
}

CircularBuffer::~CircularBuffer() {
    clear();
}

bool CircularBuffer::insert(int value) {
    if (isFull()) {
        return false;
    }
    
    Node* newNode = new Node(value);
    
    if (head == NULL) {
        // Primer nodo: apunta a si mismo
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        // Insertar al final (antes del head)
        Node* tail = head->prev;
        
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = head;
        head->prev = newNode;
    }
    
    currentSize++;
    return true;
}

bool CircularBuffer::isFull() {
    return currentSize >= capacity;
}

bool CircularBuffer::isEmpty() {
    return currentSize == 0;
}

int CircularBuffer::size() {
    return currentSize;
}

void CircularBuffer::sortInternal() {
    if (head == NULL || currentSize <= 1) {
        return;
    }
    
    // Insertion Sort sobre la lista circular
    Node* sorted = NULL;
    Node* current = head;
    
    // Romper la circularidad temporalmente
    head->prev->next = NULL;
    
    while (current != NULL) {
        Node* next = current->next;
        
        if (sorted == NULL || sorted->data >= current->data) {
            // Insertar al inicio
            current->next = sorted;
            current->prev = NULL;
            if (sorted != NULL) {
                sorted->prev = current;
            }
            sorted = current;
        } else {
            // Buscar posicion correcta
            Node* temp = sorted;
            while (temp->next != NULL && temp->next->data < current->data) {
                temp = temp->next;
            }
            
            current->next = temp->next;
            if (temp->next != NULL) {
                temp->next->prev = current;
            }
            temp->next = current;
            current->prev = temp;
        }
        
        current = next;
    }
    
    // Restaurar circularidad
    head = sorted;
    Node* tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = head;
    head->prev = tail;
}

void CircularBuffer::sortAndWriteToFile(const char* filename) {
    if (isEmpty()) {
        return;
    }
    
    // Ordenar internamente
    std::cout << "Ordenando internamente..." << std::endl;
    sortInternal();
    
    // Mostrar contenido ordenado
    std::cout << "Buffer ordenado: [";
    Node* temp = head;
    for (int i = 0; i < currentSize; i++) {
        std::cout << temp->data;
        if (i < currentSize - 1) {
            std::cout << ", ";
        }
        temp = temp->next;
    }
    std::cout << "]" << std::endl;
    
    // Escribir a archivo
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        std::cerr << "Error: No se pudo crear " << filename << std::endl;
        return;
    }
    
    temp = head;
    for (int i = 0; i < currentSize; i++) {
        fprintf(file, "%d\n", temp->data);
        temp = temp->next;
    }
    
    fclose(file);
    std::cout << "Escribiendo " << filename << "... OK." << std::endl;
}

void CircularBuffer::clear() {
    if (head == NULL) {
        return;
    }
    
    // Romper la circularidad
    head->prev->next = NULL;
    
    // Eliminar todos los nodos
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    
    head = NULL;
    currentSize = 0;
    std::cout << "Buffer limpiado." << std::endl;
}

void CircularBuffer::print() {
    if (isEmpty()) {
        std::cout << "Buffer vacio" << std::endl;
        return;
    }
    
    std::cout << "Buffer: [";
    Node* temp = head;
    for (int i = 0; i < currentSize; i++) {
        std::cout << temp->data;
        if (i < currentSize - 1) {
            std::cout << ", ";
        }
        temp = temp->next;
    }
    std::cout << "]" << std::endl;
}