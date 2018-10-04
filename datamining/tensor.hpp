//
//  tensor.hpp
//  datamining
//
//  Created by Clayton Knittel on 9/20/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef tensor_hpp
#define tensor_hpp

#include <stdio.h>

class tensor {
public:
    tensor(unsigned int rank);

private:
    unsigned int rank;
};

class vector: public tensor {
public:
    vector(int dim, float args[]);
    const char * to_str();
    
private:
    float *v;
    int size;
};

class matrix: public tensor {
public:
    matrix(int dim1, int dim2, vector args[]);
    
private:
    vector *m;
    int size;
};


#endif /* tensor_hpp */
