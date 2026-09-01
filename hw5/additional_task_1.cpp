#include <iostream>
#include <initializer_list>

// последовательнный контейнер c выделением дополнительной памяти
template <typename T>
class SequenceContainer {
public:
    // инициализация пустого контейнера, конструктор по умолчанию
    SequenceContainer() : c_pntr{nullptr}, c_size{0}, c_capacity{0} {};

    // конструктор с заданным количеством элементов
    SequenceContainer(std::initializer_list<T> sequence_container_list) {
        c_size = sequence_container_list.size();
        c_capacity = sequence_container_list.size();
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
    size_t getSize() const {
        return c_size;
    }

    void printSize() const {
        std::cout << c_size << std::endl;
    }

    // общая информация о контенере DEV
    void getInfoDev() const {
        std::cout << "General information for dev about container\n" << std::endl;
        std::cout << "Size = " << c_size 
                  << ", Capacity = " << c_capacity
                  << ", Memory adress = " << c_pntr << std::endl;
        
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
        // проверка вместимости котенейра
        if (c_size == c_capacity) {
            // новая вместимость 
            size_t new_capacity = c_capacity * 2;

            // выделяем память
            T*  new_region = new T[new_capacity];

            // копируем старые элементы 
            for (size_t i=0; i < c_size; i++) {
                new_region[i] = c_pntr[i];
            }

            delete[] c_pntr;
            c_pntr = new_region;
            c_capacity = new_capacity;
        }

        c_pntr[c_size] = value;
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

    
    void erase(int position) {
    
        if (position < 0 || position >= c_size) { return; }  

    
        if (c_size == 1) {
            delete[] c_pntr;
            c_pntr = nullptr;
            c_size = 0;
            return;
        }

        T* new_region = new T[c_size - 1];


        for (int i = 0; i < position; i++) {
            new_region[i] = c_pntr[i];
        }

    
        for (int i = position; i < c_size - 1; i++) {
            new_region[i] = c_pntr[i + 1];
        } 

        delete[] c_pntr;


        c_pntr = new_region;
        c_size -= 1;
    }

    T& operator[] (int i) {
        return c_pntr[i];
    }

private:
    T* c_pntr;
    int c_size;
    size_t c_capacity;
};

int main () {
    // test 
    SequenceContainer<int> test({1,2,3,4});

    test.getInfoDev(); 

    test.push_back(5);

    std::cout << "After pushback operation\n" << std::endl;

    test.getInfoDev();

    test.push_back(6);

    std::cout << "After pushback operation\n" << std::endl;

    test.getInfoDev();

    return 0;
} 