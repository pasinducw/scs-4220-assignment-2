//
// Created by Pasindu Wijesena on 2022-02-09.
//

#include <cmath>
#include "VEBTree.h"

static int maxBit(int value) {
    int pos = 31;
    while (pos >= 0) {
        if (value & (1 << pos)) return pos;
        pos--;
    }

    // Should not happen
    throw std::out_of_range("Cannot find a max bit for the provided value");
}


VEBTree::VEBTree(int elements) {
    if (elements <= 1) throw std::invalid_argument("Number of elements have to be greater than 0");

    int candidateU = 2;
    while (candidateU < elements) {
        candidateU <<= 1;
    }

    u = candidateU;
    upperSqrtUShifts = ((maxBit(u) + 1) / 2);
    lowerSqrtUShifts = (maxBit(u) / 2);
    minElement = maxElement = UNDEFINED;

    if (elements > 2) {
        // the structure can recurse more
        this->summary = new VEBTree((1 << upperSqrtUShifts));
        this->clusters = new VEBTree *[(1 << upperSqrtUShifts)];
        for (int i = 0; i < (1 << upperSqrtUShifts); i += 1) {
            this->clusters[i] = new VEBTree((1 << lowerSqrtUShifts));
        }
    } else {
        this->summary = nullptr;
        this->clusters = nullptr;
    }
}

VEBTree::~VEBTree() {
    if (this->u > 2) {
        delete this->summary;
        for (int i = 0; i < (1 << upperSqrtUShifts); i += 1) {
            delete this->clusters[i];
        }
        delete this->clusters;
    }
}

int VEBTree::minimum() {
    return minElement;
}

int VEBTree::maximum() {
    return maxElement;
}

bool VEBTree::member(int x) {
    if (x == minElement || x == maxElement) return true;
    else if (u == 2) return false;
    else {
        return clusters[high(x)]->member(low(x));
    }
}

int VEBTree::successor(int x) {
    if (u == 2) {
        if (x == 0 && maxElement == 1) return 1;
        else return UNDEFINED;
    } else if (minElement != UNDEFINED && x < minElement) return minElement;
    else {
        auto maxLow = clusters[high(x)]->maximum();
        if (maxLow != UNDEFINED && low(x) < maxLow) {
            auto offset = clusters[high(x)]->successor(low(x));
            return index(high(x), offset);
        } else {
            auto successorCluster = summary->successor(high(x));
            if (successorCluster == UNDEFINED) return UNDEFINED;
            else {
                auto offset = clusters[successorCluster]->minimum();
                return index(successorCluster, offset);
            }
        }
    }
}

int VEBTree::predecessor(int x) {
    if (u == 2) {
        if (x == 1 && minElement == 0) return 0;
        else return UNDEFINED;
    } else if (maxElement != UNDEFINED && x > maxElement) return maxElement;
    else {
        auto minLow = clusters[high(x)]->minimum();
        if (minLow != UNDEFINED && low(x) > minLow) {
            auto offset = clusters[high(x)]->predecessor(low(x));
            return index(high(x), offset);
        } else {
            auto predecessorCluster = summary->predecessor(high(x));
            if (predecessorCluster == UNDEFINED) {
                if (minElement != UNDEFINED && x > minElement) return minElement;
                else return UNDEFINED;
            } else {
                auto offset = clusters[predecessorCluster]->maximum();
                return index(predecessorCluster, offset);
            }
        }
    }
}

void VEBTree::emptyTreeInsert(int x) {
    minElement = maxElement = x;
}

void VEBTree::insert(int x) {
    if (minElement == UNDEFINED) emptyTreeInsert(x);
    else {
        if (x < minElement) std::swap(minElement, x);
        if (u > 2) {
            if (clusters[high(x)]->minimum() == UNDEFINED) {
                summary->insert(high(x));
                clusters[high(x)]->emptyTreeInsert(low(x));
            } else clusters[high(x)]->insert(low(x));
        }
        if (x > maxElement) maxElement = x;
    }
}

void VEBTree::remove(int x) {
    if (minElement == maxElement) minElement = maxElement = UNDEFINED;
    else if (u == 2) {
        if (x == 0) minElement = 1;
        else minElement = 0;
        maxElement = minElement;
    } else {
        if (x == minElement) {
            auto firstCluster = summary->minimum();
            x = index(firstCluster, clusters[firstCluster]->minimum());
            minElement = x;
        }
        clusters[high(x)]->remove(low(x));
        if (clusters[high(x)]->minimum() == UNDEFINED) {
            summary->remove(high(x));
            if (x == maxElement) {
                auto summaryMax = summary->maximum();
                if (summaryMax == UNDEFINED) maxElement = minElement;
                else maxElement = index(summaryMax, clusters[summaryMax]->maximum());
            }
        } else if (x == maxElement) maxElement = index(high(x), clusters[high(x)]->maximum());
    }
}

int VEBTree::index(int high, int low) {
    return (high << lowerSqrtUShifts) + (low);
}

int VEBTree::low(int key) {
    return key & ((1 << lowerSqrtUShifts) - 1);
}

int VEBTree::high(int key) {
    return key >> lowerSqrtUShifts;
}
