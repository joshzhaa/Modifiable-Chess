#include <cstddef>

template<typename T>
class circular_buffer {
    private:
        size_t size;
        T* data;
        T* write;
    public:
        circular_buffer(size_t size_in) : size(size_in), data(new T[size]), write(data) {
            for (T* ptr = data; ptr < size; ++ptr) {
                *ptr = {};
            }
        }
        void push(const T& element) {
            *write++ = element;
            write %= size; 
        }
        T& operator[] (int i) {
            return *((write - i) % size);
        }
        ~circular_buffer() {
            delete[] data;
        }
};
