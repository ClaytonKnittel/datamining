//
//  sep25.h
//  datamining
//
//  Created by Clayton Knittel on 9/25/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef sep25_h
#define sep25_h

#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <map>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;

float log_2(float f) {
    if (f == 0)
        return 0;
    return std::log2(f);
}

vector<vector<string>>* group(const vector<string> data) {
    vector<vector<string>> *ret = new vector<vector<string>>(data.size());
    for (int i = 0; i < data.size(); i++) {
        std::stringstream str(data[i]);
        string temp;
        while (str >> temp)
            (*ret)[i].push_back(temp);
    }
    return ret;
}

vector<string>* parse(string input) {
    vector<string> *ret = new vector<string>();
    std::stringstream str(input);
    string temp;
    while (str >> temp)
        (*ret).push_back(temp);
    return ret;
}

float entropy(int *totals, int total, int len) {
    float entr = 0;
    for (int i = 0; i < len; i++) {
        float p = float(totals[i]) / total;
        entr -= p * log_2(p);
    }
    return entr;
}

template<typename T>
float entropy(vector<T> data) {
    float len = data.size();
    float entr = 0;
    for (int i = 0; i < len; i++) {
        int tot = 1;
        for (int j = i + 1; j < len; j++) {
            if (data[j] == data[i]) {
                tot++;
                data.erase(data.begin() + j);
                j--;
            }
        }
        data.erase(data.begin() + i);
        i--;
        entr -= tot / len * log_2(tot / len);
    }
    return entr;
}

void constructMap(vector<vector<string>> data, vector<map<string, int>> &m) {
    m = *(new vector<map<string, int>>(data[0].size()));
    const int numAttrs = int(data[0].size());
    int i;
    for (int attr = 0; attr < numAttrs; attr++) {
        i = 0;
        map<string, int> &cur = m[attr];
        for (int example = 0; example < data.size(); example++) {
            if (cur.find(data[example][attr]) == cur.end())
                cur[data[example][attr]] = i++;
        }
    }
}

/**
 
 Note: norm must be one-to-one, otherwise
 this may produce unexpected results. This
 condition is not checked by this method.
 
 */
template <typename A, typename B>
void inverseMap(map<A, B> &norm, map<B, A> &inv) {
    for (std::pair<A, B> p : norm) {
        inv[p.second] = p.first;
    }
}

/**
 
 make [0, 1, 2, ..., n]
 
 */
vector<int> enumerate(int len) {
    vector<int> en(len);
    for (int i = 0; i < en.size(); i++)
        en[i] = i;
    return en;
}

template<typename T>
int argmax(vector<T> v) {
    T max = v[0];
    int i = 0;
    for (int j = 1; j < v.size(); j++) {
        if (v[j] > max) {
            max = v[j];
            i = j;
        }
    }
    return i;
}


class decisionTree {
private:
    class node {
    public:
        node() {};
        // called only for the root node
        node(vector<vector<string>> data, decisionTree &tree): node(data, tree, enumerate(int(tree.attrLocs.size() - 1)), 0, -1) {}
        
        ~node() {
            delete value;
            if (babies != nullptr) {
                for (node *n : *babies) {
                    delete n;
                }
                delete babies;
            }
        }
        
        void printstuff() {
            cout << babies->size() << endl;
            for (node *f : *babies)
                f->printstuff();
        }
        
        string val(vector<string> input, decisionTree &tree) {
            if (leaf)
                return *value;
            return (*babies)[tree.attrLocs[attribute][input[attribute]]]->val(input, tree);
        }
        
    private:
        
        bool leaf;
        string *value;
        float confidence;
        
        // index of the attribute that this node
        // separates the data by
        int attribute;
        vector<node*> *babies;
        
        int depth;
        
        node(vector<vector<string>> &subdat, decisionTree &tree, vector<int> availableAttrs, int depth, int mostCommonOutcomeOfParent) {
            this->depth = depth;
            tree.depth = std::max(tree.depth, depth);
            
            int outcome = int(tree.attrLocs.size()) - 1;
            int numOutcomes = int(tree.attrLocs[outcome].size());
            
            babies = nullptr;
            
            
            // first, see if this is a leaf node (all elements
            // have the same outcome
            bool same = true;
            for (int i = 1; i < subdat.size(); i++) {
                if (subdat[i][outcome] != subdat[0][outcome]) {
                    same = false;
                    break;
                }
            }
            if (same) {
                leaf = true;
                value = new string(subdat[0][outcome]);
                confidence = 1.f;
                
                attribute = -1;
                return;
            }
            
            // total number of examples with each
            // possible outcome
            vector<int> tots(numOutcomes);
            for (vector<string> sub : subdat)
                tots[tree.attrLocs[outcome][sub[outcome]]]++;
            
            // compute most common outcome of this node's
            // data to be passed onto children later
            int mostCommonOutcome = argmax(tots);
            
            // next, see if there are no more attributes
            // to be selected
            if (availableAttrs.size() == 0) {
                // then just label with most common element
                // of the set
                value = new string(tree.locNames[outcome][mostCommonOutcome]);
                leaf = true;
                confidence = float(tots[mostCommonOutcome]) / subdat.size();
                
                attribute = -1;
                return;
            }
            
            // next, see if there ar eno examples in subdat
            // this means the parent decided to split on an
            // attribute, but one of the possible values of
            // that attribute was not in any of the data
            if (subdat.size() == 0) {
                // then we just choose the most common class
                // of examples in the parent node's dataset
                value = new string(tree.locNames[outcome][mostCommonOutcomeOfParent]);
                leaf = true;
                confidence = 0.f;
                
                attribute = -1;
                return;
            }
            
            value = new string("node");
            leaf = false;
            confidence = -1.f;
            
            // split data all possible ways and find which
            // way has highest infogain (i.e. minimum entropy)
            float minEntropy = MAXFLOAT;
            int bestAttr = -1;
            float entr;
            for (int attr: availableAttrs) {
                entr = 0.f;
                // split the data by this attribute
                const int numAttrs = int(tree.attrLocs[attr].size());
                
                
                // array for keeping track of the number
                // of occurences of each possible outcome
                // (i.e. number of Y's, number of N's, ...)
                // for each possible attribute value of the
                // given split
                int totals[numAttrs][numOutcomes + 1];
                //initialize to all 0's
                memset(totals, 0, numAttrs * (numOutcomes + 1) * sizeof(int));
                
                for (vector<string> datapt : subdat) {
                    int attrVal = tree.attrLocs[attr][datapt[attr]];
                    int decisionVal = tree.attrLocs[outcome][datapt[outcome]];
                    totals[attrVal][decisionVal]++;
                    
                    // used to keep track of the total number of
                    // points in this new node, just so we don't
                    // have to total the elements of totals[attrVal]
                    // later
                    totals[attrVal][numOutcomes]++;
                }
                
//                for (int *attrval : totals) {
//                    cout << "attribute " << (attrval - totals[0]) / 3 << ": ";
//                    for (int i = 0; i <= numOutcomes; i++) {
//                        cout << *(attrval + i) << " ";
//                    }
//                    cout << endl;
//                }
                
                // find the total entropy
                for (int *attrSplit : totals) {
                    // proportion of the number of elements in t
                    // to total number of elements in the subdata
                    float p = float(*(attrSplit + numOutcomes)) / subdat.size();
                    
                    entr += p * entropy(attrSplit, *(attrSplit + numOutcomes), numOutcomes);
                }
                if (entr < minEntropy) {
                    minEntropy = entr;
                    bestAttr = attr;
                }
            }
            
            // now that we have found the best attribute
            // to split by, we actually split the data
            // and recursively call this constructor on
            // the new nodes created
            attribute = bestAttr;
            
            int numAttrs = int(tree.attrLocs[bestAttr].size());
            availableAttrs.erase(std::find(availableAttrs.begin(), availableAttrs.end(), bestAttr));
            
            // [subset for individual node][datapts][attrs]
            vector<vector<vector<string>>> newsubdatas(numAttrs);
            
            map<string, int> &bestAttrMap = tree.attrLocs[bestAttr];
            for (vector<string> datapt : subdat) {
                newsubdatas[bestAttrMap[datapt[bestAttr]]].push_back(datapt);
            }
            babies = new vector<node*>();
            for (int i = 0; i < numAttrs; i++) {
                babies->push_back(new node(newsubdatas[i], tree, availableAttrs, depth + 1, mostCommonOutcome));
            }
        }
    };
    
    // list of maps from attribute (i.e. hot, cold) to the
    // index of the node in a list of branches off of another
    // node (i.e. {hot: 0, cold: 1})
    vector<map<string, int>> attrLocs;
    
    // just the inverse of the above map
    vector<map<int, string>> locNames;
    
    node *root;
    int depth;
    
    decisionTree(vector<vector<string>> data): attrLocs(data[0].size()) {
        constructMap(data, attrLocs);
        
        // construct the inverse map (for printing and stuff)
        locNames = *(new vector<map<int, string>>(attrLocs.size()));
        for (int i = 0; i < attrLocs.size(); i++)
            inverseMap(attrLocs[i], locNames[i]);
        
        root = new node(data, *this);
    }
    
public:
    
    /**
     data input in the format:
     
     {"attr1 attr2 ... val1"
      "attr1 attr2 ... val2"
         .     .        .
         .     .        .
         .     .        .
     }
     */
    decisionTree(vector<string> data): decisionTree(*group(data)) {}
    ~decisionTree() {
        delete root;
    }
    
    void p() {
        root->printstuff();
    }
    
    string val(string input) {
        return root->val(*parse(input), *this);
    }
};

void run() {
    /*
    
    Decision trees:
     
     
     H(S) = sum of P(something) * log2(something)
     
     infogain(S, O) = H(S) - sum over the disjoint subets O (Y and N) H(S|O)
    
    */
    
    vector<string> data;
    data.push_back(string("sunny hot hum low no"));
    data.push_back(string("sunny hot hum high no"));
    data.push_back(string("overcast hot hum low yes"));
    data.push_back(string("rain s hum low yes"));
    data.push_back(string("rain cold num low yes"));
    data.push_back(string("rain cold num high no"));
    data.push_back(string("overcast cold num high yes"));
    data.push_back(string("sunny s hum low no"));
    data.push_back(string("sunny s hum low no"));
    data.push_back(string("rain s num low yes"));
    data.push_back(string("sunny s num high yes"));
    data.push_back(string("overcast s hum high yes"));
    data.push_back(string("overcast hot num low yes"));
    data.push_back(string("rain s hum high no"));
    
//    data.push_back(string("cold late no"));
//    data.push_back(string("cold early no"));
//    data.push_back(string("warm late yes"));
//    data.push_back(string("warm early yes"));
    
    decisionTree tree(data);
    
    cout << tree.val("rain cold num low") << endl;
    
//    cout << tree.val("sunny hot hum low") << endl;
//    cout << tree.val("rain s num low") << endl;
}

#endif /* sep25_h */
