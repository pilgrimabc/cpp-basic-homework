#include <iostream>
#include <initializer_list>

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

int main () {

    std::cout << "1. cоздание объекта контейнера для хранения объектов типа int\n";
    std::cout << "2. добавление в контейнер десяти элементов (0, 1 ... 9)\n";

    ListContainer<int> task({0,1,2,3,4,5,6,7,8,9});

    std::cout << "3. вывод содержимого контейнера на экран\n" << std::endl;

    task.print();
    
    std::cout << "4. вывод размера контейнера на экран\n" << std::endl;

    task.printSize();

    std::cout << "5. удаление третьего (по счёту), пятого и седьмого элементов" << std::endl;

    task.erase(2);
    task.erase(3);
    task.erase(4);

    std::cout << "6. вывод содержимого контейнера на экран\n" << std::endl;

    task.print();

    std::cout << "7. добавление элемента 10 в начало контейнера" << std::endl;

    task.insert(10,0);

    std::cout << "8. вывод содержимого контейнера на экран\n" << std::endl;

    task.print();

    std::cout << "9. добавление элемента 20 в середину контейнера" << std::endl;

    task.insert(20,4);

    std::cout << "10. вывод содержимого контейнера на экран\n" << std::endl;

    task.print();

    std::cout << "11. добавление элемента 30 в конец контейнера" << std::endl;

    task.push_back(30);

    std::cout << "12. вывод содержимого контейнера на экран\n" << std::endl;

    task.print();

    return 0;
}