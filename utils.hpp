#pragma once
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <cstring>

const unsigned long INIT_CAPACITY = 8;

template<typename T>
class Vector{
private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity = INIT_CAPACITY;

private:
    void copyFrom(const Vector<T>& other){
        capacity = other.capacity;
        size = other.size;

        data = new T[capacity];
        for (size_t i = 0; i < size; i++){
            data[i] = other[i];
        }
        
    }

    void deallocate() {
        delete[] data;
        data = nullptr;
    }

    void updateCapacity(){
        size_t newCap = capacity * 2;
        T* newData = new T[newCap];
        for (size_t i = 0; i < size; i++){
            newData[i] = data[i];
        }
        deallocate();
        data = newData;
        capacity = newCap;
    }

public:
    Vector() : data(nullptr), size(0), capacity(INIT_CAPACITY) {}
    
    Vector(const std::initializer_list<T> initList) : size(initList.size()), capacity(initList.size() * 2) {
        data = new T[capacity];
        size_t i = 0;
        for (const T& el : initList){
            data[i] = el;
            ++i;
        }
    }

    Vector(const Vector<T>& other) {
        copyFrom(other);
    }

    Vector& operator=(const Vector<T>& other){
        if(this != &other){
            deallocate();
            copyFrom(other);
        }
        return *this;
    }

    ~Vector(){
        deallocate();
        size = 0;
        capacity = 0;
    }

    void pushBack(const T& el) {
        if(!data)
            data = new T[INIT_CAPACITY];
        
        else if(size >= capacity)
            updateCapacity();
        
        data[size] = el;
        ++size;
    }

    T& operator[](size_t index) {
        if(index >= size)
            throw std::out_of_range("Vector: Index out of bounds.");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if(index >= size)
            throw std::out_of_range("Vector: Index out of bounds.");
        return data[index];
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector<T> vec){
        os << '[';
        for (size_t i = 0; i < vec.size; i++){
           os << vec.data[i];
           if(i < vec.size - 1)
               os << ", ";
        }
        os << ']';
        return os;
    }
};



class String{
private:
    char* data = nullptr;
    size_t size = 0;

private:
    void copyFrom(const String& other){
        size = other.size;

        data = new char[size + 1]{'\0'};
        strncpy(this->data, other.data, size);
        data[size] = '\0';
    }

    void deallocate() {
        delete[] data;
        data = nullptr;
    }

public:
    String() : data(nullptr), size(0) {}

    String(const String& other){
        copyFrom(other);
    }

    String& operator=(const String& other){
        if(this != &other){
            deallocate();
            copyFrom(other);
        }
        return *this;
    }

    String(const char* str) {
        if (str) {
            size = strlen(str);
            data = new char[size + 1]{'\0'};
            strncpy(data, str, size);
            data[size] = '\0';
        }
    }

    String(String&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            deallocate();
            data = other.data;
            size = other.size;
            
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~String(){
        deallocate();
        size = 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const String& str){
        return os << str.data;
    }

    String& operator+=(const String& other){
        if(other.data){
            size_t newSize = size + other.size;
            char* newData = new char[newSize + 1]{'\0'};
            
            strncpy(newData, this->data, size);
            strncat(newData, other.data, other.size);
            newData[newSize] = '\0';
            
            deallocate();
            data = newData;
            size = newSize;
        }
        return *this;
    }

    String operator+=(const char* cstr){
        return operator+=(String(cstr));
    }

    friend String operator+(const String& lhs, const String& rhs){
        String result(lhs);
        return result += rhs;
    }

};
