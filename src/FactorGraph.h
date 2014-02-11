#pragma once

#include <iostream>
#include <map>
#include "Variable.h"
#include "Factor.h"
#include "Util.h"
#include <math.h>

namespace MRF_DD {
    
    class FactorGraph {
    public:
        FactorGraph() {
            num_link = 0;
            num_feature = 0;
        }
        
        ~FactorGraph() {
            for (int i = 0; i < variables.size(); ++i) {
                delete variables[i];
            }
            for (int i = 0; i < factors.size(); ++i) {
                delete factors[i];
            }
        }
        
        BinaryVariable *CreateBinaryVariable() {
            BinaryVariable *variable = new BinaryVariable;
            variable->SetId(variables.size());
            variables.push_back(variable);
            return variable;
        }
        
        MultiVariable *CreateMultiVariable(int num_states) {
            MultiVariable *multi = new MultiVariable;
            multi->SetId(multi_variables.size());
            multi_variables.push_back(multi);
            vector<BinaryVariable*> states(num_states);
            for (int i = 0; i < num_states; i++) {
                BinaryVariable *state = CreateBinaryVariable();
                states[i] = state;
            }
            multi->Initialize(states);
            return multi;
        }
        
        MultiVariable *CreateMultiVariable(const vector<BinaryVariable*> &states) {
            MultiVariable *multi = new MultiVariable;
            multi->SetId(multi_variables.size());
            multi_variables.push_back(multi);
            multi->Initialize(states);
            return multi;
        }

        void DeclareFactor(Factor* factor, const vector<BinaryVariable*> &variables, const int factor_key) {
            factor->SetId(factors.size());
            factor->Initialize(variables, &num_link, factor_key);
            factors.push_back(factor);
        }
        
        Factor* CreateFactorDense(const vector<BinaryVariable*> &variables, const int factor_key) {
            Factor* factor = new FactorDense;
            DeclareFactor(factor, variables, factor_key);
            return factor;
        }

        Factor* CreateFactorXOR(const vector<BinaryVariable*> &variables, const int factor_key) {
            Factor* factor = new FactorXOR;
            DeclareFactor(factor, variables, factor_key);
            return factor;
        }
        
        Factor* CreateFactorAtMostOne(const vector<BinaryVariable*> &variables, const int factor_key) {
            Factor *factor = new FactorAtMostOne;
            DeclareFactor(factor, variables, factor_key);
            assert(variables.size() > 1);
            return factor;
        }
        
        void Initialize();
        
        int Train();
        
        int Inference();
        
        void ConvertToBinaryFactorGraph(FactorGraph *binary_graph);
        
        void GenFeature();
        
        void Evaluate();

        // num of links between variables and factors
        int num_link;
        // num of training instances
        int num_training_instance;
        // num of local feature types
        int num_feature_type;
        // num factor types
        int num_factor_type;
    
        // variable set
        vector<BinaryVariable*> variables;
        // muli variable set
        vector<MultiVariable*> multi_variables;
        // factor set
        vector<Factor*> factors;
        
        // Local potential of the variables.
        vector<double> variable_potentials;

        // max iteration
        int max_iteration;
        // step size
        double init_eta = 2.0;

        // weight parameters
        vector<double> w;
        // dual variables
        vector<double> lambda;
        
        // MAP assignment.
        vector<int> map_x;
        vector<int> average_x;
        vector<int> sum_x;
        vector<int> best_x;
        
        int num_feature;
        
        // Local feature name to id
        vector<string> feature_keys;
        vector<int> feature_to_fid;
        // Factor name to id
        vector<string> factor_keys; // size = num of factor keys
        vector<int> factor_type; // size = num of factor keys
        vector<int> factor_degree; // size = num of factor keys
        vector<int> factor_to_fid; 
        
        
    };
}