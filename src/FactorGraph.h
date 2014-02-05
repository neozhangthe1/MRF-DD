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
            num_links = 0;
            num_features = 0;
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

        void DeclareFactor(Factor* factor, const vector<BinaryVariable*> &variables, const vector<bool> &negated) {
            factor->SetId(factors.size());
            factor->Initialize(variables, negated, &num_links);
            factors.push_back(factor);
        }

        Factor* CreateFactorXOR(const vector<BinaryVariable*> &variables, const vector<bool> &negated) {
            Factor* factor = new FactorXOR;
            DeclareFactor(factor, variables, negated);
            return factor;
        }
        
        Factor* CreateFactorAtMostOne(const vector<BinaryVariable*> &variables,
                                      const vector<bool> &negated) {
            Factor *factor = new FactorAtMostOne;
            DeclareFactor(factor, variables, negated);
            assert(variables.size() > 1);
            return factor;
        }                         
        
        Factor* CreateFactorPAIR(const vector<BinaryVariable*> &variables,
                                 double edge_log_potential) {
            Factor *factor = new FactorPAIR;
            vector<bool> negated;
            DeclareFactor(factor, variables, negated);
           // vector<double> additional_log_potentials(1, edge_log_potential);
           // factor->SetAdditionalLogPotentials(additional_log_potentials);
            return factor;
        }
        
        void ConvertToBinaryFactorGraph(FactorGraph *binary_graph);

        void GetBestConfiguration(int num_variable, int num_states);
        
        void GenFeature();
        
        int Train();
        
        int Inference(double lower_bound,
                      vector<double> *posteriors,
                      vector<double> *additional_posteriors,
                      double *value,
                      double *upper_bound);

        int num_links;
        int num_features;
        int num_local_param;
        int num_label;
        int num_feature_type;
        int num_factor_type;
    

        vector<BinaryVariable*> variables;
        vector<MultiVariable*> multi_variables;
        vector<Factor*> factors;

        int psdd_max_iteration;
        double psdd_eta;

        vector<double> w;
        vector<double> lambdas;
        vector<double> maps;
        vector<double> maps_av;
        

        
        map<string, int> feature_type;
        
        
    };
}