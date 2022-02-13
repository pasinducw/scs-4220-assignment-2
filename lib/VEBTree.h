//
// Created by Pasindu Wijesena on 2022-02-09.
//

#ifndef ASSIGNMENT2_VEBTREE_H
#define ASSIGNMENT2_VEBTREE_H

#include <vector>
#include <list>
#include <unordered_map>

#define UNDEFINED -1000000000

class VEBTree {
private:
    VEBTree **clusters;
    VEBTree *summary;

    int minElement;
    int maxElement;

    int u;
    int upperSqrtUShifts;
    int lowerSqrtUShifts;

    int high(int key);

    int low(int key);

    int index(int high, int low);

    void emptyTreeInsert(int);

public:
    explicit VEBTree(int);

    ~VEBTree();

    bool member(int);

    void insert(int);

    void remove(int);

    int minimum();

    int maximum();

    int successor(int);

    int predecessor(int);
};

template<typename T>
class AugmentedVEBTree {
private:
    VEBTree *heap;
    std::unordered_map<int, std::list<T>> records;
    int N = 0; // number of records
public:
    AugmentedVEBTree(int u){
        heap = new VEBTree(u);
    }

    ~AugmentedVEBTree(){
        delete heap;
    }

    void push(int key, T data) {
        if (records[key].empty()) heap->insert(key);
        records[key].push_back(data);
        N += 1;
    }

    void pop() {
        if (N == 0) throw std::underflow_error("There are no records in the heap to pop!");
        auto minimum = heap->minimum();
        records[minimum].pop_back();
        if (records[minimum].empty()) heap->remove(minimum);
        N -= 1;
    }

    bool empty() {
        return N == 0;
    }

    T top() {
        auto minimum = heap->minimum();
        T record = records[minimum].back();
        return record;
    }
};


#endif //ASSIGNMENT2_VEBTREE_H
