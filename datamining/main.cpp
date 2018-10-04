//
//  main.cpp
//  datamining
//
//  Created by Clayton Knittel on 9/20/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <iostream>
#include "tensor.hpp"
#include "../lecturenotes/decisionTree.h"
#include "../lecturenotes/randomForest.h"
#include <vector>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

class track {
public:
    track(int val) {
        this->val = val;
    }
    ~track() {
    }
    void operator=(const track &t) {
        this->val = t.val;
    }
    int value() const {
        return val;
    }
private:
    int val;
};

class a {
public:
    static int co;
    
    a(int len): val(co++) {
        as = new vector<a*>();
        for (int fe = 0; fe < len; fe++) {
            as->push_back(new a());
        }
    }
    
    a(): val(co++) {
        as = nullptr;
    }
    
    ~a() {
        if (as != nullptr) {
            for (a *&f : *as)
                delete f;
            delete as;
        }
    }
    
    void operator=(const a &c) {
        this->val = c.val;
    }
    
    string str() {
        string ret;
        stringstream s;
        s << val.value();
        s >> ret;
        if (as != nullptr) {
            for (a *f : *as) {
                ret += f->str();
            }
        }
        return ret;
    }
    
private:
    std::vector<a*> *as;
    track val;
};
int a::co = 0;

a method(int nums) {
    a ret(nums);
    return ret;
}

int main(int argc, const char * argv[]) {
    //run();
    const int a = 1, b = 3;
    cout << max<int>(a, b) << endl;
    //run27();
//    a v(4);
//    cout << v.str() << endl;
//    a val = method(4);
//    cout << val.str() << endl;
    
    return 0;
}
