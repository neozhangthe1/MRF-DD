#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>

using namespace std;

namespace MRF_DD {

    class Variable {
    public:
        Variable() { id = -1, log_potential = 0.0}
        virtual ~BinaryVariable() {}
        int label;
        vector<double> value;
        
        
        int Degree() { return factor_.size(); }
        
        void LinkToFactor(class Factor *factor, int link_id) {
            factors.push_back(factor);
            links.push_back(link_id);
        }
        
    private:
        int id;
        double log_potential;
        std::vector<Factor*> factors;
        std::vector<int> links;
    };
    


	
}


