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

        for (int i=0; i < c_size; i++) {
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

    // итератор
    struct iterator{
    private:
        T* s_pntr;
    public:
        // конструктор
        iterator(T* pntr) : s_pntr{pntr} {};

        // оператор разыменовывания
        T& operator*() {
            return *s_pntr;
        };

        iterator& operator++() {
            s_pntr += 1;
            return *this;
        }

        bool operator!=(const iterator& other) {
            if (s_pntr == other.s_pntr) {
                return false;
            }
            return true;
        }
    };

    iterator begin() {
        return iterator(c_pntr);
    }

    iterator end() {
        return iterator(c_pntr + c_size);
    }

    
private:
    T* c_pntr;
    int c_size;
};

void test_SequenceContainer() {
    std::cout << "###############################\n";
    std::cout << "ITERATOR TEST FOR SEQUENCE CONTAINER\n";

    SequenceContainer<int> test({1,2,3,4});

    for (auto iter=test.begin(); iter != test.end(); ++iter) {
        std::cout << *iter << std::endl;
    }
    
}

template <typename T>
class ListContainer {
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

public:
    ListContainer() : first_pntr{nullptr}, last_pntr{nullptr}, l_size{0} {};

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

        for (int i=0; i < list_container_array.size(); i++) {
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

    struct iterator{
    private: 
        Node* i_pntr;
    public:
        iterator(Node* n_pntr): i_pntr{n_pntr} {};

        T& operator*() {
            return i_pntr->data;
        }

        iterator& operator++() {
            i_pntr = i_pntr->next;

            return *this;
        }

        bool operator!=(const iterator& other) {
            if (i_pntr != other.i_pntr) {
                return true;
            }
            return false;
        }
    };

    iterator begin() {
        return iterator(first_pntr);
    }

    iterator end() {
        return iterator(nullptr);
    }

};

void test_ListContainer() {
    std::cout << "###############################\n";
    std::cout << "ITERATOR TEST FOR LIST CONTAINER\n";

    ListContainer<int> test({1,2,3,4});

    for (auto iter=test.begin(); iter != test.end(); ++iter) {
        std::cout << *iter << std::endl;
    }
    
}

// реализация контейнера односвязного списка
template <typename T>
class ForwardListContainer {
private:
    struct Node {
        T data;
        Node* next; 
    };

    Node* first_pntr;
    size_t l_size;

    void clear() {
        Node* element = first_pntr;

        while (element != nullptr) {
            Node* next_element = element->next;
            delete element;
            element = next_element;
        }

        first_pntr = nullptr;
        l_size = 0;
    }

public:
    ForwardListContainer () : first_pntr{nullptr}, l_size{0} {};

    void push_back (T value) {
        // создаем новый узел
        Node* new_node = new Node;
        
        new_node->next = nullptr;
        new_node->data = value;

        // идем до узла
        if (l_size == 0) {
            first_pntr = new_node;
        } else {
            Node* element = first_pntr;

            while (element->next != nullptr) {
                element = element->next;
            }
            element->next = new_node;
        }
        
        l_size += 1;
    }

    ~ForwardListContainer() {
        this->clear();
    }

    // конструктор через заданный список элементов
    ForwardListContainer(std::initializer_list<T> list_container_array) : ForwardListContainer() {
        // копируем элементы из массива в контейнер
        const T* list_container_array_begin = list_container_array.begin();

        for (size_t i=0; i < list_container_array.size(); i++) {
            this->push_back(*(list_container_array_begin + i));
        }
    } 

    // копирование
    ForwardListContainer(const ForwardListContainer& other) {
        l_size = 0;
        first_pntr = nullptr;

        Node* other_element = other.first_pntr;
        while (other_element != nullptr) {
            this->push_back(other_element->data);
            other_element = other_element->next;
        }
    }

    // копирующее присваивание 
    ForwardListContainer& operator=(const ForwardListContainer& other) {
        this->clear();

        l_size = 0;
        first_pntr = nullptr;

        Node* other_element = other.first_pntr;
        while (other_element != nullptr) {
            this->push_back(other_element->data);
            other_element = other_element->next;
        }

        return *this;
    }

    // перемещение
    ForwardListContainer(ForwardListContainer&& other) {
        l_size = other.l_size;
        first_pntr = other.first_pntr;

        other.first_pntr = nullptr;
        other.l_size = 0;
    }

    // копирующее перемещение 
    ForwardListContainer& operator=(ForwardListContainer&& other) {
        this->clear();

        l_size = other.l_size;
        first_pntr = other.first_pntr;

        other.first_pntr = nullptr;
        other.l_size = 0;

        return *this;
    }

    struct iterator{
    private:
        Node* i_pntr;
    public:
        iterator(Node* n_pntr) : i_pntr{n_pntr} {};

        T& operator*() {
            return i_pntr->data;
        }

        iterator& operator++() {
            i_pntr = i_pntr->next;

            return *this;
        }

        bool operator!=(const iterator& other) {
            if (i_pntr != other.i_pntr) {
                return true;
            }
            return false;
        }
    };

    iterator begin() {
        return iterator(first_pntr);
    }

    iterator end() {
        return iterator(nullptr);
    }

};

void test_ForwardListContainer() {
    std::cout << "###############################\n";
    std::cout << "ITERATOR TEST FOR FORWARD LIST CONTAINER\n";

    ForwardListContainer<int> test({1,2,3,4});

    for (auto iter=test.begin(); iter != test.end(); ++iter) {
        std::cout << *iter << std::endl;
    }
    
}

int main () {

    test_SequenceContainer();

    test_ListContainer();

    test_ForwardListContainer();

    return 0;
}