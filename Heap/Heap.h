#pragma once

#include <iostream>
#include <sstream>
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

template <typename ValueType>
class Heap // min heap
{
public:
    Heap();
    ~Heap();

    Heap(const Heap<ValueType>& heap2);
    Heap& operator=(const Heap<ValueType>& heap2);

    ValueType& peek() const;
    ValueType extract();
    void insert(const ValueType& value);

    int size() const;
    bool isEmpty() const;
    void clear();

    bool equals(const Heap<ValueType>& heap2) const;
    bool operator==(const Heap& heap2) const;
    bool operator!=(const Heap& heap2) const;

    int contains(int i, const ValueType& value);
    bool contains(const ValueType& value);
    void remove(const ValueType& value);

    void peekUpdate();


private:

    ValueType* elements;
    int capacity;
    int numElements;

    static const int INITIAL_CAPACITY = 8;

    void expand();
    void bubbleUp(int i);
    void bubbleDown(int i);
};


template <typename ValueType>
const int Heap<ValueType>::INITIAL_CAPACITY;

template <typename ValueType>
Heap<ValueType>::Heap() {
    elements = new ValueType[INITIAL_CAPACITY];
    capacity = INITIAL_CAPACITY;
    numElements = 0;
}

template <typename ValueType>
Heap<ValueType>::~Heap() {
    delete[] elements;
}

template <typename ValueType>
Heap<ValueType>::Heap(const Heap<ValueType>& heap2)
{
    capacity = max(INITIAL_CAPACITY, heap2.numElements) * 2;
    elements = new ValueType[capacity];
    for (int i = 0; i < heap2.numElements; i++) {
        elements[i] = heap2.elements[i];
    }
    numElements = heap2.numElements;
}

template <typename ValueType>
Heap<ValueType>& Heap<ValueType>::operator=(const Heap<ValueType>& heap2)
{
    delete [] elements;
    capacity = max(INITIAL_CAPACITY, heap2.numElements) * 2;
    elements = new ValueType[capacity];
    for (int i = 0; i < heap2.numElements; i++) {
        elements[i] = heap2.elements[i];
    }
    numElements = heap2.numElements;
    return *this;
}

template <typename ValueType>
ValueType& Heap<ValueType>::peek() const {
    if (isEmpty()) {
        error("Queue::peek: Attempting to peek at an empty queue");
    }
    return elements[0];
}

template <typename ValueType>
void Heap<ValueType>::peekUpdate()
{
    bubbleDown(0);
}

template <typename ValueType>
ValueType Heap<ValueType>::extract() {
    if (isEmpty()) {
        error("Queue::dequeue: Attempting to dequeue an empty queue");
    }
    ValueType value = elements[0];
    elements[0] = elements[--numElements];
    bubbleDown(0);
    return value;
}

template <typename ValueType>
void Heap<ValueType>::insert(const ValueType& value) {
    if (numElements == capacity) {
        expand();
    }
    elements[numElements] = value;
    bubbleUp(numElements);
    ++numElements;
}

template <typename ValueType>
int Heap<ValueType>::size() const {
    return numElements;
}

template <typename ValueType>
bool Heap<ValueType>::isEmpty() const {
    return numElements == 0;
}

template <typename ValueType>
void Heap<ValueType>::clear() {
    numElements = 0;
}

template <typename ValueType>
void Heap<ValueType>::expand() {
    capacity = numElements * 2;
    ValueType* newElements = new ValueType[capacity];
    for (int i = 0; i < numElements; i++) {
        newElements[i] = elements[i];
    }
    delete[] elements;
    elements = newElements;
}

template <typename ValueType>
void Heap<ValueType>::bubbleDown(int i) {
    if (2*i+1 < numElements) {
        int j = (2*i+2 >= numElements || elements[2*i+1] < elements[2*i+2]) ? 2 * i + 1 : 2 * i + 2;
        if (elements[j] < elements[i]) {
            swap(elements[i], elements[j]);
            bubbleDown(j);
        }
        return;
    }
}

template <typename ValueType>
void Heap<ValueType>::bubbleUp(int i) {
    if (i > 0) {
        int j = (i - 1) / 2;
        if (elements[i] < elements[j]) {
            swap(elements[i], elements[j]);
            bubbleUp(j);
        }
        return;
    }
}

template <typename KeyType, typename ValueType>
struct KVPair
{
    KeyType key;
    ValueType value;
};

template <typename KeyType, typename ValueType>
bool operator<(const KVPair<KeyType,ValueType>& a, const KVPair<KeyType,ValueType>& b)
{
    return a.key < b.key;
}

template <typename ValueType>
int Heap<ValueType>::contains(int i, const ValueType& value)
{
    if(i >= numElements)
        return -1;
    if(value == elements[i])
        return i;
    else if(value < elements[i])
        return -1;
    else
    {
        int x = contains(2*i +1,value);
        return (x != -1)? x: contains(2*i +2,value);
    }

}

template <typename ValueType>
bool Heap<ValueType>::contains(const ValueType& value)
{
    return contains(0,value) != -1;
}

template <typename ValueType>
void Heap<ValueType>::remove(const ValueType& value)
{
    int i = contains(0,value);
    if(i != -1)
    {
        elements[i] = elements[--numElements];
        if(elements[i] < elements[(i-1)/2])
            bubbleUp(i);
        else
            bubbleDown(i);
    }
}

PROVIDED_TEST("constructor and destructor test")
{
    Heap<int> heap1;
    heap1.insert(1);

    Heap<double> heap2;
    heap2.insert(0.01);

    Heap<string> heap3;
    heap3.insert("Heap");
}

PROVIDED_TEST("empty and size test")
{
    Heap<int> heap;
    EXPECT_EQUAL(heap.isEmpty(), true);
    EXPECT_EQUAL(heap.size(), 0);
}

PROVIDED_TEST("Add 5 elements to heap.")
{
    Heap<int> heap;
    for (int i = 0; i < 5; i++) {
        heap.insert(i);
    }

    EXPECT_EQUAL(heap.size(), 5);
    EXPECT_EQUAL(heap.isEmpty(), false);
}

PROVIDED_TEST("Add 50 elements to heap. Tests expansion")
{
    Heap<double> heap;
    for (int i = 0; i < 50; i++) {
        heap.insert(i);
    }

    EXPECT_EQUAL(heap.size(), 50);
    for (int i = 0; i < 50; i++)
    {
        EXPECT_EQUAL(heap.extract(), i);
    }

    EXPECT_EQUAL(heap.isEmpty(), true);
}

PROVIDED_TEST("Extract order test")
{
    Heap<int> heap;
    heap.insert(1);
    heap.insert(3);
    heap.insert(2);
    heap.insert(5);
    heap.insert(4);

    EXPECT_EQUAL(heap.extract(), 1);
    EXPECT_EQUAL(heap.extract(), 2);
    EXPECT_EQUAL(heap.extract(), 3);
    EXPECT_EQUAL(heap.extract(), 4);
    EXPECT_EQUAL(heap.extract(), 5);
    EXPECT_EQUAL(heap.isEmpty(), true);
    EXPECT_ERROR(heap.extract());
}

PROVIDED_TEST("KVParir test")
{
    typedef KVPair<int, string> KVPair;
    Heap<KVPair> heap;
    heap.insert(KVPair{1, "a"});
    heap.insert(KVPair{3, "c"});
    heap.insert(KVPair{2, "b"});
    heap.insert(KVPair{5, "e"});
    heap.insert(KVPair{4, "d"});

    EXPECT_EQUAL(heap.extract().value, "a");
    EXPECT_EQUAL(heap.extract().value, "b");
    EXPECT_EQUAL(heap.extract().value, "c");
    EXPECT_EQUAL(heap.extract().value, "d");
    EXPECT_EQUAL(heap.extract().value, "e");
    EXPECT_EQUAL(heap.isEmpty(), true);
    EXPECT_ERROR(heap.extract());
}
