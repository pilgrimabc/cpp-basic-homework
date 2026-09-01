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

    // геттер разрмера
    int getSize() const {
        return c_size;
    }

    // общая информация о контенере
    void getInfo() const {
        std::cout << "Container is based in " << c_pntr << " with size " << c_size << std::endl;

        for (int i=0; i < c_size; i++) {
            std::cout << "Element = " << *(c_pntr+i) << " has adress" << c_pntr + i << std::endl;
        }

    };

    void pushback(T value) {
        T* new_region = new T[c_size + 1];

        for (int i=0; i < c_size; i++) {
            new_region[i] = c_pntr[i];
        }

        new_region[c_size] = value; 
        delete[] c_pntr;

        // обновляем мемберов 
        c_pntr = new_region;
        c_size += 1;
    }

    void insert(T value, int position) {
        // проверка границ
        if (position < 0 || position > c_size) {return;}
        // выделяем область памяти
        T* new_region = new T[c_size + 1];

        // перебираем до нужного элемента
        for (int i=0; i < position; i++) {
            new_region[i] = c_pntr[i];
        }
        
        // вставляем нужный элемент 
        new_region[position] = value;

        // сдвигаем на единицу индекс и копируем дальше
         for (int i = position; i < c_size; i++) {
            new_region[i + 1] = c_pntr[i];
         }

        delete[] c_pntr;

        //обновляем мемберов
        c_pntr = new_region;
        c_size += 1;

    }

    void erase(T value, int position) {
        // проверка границ
        if (position < 0 || position > c_size) {return;}  

        // если контейнер состоит из одного элемента, то надо вернуть пустой контейнер
        if (c_size == 1) {
            delete[] c_pntr;
            c_pntr = nullptr;
            c_size = 0;
            return;
        }

        // выделяем область памяти 
        T* new_region = new T[c_size - 1];

        //перебираем до нужного элемнета
        for (int i=0; i < position; i++) {
            new_region[i] = c_pntr[i];
        }

        // "удаление" элемента и копирование всего сразу после него
        for (int i = position; i < c_size - 1; i++) {
            new_region[i] = c_pntr[i + 1];
        } 

        delete[] c_pntr;

        //обновляем мемберов
        c_pntr = new_region;
        c_size -= 1;
        
    }

    T& operator[] (int i) {
        return c_pntr[i];
    }

private:
    T* c_pntr;
    int c_size;
};

template <typename T>
class ListContainer {
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

    void getInfo() const {
        // начнинаем с первого элемента
        Node* element = first_pntr;

        // идем по всем узлам
        do {
            std::cout << "Element = " << element->data << " has adress " << element << std::endl;
            element = element->next;
        }
        while (element != nullptr);
    }

    void erase(int delete_index) {
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

    void insert(T value, int index) {
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

    T& operator[] (int index) {
        Node* element = first_pntr;

        for (size_t i=0; i < index; i++) {
            element = element->next;
        }

        return element->data;
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

};


int main() {

    //SequenceContainer<int> test;

    //test.getInfo();

    //SequenceContainer<int> test2({1,2,3,4});

    //test2.getInfo();

    //test2.pushback(5);

    //test2.getInfo();

    //test2.insert(10,5);

    //test2.getInfo();

    //test2.erase(10,6);

    //test2.getInfo();

    //std::cout << "operator[] overload" << std::endl;
    //std::cout << "test2[3] = " << test2[3] << std::endl;

    //test2.insert(6,2);

    //test2.getInfo();

    //ListContainer<int> test({1,2,3,4});

    //test.getInfo();

    //std::cout << "\n#################################\n" << std::endl;

    //test.insert(10,0);

    //test.getInfo();


    return 0;
}
