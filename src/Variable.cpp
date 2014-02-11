//
//  Variable.cpp
//  
//
//  Created by Yutao Zhang on 14-2-10.
//
//

#include "Variable.h"

namespace MRF_DD {
    
    double BinaryVariable::CalcPotential(vector<double> w) {
        double value = 0.0;
        for (int i = 0; i < num_feature; i++) {
            int fid = features[i];
            int val = feature_values[i];
            value += (val * w[fid]);
        }
        this->potential = value;
        return value;
    }
    
}