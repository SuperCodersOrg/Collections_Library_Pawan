#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdexcept>
#include <iostream>

template <typename T>
class LinkedList {
private:
    // Node structure for the linked list
    struct Node {
        T data;
        Node* next;
        
        Node(const T& val) : data(val), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    // Default constructor
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor
    ~LinkedList() {
        clear();
    }

    // --- State Queries ---
    
    [[nodiscard]] int getSize() const { return size; }
    [[nodiscard]] bool isEmpty() const { return size == 0; }

    // --- Modifiers ---

    void prepend(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    void append(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            head = newNode;
            // FIXED: Set tail as well when appending to empty list!
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    // (Remaining methods will be implemented in subsequent steps)
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
};

#endif // LINKED_LIST_H
