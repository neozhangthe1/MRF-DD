#include "Factor.h"

#include <cmath>

using namespace std;

namespace MRF_DD {

    
    double FactorAtMostOne::SolveMAP(vector<double> additional_potential) {
        double objective = 0.0;
        double max_potential = 0.0;
        double max_var = -1;
        
        for (int i = 0; i < variables.size(); i++) {
            values[i] = -1;
            double potential = (variables[i]->GetPotential() + additional_potential[i]);
            objective += (-potential);
            if (potential > max_potential) {
                max_var = i;
            }
        }
        
        if (max_var > 0) {
            values[max_var] = 1;
            objective += max_potential;
            objective += max_potential;
        }
        
        return objective;
    }
    
    int FactorAtMostOne::GetFeatureOffset(vector<int> assignment) {
        return 0;
    }
    
    double FactorDense::SolveMAP(vector<double> additional_potential) {
        double objective = -100000000;
        vector<int> curt_assignment;
        curt_assignment.resize(variables.size(), -1);
        
        // Generate permutation of variable assignment.
        int num_permutation = pow(2, variables.size());
        for (int i = 0; i < num_permutation; i++) {
            int j = 0;
            while (1) {
                if (curt_assignment[j] == -1) {
                    break;
                } else {
                    curt_assignment[j] = -1;
                }
            }
            curt_assignment[j] = 1;
            
            // Calculate objective.
            double curt_objective = 0.0;
            for (int k = 0; k < variables.size(); k++) {
                curt_objective += curt_assignment[k] * (variables[k]->GetPotential() + additional_potential[k]);
            }
            if (curt_objective > objective) {
                objective = curt_objective;
                this->values = curt_assignment;
            }
        }
        
        return objective;
    }
    
    int FactorDense::GetFeatureOffset(vector<int> assignment) {
        int count = 0;
        for (int i = 0; i < assignment.size(); i++) {
            if (assignment[i] == 1) {
                count += 1;
            }
        }
        return count;
    }

    double FactorXOR::SolveMAP(vector<double> additional_potential) {
        return 0.0;
    }
    
    int FactorXOR::GetFeatureOffset(vector<int> assignment) {
        return 0;
    }

	
}


