#pragma once 

#include <iostream>
#include <initializer_list>

// последовательнный контейнер 
template <typename T>
class SequenceContainer {
public:
    // инициализация пустого контейнера, конструктор по умолчанию
    SequenceContainer() : c_pntr{nullptr}, c_size{0} {};

    // конструктор с заданным количеством элементов
    SequenceContainer(std::initializer_list<T> sequence_container_list) {
        c_size = sequence_container_list.size();
        c_pntr = new T[c_size];

        // копируем элементы из массива в контейнер
        const T* sequence_container_list_begin = sequence_container_list.begin();

        for (size_t i=0; i < c_size; i++) {
            c_pntr[i] = *(sequence_container_list_begin + i);
        }
    }

    ~SequenceContainer() {
        delete[] c_pntr;
    }

    // копирование
    SequenceContainer(const SequenceContainer& other) {
        c_size = other.c_size;
        c_pntr = new T[c_size];

        for (size_t i=0; i < c_size; i++) {
            c_pntr[i] = other.c_pntr[i];
        }
    }

    // присваивание
    SequenceContainer& operator=(const SequenceContainer& other) {
        delete[] c_pntr;

        c_size = other.c_size;
        c_pntr = new T[c_size];

        for (size_t i=0; i < c_size; i++) {
            c_pntr[i] = other.c_pntr[i];
        }

        return *this;
    }

    // перемещение 
    SequenceContainer(SequenceContainer&& other) {
        c_size = other.c_size;
        c_pntr = other.c_pntr;

        other.c_size = 0;
        other.c_pntr = nullptr;
    }

    // перемещающее присваивание 
    SequenceContainer& operator=(SequenceContainer&& other) {
        delete[] c_pntr;

        c_size = other.c_size;
        c_pntr = other.c_pntr;

        other.c_pntr = nullptr;
        other.c_size = 0;

        return *this;
    }

    // геттер разрмера
    size_t size() const {
        return c_size;
    }

    void printSize() const {
        std::cout << c_size << std::endl;
    }

    // общая информация о контенере DEV
    void getInfoDev() const {
        std::cout << "Container is based in " << c_pntr << " with size " << c_size << std::endl;

        for (int i=0; i < c_size; i++) {
            std::cout << "Element = " << *(c_pntr+i) << " has adress" << c_pntr + i << std::endl;
        }

    };

    void print() const {
        for (size_t i=0; i < c_size; i++) {
            if (i == c_size - 1) {
                std::cout << c_pntr[i] << std::endl;
            } else {
                std::cout << c_pntr[i] << ", ";
            }
            
        }
    }

    void push_back(T value) {
        T* new_region = new T[c_size + 1];

        for (size_t i=0; i < c_size; i++) {
            new_region[i] = c_pntr[i];
        }

        new_region[c_size] = value; 
        delete[] c_pntr;

        // обновляем мемберов 
        c_pntr = new_region;
        c_size += 1;
    }

    void insert(T value, size_t position) {
        // проверка границ
        if (position > c_size) {return;}
        // выделяем область памяти
        T* new_region = new T[c_size + 1];

        // перебираем до нужного элемента
        for (size_t i=0; i < position; i++) {
            new_region[i] = c_pntr[i];
        }
        
        // вставляем нужный элемент 
        new_region[position] = value;

        // сдвигаем на единицу индекс и копируем дальше
         for (size_t i = position; i < c_size; i++) {
            new_region[i + 1] = c_pntr[i];
         }

        delete[] c_pntr;

        //обновляем мемберов
        c_pntr = new_region;
        c_size += 1;

    }

    
    void erase(size_t position) {
    
        if (position >= c_size) { return; }  

    
        if (c_size == 1) {
            delete[] c_pntr;
            c_pntr = nullptr;
            c_size = 0;
            return;
        }

        T* new_region = new T[c_size - 1];


        for (size_t i = 0; i < position; i++) {
            new_region[i] = c_pntr[i];
        }

    
        for (size_t i = position; i < c_size - 1; i++) {
            new_region[i] = c_pntr[i + 1];
        } 

        delete[] c_pntr;


        c_pntr = new_region;
        c_size -= 1;
    }

    T& operator[] (int i) {
        return c_pntr[i];
    }

    bool operator==(const SequenceContainer& other) const {
        if (c_size != other.c_size) return false;

        for (size_t i=0; i < c_size; i++) {
            if (c_pntr[i] != other.c_pntr[i]) {
                return false;
            }
        }
        return true;
    }

private:
    T* c_pntr;
    size_t c_size;
};