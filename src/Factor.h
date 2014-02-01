#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>

using namespace std;

namespace MRF_DD {
    
    struct FactorTypes {
        enum {
            FACTOR_GENERIC = 0,
            FACTOR_PAIR,
            FACTOR_XOR,
            FACTOR_OR,
            FACTOR_OROUT,
            FACTOR_ATMOSTONE,
            FACTOR_BUDGET,
            FACTOR_KNAPSACK,
            FACTOR_MULTI_DENSE
        }
    };
    
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
    
    class Factor {
    public:
        Factor();
        virtual ~Factor() {}
        
        // Return the type
        virtual int type() = 0;
        
        // Return the number of binary variables linked to the factor
        int Degree() { return }
        
        // Compute the MAP (local subproblem in the projected subgradient algorithm).
        virtual void SolveMAP(const vector<double> &variable_log_potentials,
                              const vector<double> &additional_log_potentials,
                              vector<double> *variable_posteriors,
                              vector<double> *additional_posteriors,
                              double *value) = 0;
        
    private:
        int id;
        
    projected:
        vector<Variable*> variables;
        vector<bool> negated;
        vector<int> links;
        vector<double> additional_log_potentials;
    };
    
    // AtMostOne factor. Only configurations with at most one 1 are legal
    class FactorAtMostOne: public Factor {
    public:
        int type() { return FactorTypes::FACTOR_ATMOSTONE; }
        
        // Compute the MAP (local subproblem in the projected subgradient algorithm
        void SolveMAP(const vector<double> &variable_log_potentials,
                      const vector<double> &additional_log_potentials,
                      vector<double> *variable_posteriors,
                      vector<double> *additional_posteriors,
                      double *value);
        
    };

	
}


