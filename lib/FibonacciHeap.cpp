//
// Created by Pasindu Wijesena on 2022-02-09.
//

#include "FibonacciHeap.h"
#include <vector>
#include <iostream>

std::vector<FibonacciHeapNode *> getChildrenNodes(FibonacciHeapNode *node) {
    std::vector<FibonacciHeapNode *> siblings;

    if (node->child == nullptr) return siblings;

    siblings.push_back(node->child);
    auto current = node->child->right;
    while (current != node->child) {
        siblings.push_back(current);
        current = current->right;
    }

    return siblings;
}

std::vector<FibonacciHeapNode *> getNodesAtCurrentLevel(FibonacciHeapNode *node) {
    std::vector<FibonacciHeapNode *> siblings;
    if (node == nullptr) return siblings;


    siblings.push_back(node);

    auto base = node;
    auto current = node->right;

    while (current != base) {
        siblings.push_back(current);
        current = current->right;
    }

    return siblings;
}

FibonacciHeapNode::FibonacciHeapNode(int key) {
    degree = 0;
    p = nullptr;
    child = nullptr;
    left = this;
    right = this;
    mark = false;
    this->key = key;
}

FibonacciHeapNode::~FibonacciHeapNode() {
    right = nullptr;
    left = nullptr;
    p = nullptr;
    child = nullptr;
}

FibonacciHeap::FibonacciHeap() {
    roots = nullptr;
    minNode = nullptr;
    n = 0;
}

FibonacciHeap::~FibonacciHeap() {
    roots = nullptr;
    minNode = nullptr;
}

void FibonacciHeap::insert(int key) {
    auto node = new FibonacciHeapNode(key);
    if (minNode == nullptr) {
        roots = node;
        minNode = node;
    } else {
        insertToRootList(node);
        if (node->key < minNode->key) minNode = node;
    }
    n = n + 1;
}

void FibonacciHeap::popMinimum() {
    auto z = minNode;
    if (z != nullptr) {
        // create the child list
        std::vector<FibonacciHeapNode *> childrenNodes = getNodesAtCurrentLevel(z->child);
        for (auto x:childrenNodes) {
            insertToRootList(x);
            x->p = nullptr;
        }
        // remove z from the root list of H
        removeFromRootList(z);


        if (z == z->right) {
            minNode = nullptr;
        } else {
            minNode = z->right;
            consolidate();
        }
        n = n - 1;
    }
    delete z;
}

void FibonacciHeap::consolidate() {
    int maxD = 0;
    std::vector<FibonacciHeapNode *> rootNodes = getNodesAtCurrentLevel(roots);

    for (auto node: rootNodes) maxD = std::max(maxD, node->degree); // TODO: revise the limit
    maxD += rootNodes.size();
    std::vector<FibonacciHeapNode *> A(maxD, nullptr);

    for (auto w:rootNodes) {
        auto x = w;
        auto d = x->degree;
        while (A[d] != nullptr) {
            auto y = A[d];
            if (x->key > y->key) std::swap(x, y);
            link(y, x);
            A[d] = nullptr;
            d = d + 1;
        }
        A[d] = x;
        maxD = std::max(maxD, d);
    }
    minNode = nullptr;
    for (FibonacciHeapNode *node: A) {
        if (node != nullptr) {
            if (minNode == nullptr) {
                createNewRootList(node);
                minNode = node;
            } else {
                insertToRootList(node);
                if (node->key < minNode->key) minNode = node;
            }
        }
    }
}

void FibonacciHeap::link(FibonacciHeapNode *y, FibonacciHeapNode *x) {
    removeFromRootList(y);

    y->p = x;
    if (x->child == nullptr) {
        x->child = y;
        y->right = y;
        y->left = y;
    } else {
        y->right = x->child;
        y->left = x->child->left;
        x->child->left->right = y;
        x->child->left = y;
    }
    x->degree = x->degree + 1;
    y->mark = false;
}

void FibonacciHeap::insertToRootList(FibonacciHeapNode *node) {
    node->left = minNode->left;
    node->right = minNode;
    minNode->left->right = node;
    minNode->left = node;
}

void FibonacciHeap::createNewRootList(FibonacciHeapNode *node) {
    roots = node;
    node->right = node;
    node->left = node;
}

void FibonacciHeap::removeFromRootList(FibonacciHeapNode *node) {
    if (node->p != nullptr) throw std::invalid_argument("Node is not a root node");
    node->right->left = node->left;
    node->left->right = node->right;
    if (roots == node) {
        if (node->right == node) roots = nullptr;
        else roots = node->right;
    }

    if (minNode == node) {
        minNode = nullptr;
    }
}

int FibonacciHeap::minimum() {
    if (minNode == nullptr) {
        return UNDEFINED;
    }
    return minNode->key;
}







