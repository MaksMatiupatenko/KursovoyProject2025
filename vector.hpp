#pragma once

#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>

template<typename T, class Allocator = std::allocator<T>>
class vector {
    template<typename Y>
    class base_iterator {
        friend class vector<T>;
        template<typename X>
        friend class base_iterator;

        using value_type = Y;
        using difference_type = std::ptrdiff_t;
        using reference = Y&;
        using pointer = Y*;
        using iterator_category = std::contiguous_iterator_tag;

        Y* ptr_;

    public:
        base_iterator() = default;
        base_iterator(const base_iterator&) = default;
        base_iterator(base_iterator&&) = default;
        base_iterator& operator=(const base_iterator&) = default;
        base_iterator& operator=(base_iterator&&) = default;
        template<typename X>
        base_iterator(const base_iterator<X>& other) : ptr_(other.ptr_) {}
        template<typename X>
        base_iterator(base_iterator<X>&& other) : ptr_(other.ptr_) {}
        template<typename X>
        base_iterator& operator=(const base_iterator<X>& other) {
            ptr_ = other.ptr_;
            return *this;
        }
        template<typename X>
        base_iterator& operator=(base_iterator<X>&& other) {
            ptr_ = other.ptr_;
            return *this;
        }

        Y& operator*() const {
            return *ptr_;
        }
        Y* operator->() const {
            return ptr_;
        }
        Y& operator[](difference_type n) const {
            return ptr_[n];
        }

        base_iterator& operator++() {
            ++ptr_;
            return *this;
        }
        base_iterator operator++(int) {
            base_iterator it = *this;
            ++ptr_;
            return it;
        }
        base_iterator& operator--() {
            --ptr_;
            return *this;
        }
        base_iterator operator--(int) {
            base_iterator it = *this;
            --ptr_;
            return it;
        }

        base_iterator& operator+=(difference_type n) {
            ptr_ += n;
            return *this;
        }
        base_iterator operator+(difference_type n) const {
            base_iterator it = *this;
            return it += n;
        }
        friend base_iterator operator+(difference_type n, const base_iterator& it) {
            base_iterator it1 = it;
            return it1 += n;
        }
        base_iterator& operator-=(difference_type n) {
            ptr_ -= n;
            return *this;
        }
        base_iterator operator-(difference_type n) const {
            base_iterator it = *this;
            return it -= n;
        }
        base_iterator operator-(const base_iterator& other) const {
            return ptr_ - other.ptr_;
        }

        bool operator==(const base_iterator& other) const {
            return ptr_ == other.ptr_;
        }
        bool operator!=(const base_iterator& other) const {
            return ptr_ != other.ptr_;
        }
        bool operator<(const base_iterator& other) const {
            return ptr_ < other.ptr_;
        }
        bool operator<=(const base_iterator& other) const {
            return ptr_ <= other.ptr_;
        }
        bool operator>(const base_iterator& other) const {
            return ptr_ > other.ptr_;
        }
        bool operator>=(const base_iterator& other) const {
            return ptr_ >= other.ptr_;
        }
        int operator<=>(const base_iterator& other) const {
            return ptr_ <=> other.ptr_;
        }
    };

public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = base_iterator<value_type>;
    using const_iterator = base_iterator<const value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() : vector(Allocator()) {}
    explicit vector(const Allocator& alloc) : vector((size_t)0, alloc) {}
    vector(size_t count, const Allocator& alloc = Allocator()) : vector(count, T(), Allocator()) {}
    vector(size_t count, const T& value, const Allocator& alloc = Allocator()) : alloc_(alloc), size_(count), capacity_(count), data_(nullptr) {
        if (count != 0) {
            data_ = alloc_.allocate(count);
            for (size_t i = 0; i < count; ++i) {
                data_[i] = value;
            }
        }
    }
    template<class InputIt>
    vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : alloc_(alloc) {
        size_t count = 0;
        InputIt it = first;
        while (it != last) {
            ++count;
            ++it;
        }

        size_ = count;
        capacity_ = count;
        if (count != 0) {
            data_ = alloc_.allocate(count);
            for (size_t i = 0; i < count; ++i, ++first) {
                data_[i] = *first;
            }
        }
    }
    vector(const vector& other) : vector(other, other.alloc_) {}
    vector(vector&& other) noexcept {
        alloc_ = std::move(other.alloc_);
        data_ = other.data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    vector(const vector& other, const Allocator& alloc) : vector(other.begin(), other.end(), alloc) {}
    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : vector(init.begin(), init.end(), alloc) {}

    ~vector() {
        if (capacity_ > 0) {
            alloc_.deallocate(data_, capacity_);
        }
    }

    vector& operator=(const vector& other) {
        if (capacity_ > 0) {
            alloc_.deallocate(data_, capacity_);
        }
        alloc_ = other.alloc_;

        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = nullptr;
        if (capacity_ > 0) {
            data_ = alloc_.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }

        return *this;
    }
    vector& operator=(vector&& other) noexcept {
        swap(other);
        return *this;
    }
    vector& operator=(std::initializer_list<T>& init) {
        if (capacity_ > 0) {
            alloc_.deallocate(data_, capacity_);
            data_ = nullptr;
        }

        auto first = init.begin();
        auto last = init.end();
        size_t count = 0;
        auto it = first;
        while (it != last) {
            ++count;
            ++it;
        }

        size_ = count;
        capacity_ = count;
        if (count != 0) {
            data_ = alloc_.allocate(count);
            for (size_t i = 0; i < count; ++i, ++first) {
                data_[i] = *first;
            }
        }

        return *this;
    }

    void assign(size_t count, const T& value) {
        *this = vector(count, value, alloc_);
    }
    template<class InputIt>
    void assign(InputIt first, InputIt last) {
        *this = vector(first, last, alloc_);
    }
    void assign(std::initializer_list<T>& init) {
        *this = vector(init, alloc_);
    }

    Allocator get_allocator() const noexcept {
        return alloc_;
    }

    T& at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range({"vector access out of range"});
        }
        return data_[pos];
    }
    const T& at(size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range({"vector access out of range"});
        }
        return data_[pos];
    }

    T& operator[](size_t pos) {
        return data_[pos];
    }
    const T& operator[](size_t pos) const {
        return data_[pos];
    }

    T& front() {
        return data_[0];
    }
    const T& front() const {
        return data_[0];
    }

    T& back() {
        return data_[size_ - 1];
    }
    const T& back() const {
        return data_[size_ - 1];
    }

    T* data() noexcept {
        return data_;
    }
    const T* data() const noexcept {
        return data_;
    }

    iterator begin() noexcept {
        iterator it;
        it.ptr_ = data_;
        return it;
    }
    const_iterator begin() const noexcept {
        const_iterator it;
        it.ptr_ = data_;
        return it;
    }
    const_iterator cbegin() const noexcept {
        return begin();
    }

    iterator end() noexcept {
        iterator it;
        it.ptr_ = data_ + size_;
        return it;
    }
    const_iterator end() const noexcept {
        const_iterator it;
        it.ptr_ = data_ + size_;
        return it;
    }
    const_iterator cend() const noexcept {
        return begin();
    }

    reverse_iterator rbegin() noexcept {
        iterator it;
        it.ptr_ = data_ + size_ - 1;
        return it;
    }
    const_reverse_iterator rbegin() const noexcept {
        const_iterator it;
        it.ptr_ = data_ + size_ - 1;
        return it;
    }
    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    reverse_iterator rend() noexcept {
        iterator it;
        it.ptr_ = data_ - 1;
        return it;
    }
    const_reverse_iterator rend() const noexcept {
        const_iterator it;
        it.ptr_ = data_ - 1;
        return it;
    }
    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    bool empty() const {
        return size_ == 0;
    }
    size_t size() const {
        return size_;
    }
    size_t max_size() const noexcept {
        return std::numeric_limits<difference_type>::max() / sizeof(T);
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = alloc_.allocate(new_capacity);
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = std::move(data_[i]);
            }
            if (capacity_ > 0) {
                alloc_.deallocate(data_, capacity_);
            }
            capacity_ = new_capacity;
            data_ = new_data;
        }
    }

    size_t capacity() const noexcept {
        return capacity_;
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            if (size_ == 0) {
                alloc_.deallocate(data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            } else {
                T* new_data = alloc_.allocate(size_);
                for (size_t i = 0; i < size_; ++i) {
                    new_data[i] = std::move(data_[i]);
                }
                alloc_.deallocate(data_, capacity_);
                capacity_ = size_;
                data_ = new_data;
            }
        }
    }

    void clear() noexcept {
        if (capacity_ > 0) {
            alloc_.deallocate(data_, capacity_);
        }
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    iterator insert(const_iterator pos, const T& value) {
        if (size_ + 1 > capacity_) {
            reserve(capacity_ * 2 + 1);
        }
        size_t ind = pos - cbegin();

        for (size_t i = size_; i > ind; --i) {
            data_[i] = data[i - 1];
        }
        data_[ind] = value;
        ++size_;
        return begin() + ind;
    }
    iterator insert(const_iterator pos, T&& value) {
        if (size_ + 1 > capacity_) {
            reserve(capacity_ * 2 + 1);
        }
        size_t ind = pos - cbegin();

        for (size_t i = size_; i > ind; --i) {
            data_[i] = std::move(data[i - 1]);
        }
        data_[ind] = std::move(value);
        ++size_;
        return begin() + ind;
    }
    iterator insert(const_iterator pos, size_t count, const T& value) {
        if (size_ + count > capacity_) {
            reserve(capacity_ * 2 + count);
        }
        size_t ind = pos - cbegin();

        for (size_t i = size_+ count - 1; i >= ind + count; --i) {
            data_[i] = data[i - count];
        }
        for (size_t i = ind; i < ind + count; ++i) {
            data_[i] = value;
        }
        size_ += count;
        return begin() + ind;
    }
    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_t count = std::distance(first, last);
        if (size_ + count > capacity_) {
            reserve(capacity_ * 2 + count);
        }
        size_t ind = pos - cbegin();

        for (size_t i = size_+ count - 1; i >= ind + count; --i) {
            data_[i] = data[i - count];
        }
        for (size_t i = ind; i < ind + count; ++i, ++first) {
            data_[i] = *first;
        }
        size_ += count;
        return begin() + ind;
    }
    iterator insert(const_iterator pos, std::initializer_list<T> init) {
        return insert(pos, init.begin(), init.end());
    }

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        return insert(pos, T(std::forward<Args>(args)...));
    }

    iterator erase(const_iterator pos) {
        size_t ind = pos - cbegin();
        for (size_t i = ind + 1; i < size_; ++i) {
            data_[i - 1] = std::move(data_[i]);
        }
        --size_;
        if (size_ * 4 < capacity_) {
            shrink_to_fit();
        }

        return begin() + ind;
    }
    iterator erase(const_iterator first, const_iterator last) {
        size_t ind = first - cbegin();
        size_t count = last - first;
        for (size_t i = ind + count; i < size_; ++i) {
            data_[i - count] = std::move(data_[i]);
        }
        size_ -= count;
        if (size_ * 4 < capacity_) {
            shrink_to_fit();
        }

        return begin() + ind;
    }

    void push_back(const T& value) {
        if (size_ + 1 > capacity_) {
            reserve(capacity_ * 2 + 1);
        }
        data_[size_] = value;
        ++size_;
    }
    void push_back(T&& value) {
        if (size_ + 1 > capacity_) {
            reserve(capacity_ * 2 + 1);
        }
        data_[size_] = std::move(value);
        ++size_;
    }

    template <class... Args>
    T& emplace_back(Args&&... args) {
        if (size_ + 1 > capacity_) {
            reserve(capacity_ * 2 + 1);
        }
        data_[size_] = T(std::forward<Args>(args)...);
        ++size_;
        return data_[size_ - 1];
    }

    void pop_back() {
        --size_;
        if (size_ * 4 < capacity_) {
            shrink_to_fit();
        }
    }

    void resize(size_t count) {
        if (count < size_) {
            size_ = count;
            if (size_ * 4 < capacity_) {
                shrink_to_fit();
            }
        } else if (count > size_) {
            reserve(count);
            for (size_t i = size_; i < count; ++i) {
                data_[i] = T();
            }
            size_ = count;
        }
    }
    void resize(size_t count, const T& value) {
        if (count < size_) {
            size_ = count;
            if (size_ * 4 < capacity_) {
                shrink_to_fit();
            }
        } else if (count > size_) {
            reserve(count);
            for (size_t i = size_; i < count; ++i) {
                data_[i] = value;
            }
            size_ = count;
        }
    }

    void swap(vector& other) noexcept {
        std::swap(alloc_, other.alloc_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }


private:
    Allocator alloc_;
    size_t size_;
    size_t capacity_;
    T* data_;
};

template<typename T, class Alloc>
int operator<=>(const vector<T, Alloc>& l, const vector <T, Alloc>& r) {
    size_t i = 0;
    while (i < std::min(l.size(), r.size()) && l[i] == r[i]) {
        ++i;
    }
    if (i == std::min(l.size(), r.size())) {
        return l.size() <=> r.size();
    }
    return l[i] <=> r[i];
}
template<typename T, class Alloc>
bool operator==(const vector<T, Alloc>& l, const vector <T, Alloc>& r) {
    return (l <=> r) == 0;
}
