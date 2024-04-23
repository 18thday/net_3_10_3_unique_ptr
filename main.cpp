#include <cassert>
#include <iostream>
#include <utility>

template <typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* raw_ptr) : raw_ptr_(raw_ptr) {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) {
        this->operator=(std::move(other));
    }
    UniquePtr& operator=(UniquePtr&& other) {
        if (raw_ptr_ != other.raw_ptr_) {
            delete this->release();
            raw_ptr_ = other.raw_ptr_;
            other.raw_ptr_ = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        delete raw_ptr_;
    }

    T& operator*() {
        return *raw_ptr_;
    }
    const T& operator*() const {
        return *raw_ptr_;
    }

    T* operator->() {
        return raw_ptr_;
    }
    const T* operator->() const {
        return raw_ptr_;
    }

    T* release() {
        T* raw_ptr = raw_ptr_;
        raw_ptr_ = nullptr;
        return raw_ptr;
    }

    T* get() {
        return raw_ptr_;
    }

private:
    T* raw_ptr_ = nullptr;
};

struct AB {
    int A;
    int B;
    int sum() {
        return A+B;
    }
};

int main() {
    UniquePtr<int> ptr_int(new int(7));
    std::cout << *ptr_int << std::endl;

    auto raw_ptr = ptr_int.release();
    std::cout << *raw_ptr << std::endl;
    assert(ptr_int.get() == nullptr);

    UniquePtr<AB> ptr_AB(new AB({2, 4}));
    std::cout << "A = " << ptr_AB->A
              << ", B = " << ptr_AB->B
              << ", sum = " << ptr_AB->sum() <<std::endl;

    // copy constructor = delete
//    UniquePtr<AB> ptr_AB_copy(ptr_AB); // is not compiled

    // copy assignment operator = delete
//    UniquePtr<AB> ptr_AB_2(new AB({4, 2}));
//    ptr_AB = ptr_AB_2; // is not compiled

    return 0;
}
