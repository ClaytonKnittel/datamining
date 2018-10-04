//
//  tensor.cpp
//  datamining
//
//  Created by Clayton Knittel on 9/20/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include "tensor.hpp"
#include <string>
#include <sstream>
#include <iostream>

tensor::tensor(unsigned int rank) {
    this->rank = rank;
}

vector::vector(int size, float args[]=nullptr) : tensor(1) {
    if (args == nullptr)
        v[size] = {};
    else
        this->v = args;
    this->size = size;
}

const char * vector::to_str() {
    std::stringstream s;
    s << "(";
    for (int x = 0; x < size; x++) {
        s << *(v + x);
        if (x < size - 1)
            s << ", ";
    }
    s << ")";
    return s.str().c_str();
}

