#include <iostream>
#include "Variable.h"
#include "Factor.h"

namespace MRF_DD {
    
    class FactorGraph {
    public:
        FactorGraph() {
            num_links = 0
            
        }
        
        ~FactorGraph() {
            for (int i = 0; i < variables.size(); ++i) {
                delete variables[i];
            }
            for (int i = 0; i < factors.size(); ++i) {
                delete factors[i];
            }
        }
        
        Factor *CreateFactorAtMostOne(const vector<Variable*> &variables,
                                      const vector<bool> &negated,
                                      bool owned_by_graph = true) {
            Factor *factor = new FactorAtMostOne;
            DeclareFactor(factor, variables, negated, owned_by_graph);
            assert(variables.size() > 1);
            return factor;
        }
        
        Factor *CreateFactorPair(const vector<Variable*> &variables,
                                 double edge_log_potential,
                                 bool owned_by_graph = true) {
            Factor *factor = new FactorPair;
            vector<bool> negated;
            DeclareFactor(factor, variables, negated, owned_by_graph);
            vector<double> additional_log_potentials(1, edge_log_potential);
            factor->SetAdditionalLogPotentials(additional_log_potentials);
            return factor;
        }            

    private:
        int num_links;
        vector<Variable*> variables;
        vector<Factor*> factors;

        int psdd_max_iteration;
        double psdd_eta;

        vector<double> lambdas;
        vector<double> maps;
        vector<double> maps_av;
        
    };
}