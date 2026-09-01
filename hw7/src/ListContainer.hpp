#pragma once

#include <iostream>
#include <initializer_list>

template <typename T>
class ListContainer {
public:
    ListContainer() : l_size{0}, first_pntr{nullptr}, last_pntr{nullptr} {};

    void push_back(T value) {
        // создаем новый узел
        Node* new_node = new Node;
        
        new_node->prev = last_pntr;
        new_node->data = value;
        new_node->next = nullptr;

        // проверка на пустоту списка
        if (l_size == 0) {
            first_pntr = new_node;
            last_pntr = new_node;
        } else {
            last_pntr->next = new_node;
            last_pntr = new_node;
        }

        l_size += 1;
    }

    ListContainer(std::initializer_list<T> list_container_array) : ListContainer() {

        //копируем элементы из массива в контейнер
        const T* list_container_array_begin = list_container_array.begin();

        for (size_t i=0; i < list_container_array.size(); i++) {
            this->push_back(*(list_container_array_begin + i));
        }

    }

    ~ListContainer() {
        Node* element = first_pntr;
        
        while (element != nullptr) {
            Node* next_node = element->next;
            delete element;
            element = next_node;
        }
    }

    // копирование
    ListContainer(const ListContainer& other) {
        l_size = 0;
        first_pntr = nullptr;
        last_pntr = nullptr;

        Node* other_element = other.first_pntr;
        while (other_element != nullptr) {
            this->push_back(other_element->data);
            other_element = other_element->next;
        }
    }

    // копирующее присваивание 
    ListContainer& operator=(const ListContainer& other) {
        this->clear();

        l_size = 0;
        first_pntr = nullptr;
        last_pntr = nullptr;

        Node* other_element = other.first_pntr;
        while (other_element != nullptr) {
            this->push_back(other_element->data);
            other_element = other_element->next;
        }

        return *this;
        
    }

    // перемещение
    ListContainer(ListContainer&& other) {
        l_size = other.l_size;
        first_pntr = other.first_pntr;
        last_pntr = other.last_pntr;

        other.first_pntr = nullptr;
        other.last_pntr = nullptr;
        other.l_size = 0;
    }

    // копирующее перемещение 
    ListContainer& operator=(ListContainer&& other) {
        this->clear();

        l_size = other.l_size;
        first_pntr = other.first_pntr;
        last_pntr = other.last_pntr;

        other.first_pntr = nullptr;
        other.last_pntr = nullptr;
        other.l_size = 0;

        return *this;
    }

    void getInfoDev() const {
        // начнинаем с первого элемента
        Node* element = first_pntr;

        // идем по всем узлам
        do {
            std::cout << "Element = " << element->data << " has adress " << element << std::endl;
            element = element->next;
        }
        while (element != nullptr);
    }

    void print() const {
        Node* element = first_pntr;

        for (size_t i=0; i < l_size; i++) {
            if (i == l_size - 1) {
                std::cout << element->data << std::endl;
            } else {
                std::cout << element->data << ", ";
            }
            element = element->next;
        }
    }

    void printSize() const{
        std::cout << l_size << std::endl;
    }

    void erase(size_t delete_index) {
        // начинаем с первого элемента 
        Node* element = first_pntr;

        // идем по всем узлам до нужного индекса
        for (size_t i=0; i < delete_index; i++) {
            element = element->next;
        }

        // сшиваем указатели узлов
        if (l_size == 1) {
            // если элемент единственный
            first_pntr = nullptr;
            last_pntr = nullptr;
        } else if (delete_index == 0) {
            // меняем указатели первого узла
            first_pntr = element->next;
            first_pntr->prev = nullptr;
        } else if (delete_index == l_size - 1) {
            // меняем указатели последнего узла
            last_pntr = element->prev;
            last_pntr->next = nullptr;
        } else {
            // случай когда узел по середине
            element->prev->next = element->next;
            element->next->prev = element->prev;
        }

        delete element;
        l_size -= 1;
        

    }

    void insert(T value, size_t index) {
        Node* element = first_pntr;

        for (size_t i=0; i < index; i++) {
            element = element->next;
        }

        // добавляем новый узел
        Node* new_node = new Node;
        new_node->data = value;

        if (index == 0) {
            new_node->prev = nullptr;
            new_node->next = first_pntr;
            
            first_pntr->prev = new_node;
            first_pntr = new_node;

        } else if (index == l_size) {
            new_node->prev = last_pntr;
            new_node->next = nullptr;

            last_pntr->next = new_node;
            last_pntr = new_node;

        } else {
            new_node->next = element;
            new_node->prev = element->prev;

            element->prev->next = new_node;
            element->prev = new_node;
        }

        l_size += 1;
    } 

    T& operator[] (size_t index) {
        Node* element = first_pntr;

        for (size_t i=0; i < index; i++) {
            element = element->next;
        }

        return element->data;
    }

    size_t size() const {
        return l_size;
    }

    bool operator==(const ListContainer& other) const {
        if (l_size != other.size()) return false;

        Node* element1 = first_pntr;
        Node* element2 = other.first_pntr;

        while (element1 != nullptr) {
            if (element1->data != element2->data) {
                return false;
            }
            element1 = element1->next;
            element2 = element2->next;
        }

        return true;
    }

private:

    struct Node
    {
        Node* next; 
        Node* prev; 
        T data;
    };

    size_t l_size;
    Node* first_pntr; // указатель на первый элемент
    Node* last_pntr; // указатель на последний элемент

    void clear() {
        Node* element = first_pntr;
        
        while (element != nullptr) {
            Node* next_node = element->next;
            delete element;
            element = next_node;
        }
    }
};