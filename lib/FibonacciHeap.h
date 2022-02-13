//
// Created by Pasindu Wijesena on 2022-02-09.
//

#ifndef ASSIGNMENT2_FIBONACCIHEAP_H
#define ASSIGNMENT2_FIBONACCIHEAP_H

#include <list>
#include <unordered_map>

#define UNDEFINED -1000000000

class FibonacciHeapNode {
public:
    int degree;
    FibonacciHeapNode *p; // parent node
    FibonacciHeapNode *child;
    FibonacciHeapNode *left;
    FibonacciHeapNode *right;
    bool mark;
    int key; // value of the node

    explicit FibonacciHeapNode(int);

    ~FibonacciHeapNode();
};

class FibonacciHeap {
private:
    FibonacciHeapNode *roots;
    FibonacciHeapNode *minNode;
    int n; // number of nodes currently in the Heap

    void consolidate();

    void link(FibonacciHeapNode *, FibonacciHeapNode *);

    void insertToRootList(FibonacciHeapNode *);

    void removeFromRootList(FibonacciHeapNode *);

    void createNewRootList(FibonacciHeapNode *);

public:
    FibonacciHeap();

    ~FibonacciHeap();

    void insert(int);

    void popMinimum();

    int minimum();
};

template<typename T>
class AugmentedFibonacciHeap {
private:
    FibonacciHeap heap;
    std::unordered_map<int, std::list<T>> records;
    int N = 0; // number of records
public:
    AugmentedFibonacciHeap() {
        // nothing to be done
    }

    void push(int key, T data) {
        if (records[key].empty()) heap.insert(key);
        records[key].push_back(data);
        N += 1;
    }

    void pop() {
        if (N == 0) throw std::underflow_error("There are no records in the heap to pop!");
        auto minimum = heap.minimum();
        records[minimum].pop_back();
        if (records[minimum].empty()) heap.popMinimum();
        N -= 1;
    }

    bool empty() {
        return N == 0;
    }

    T top() {
        auto minimum = heap.minimum();
        T record = records[minimum].back();
        return record;
    }
};

#endif //ASSIGNMENT2_FIBONACCIHEAP_H
