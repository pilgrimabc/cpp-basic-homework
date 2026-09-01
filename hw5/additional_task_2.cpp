#include <iostream>
#include <initializer_list>

// реализация контейнера односвязного списка
template <typename T>
class ForwardListContainer {
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

    // конструктор через заданный список элементов
    ForwardListContainer(std::initializer_list<T> list_container_array) : ForwardListContainer() {
        // копируем элементы из массива в контейнер
        const T* list_container_array_begin = list_container_array.begin();

        for (size_t i=0; i < list_container_array.size(); i++) {
            this->push_back(*(list_container_array_begin + i));
        }
    } 

    ~ForwardListContainer() {
        Node* element = first_pntr;

        while (element != nullptr) {
            Node* next_element = element->next;
            delete element;
            element = next_element;
        }

        first_pntr=nullptr;
        l_size=0;
    }

    void erase(size_t delete_index) {
        // если первый элемент
        if (delete_index == 0) {
            Node* delete_element = first_pntr;

            first_pntr = first_pntr->next;
            delete delete_element;

            l_size -= 1;
            return;
        }
        
        // идем до элемента, который стоит перед delete_element
        Node* prev_element = first_pntr;
        for (size_t i=0; i < delete_index - 1; i++) {
            prev_element = prev_element->next;
        }

        // находим узел для удаления и сшиваем узлы
        Node* delete_element = prev_element->next;

        prev_element->next = delete_element->next;
        delete delete_element;

        l_size -= 1;
    }

    void insert(T value, size_t insert_index) {
        // новый узел 
        Node* insert_element = new Node;
        insert_element->data = value;

        if (insert_index == 0) {
            insert_element->next = first_pntr;
            
            first_pntr = insert_element;
            l_size += 1;

            return;
        }

        // идем до узла перед insert_element и сшиваем указатели
        Node* prev_element = first_pntr;
        for (size_t i=0; i < insert_index - 1; i++) {
            prev_element = prev_element->next;
        }

        insert_element->next = prev_element->next;
        prev_element->next = insert_element;

        l_size += 1;

    }

    size_t size() const {
        return l_size;
    }

    void printSize() const {
        std::cout << this->size() << std::endl;
    }

    T& operator[](size_t index) {
        Node* element = first_pntr;

        for (size_t i=0; i < index; i++) {
            element = element->next;
        }

        return element->data;
    }

    void print() const {
        Node* element = first_pntr;

        while (element != nullptr) {
            std::cout << element->data << ", ";
            element = element->next;
        }
        std::cout<< std::endl;
    }

private:
    struct Node {
        T data;
        Node* next; 
    };

    Node* first_pntr;
    size_t l_size;

};

int main () {
    std::cout << "1. cоздание объекта контейнера для хранения объектов типа int\n";
    std::cout << "2. добавление в контейнер десяти элементов (0, 1 ... 9)\n";

    ForwardListContainer<int> task({0,1,2,3,4,5,6,7,8,9});

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