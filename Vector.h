#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

template<typename T>
class Vector{
private:
    T *m_container;
    size_t m_size;
    size_t m_capacity;

public:
    class Iterator;
    class Const_iterator;

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = Iterator;
    using const_iterator = Const_iterator;

    Vector(size_type size = 0) : m_size(size), m_capacity(size) {
        assert(size >= 0);
        m_container = new value_type[size];
        //std::cout << "DEFAULT_CONSTRUCTOR" << '\n';
    }

    Vector(const std::initializer_list<T> &list) : Vector(list.size()) {
        size_type count(0);
        for (auto &element : list)
            m_container[count++] = element;
        //std::cout << "INITIALIZER_LIST_CONSTRUCTOR" << '\n';
    }

    Vector(const Vector &vect) : m_size(vect.m_size), m_capacity(vect.m_size), m_container(new value_type[vect.m_size]) {
        for (size_type i = 0; i < vect.m_size; i++) {
           m_container[i] = vect.m_container[i];
        }
        //std::cout << "COPY_CONSTRUCTOR" << '\n';
    }
    //Destructor
    ~Vector() { delete[] m_container; }

    //simple methods
    size_type  size()  const { return m_size; }
    bool empty() const { return m_size == 0; }
    void clear() { m_size = 0; };
    size_type capacity() { return m_capacity; }

    void reserve(size_t new_len){
        if(new_len < m_capacity) throw std::runtime_error("Falsch(reserve)");

        T *new_array = new T [new_len];
        for (size_t i{0}; i < m_size; i++)
            new_array[i] = m_container[i];

        delete[] m_container;
        m_container = new_array;
        this->m_capacity = new_len;
    }

    void shrink_to_fit(){
        if (m_capacity > m_size)
            m_capacity = m_size;
    }

    void push_back(const value_type &value) {
        if (m_size < m_capacity) {
            ++m_size;
            m_container[m_size - 1] = value;
        }
        else if (m_size == m_capacity) {
            reserve(m_capacity + 1);
            ++m_size;
            m_container[m_size - 1] = value;
        }
    }

    void pop_back() { if (m_size > 0) --m_size; }


  reference operator[](const size_type size) {
      assert(size >= 0 && size <= m_size);
      return m_container[size];
  }

  const_reference operator[](const size_type size) const {
      assert(size >= 0 && size <= m_size);
      return m_container[size];
  }

    Vector& operator= (const Vector &vect) {
        delete[] m_container;
        m_size = vect.m_size;
        m_capacity = vect.m_capacity;
        m_container = new value_type[m_size];

        for (size_type i = 0; i < m_size; i++)
            m_container[i] = vect.m_container[i];

        return *this;
    }

    class Iterator : public std::iterator<std::forward_iterator_tag, value_type> {
        pointer i_ptr;
        Vector<value_type> *vect;

    public:
        Iterator(pointer to_container = nullptr, Vector<value_type> *v = nullptr)
            : i_ptr(to_container), vect(v)
        {
              //std::cout << "ITERATOR CONSTRUCTOR" << '\n';
        }

        operator Const_iterator() {
            return Const_iterator(i_ptr, vect);
        }

        reference operator* () {
            if (i_ptr < vect->begin().i_ptr || i_ptr >= vect->end().i_ptr)
                throw std::runtime_error("ITERATOR OUT OF BOUNDS (*)");

            return *i_ptr;
        }

        const_pointer operator-> () { return i_ptr; }

        Iterator operator+ (int value) {
            if (i_ptr < vect->begin().i_ptr || i_ptr >= vect->end().i_ptr)
                throw std::runtime_error("ITERATOR OUT OF BOUNDS (+)");
            i_ptr = i_ptr + value;

          return *this;
        }

        Iterator operator- (int value) {
            if (i_ptr < vect->begin().i_ptr || i_ptr >= vect->end().i_ptr)
                throw std::runtime_error("ITERATOR OUT OF BOUNDS (-)");
            i_ptr = i_ptr - value;

            return *this;
        }

        Iterator operator++() {
            if (i_ptr >= vect->end().i_ptr)
                throw std::runtime_error("Cannot increment any further (++V)");
            ++i_ptr;

            return *this;
        }


        Iterator operator++(int) {
            if (i_ptr >= vect->end().i_ptr)
                throw std::runtime_error("Cannot increment any further (V++)");
            Iterator tmp(*this);
            operator++();

            return tmp;
        }

        Iterator operator--() {
            if (i_ptr <= vect->begin().i_ptr)
                throw std::runtime_error("Cannot decrement any further (--V)");
                --i_ptr;

            return *this;
        }

        Iterator operator--(int) {
            if (i_ptr < vect->begin().i_ptr)
                throw std::runtime_error("Cannot decrement any further (V--)");
            Iterator tmp(*this);
            operator--();

            return tmp;
        }

        Iterator operator+=(int a) {
            if (i_ptr >= (vect->end().i_ptr))
                throw std::runtime_error("ITERATOR OUT OF BOUNDS (+=)");
            i_ptr += a;

            return *this;
        }

        Iterator operator-=(int a) {
            if (i_ptr < vect->begin().i_ptr)
                throw std::runtime_error("ITERATOR OUT OF BOUNDS (+=)");
            i_ptr -= a;

            return *this;
        }

        friend bool operator==(const Iterator& lop, const Iterator& rop) {
            return lop.i_ptr == rop.i_ptr;
        }
        friend bool operator!=(const Iterator& lop, const Iterator& rop) {
            return lop.i_ptr != rop.i_ptr;
        }
        friend Iterator operator-(int x, Iterator& obj) { return obj - x; }

        bool operator<(const Iterator &rop) { return i_ptr < rop.i_ptr; }
        bool operator>(const Iterator &rop) { return i_ptr > rop.i_ptr; }
        bool operator>=(const Iterator &rop) { return i_ptr >= rop.i_ptr; }
        bool operator<=(const Iterator &rop) { return i_ptr <= rop.i_ptr; }

    };

    class Const_iterator : public std::iterator<std::forward_iterator_tag, value_type> {
        pointer i_ptr;
        Vector<value_type> *vect;

    public:
        Const_iterator(pointer to_container = nullptr, Vector<value_type> *v = nullptr)
            : i_ptr(to_container), vect(v)
        {
              //std::cout << "CONST_ITERATOR CONSTRUCTOR" << '\n';
        }

        reference operator* () { return *i_ptr; }
        const_pointer operator-> () { return i_ptr; }

        const_iterator operator+ (int value) {
          i_ptr = i_ptr + value;
          return *this;
        }

        const_iterator operator- (int value) {
            i_ptr = i_ptr - value;
            return *this;
        }

        const const_iterator operator++() {
            ++i_ptr;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp(*this);
            operator++();
            return tmp;
        }

        const_iterator operator--() {
            --i_ptr;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp(*this);
            operator--();
            return tmp;
        }

        const_iterator operator+=(int a) {
            i_ptr += a;
            return *this;
        }

        const_iterator operator-=(int a) {
            i_ptr -= a;
            return *this;
        }

        friend bool operator==(const const_iterator& lop, const const_iterator& rop) {
            return lop.i_ptr == rop.i_ptr;
        }
        friend bool operator!=(const const_iterator& lop, const const_iterator& rop) {
            return lop.i_ptr != rop.i_ptr;
        }

        friend const_iterator operator-(int x, const_iterator& obj) { return obj - x; }

        bool operator<(const const_iterator &rop) { return i_ptr < rop.i_ptr; }
        bool operator>(const const_iterator &rop) { return i_ptr > rop.i_ptr; }
        bool operator>=(const const_iterator &rop) { return i_ptr >= rop.i_ptr; }
        bool operator<=(const const_iterator &rop) { return i_ptr <= rop.i_ptr; }

        friend difference_type operator-(const Const_iterator& lop, const Const_iterator& rop) {
            return lop.i_ptr - rop.i_ptr;
        }
    };

    Iterator begin() { return Iterator(m_container, this); }
    Iterator end()   { return Iterator(m_container + m_size, this); }

    Const_iterator begin() const { return Const_iterator(m_container, this); }
    Const_iterator end()   const { return Const_iterator(m_container + m_size, this); }

    Iterator erase(Const_iterator pos) {
        auto diff = pos - begin();

        if (diff < 0 || static_cast<size_type>(diff) >= m_size)
            throw std::runtime_error("ITERATOR OUT OF BOUNDS (erase)");

        size_type current { static_cast<size_type>(diff) };

        for (size_type i { current }; i < m_size - 1; ++i)
            m_container[i] = m_container[i + 1]; --m_size;

        return Iterator { m_container + current, this };
    }

    Iterator insert(Const_iterator pos, const_reference val) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > m_size)
            throw std::runtime_error("Iterator out of bounds (insert)");

        size_type current { static_cast<size_type>(diff) };
        if (m_size >= m_capacity)
            reserve(m_capacity * 2); //max_sz*2+10, wenn Ihr Container max_sz==0 erlaubt

        for (size_type i { m_size }; i-- > current;)
            m_container[i + 1] = m_container[i];

        m_container[current] = val;
        ++m_size;

        return Iterator { m_container + current, this };
    }

    friend std::ostream& operator<< (std::ostream &out, Vector &obj) {
        for (size_type i = 0; i < obj.m_size; i++)
            out << obj.m_container[i] << ' ';
            out << '\n';
        return out;
    }
};
#endif
