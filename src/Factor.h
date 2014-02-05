#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include "Variable.h"

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
        };
    };
    
    
    class Factor {
    public:
        Factor() {};
        virtual ~Factor() {}
        
        // Return the type
        virtual int type() = 0;
    
        
        // Return the number of binary variables linked to the factor
        int Degree() { return variables.size(); }
        
        // Return a binary variable.
        BinaryVariable *GetVariable(int i) { return variables[i]; }
        
        bool IsVariableNegated(int i) { return negated[i]; }
        
        virtual void Initialize(const vector<BinaryVariable*> &variables, const vector<bool> &negated, int *link_id) {
            this->variables = variables;
            if (negated.size() == 0) {
                this->negated.assign(variables.size(), false);
            } else {
                this->negated = negated;
            }
            links.resize(variables.size());
            for (int i = 0; i < variables.size(); i++) {
                this->links[i] = *link_id;
                this->variables[i]->LinkToFactor(this, links[i]);
                ++(*link_id);
            }
        }
        
        int GetId() { return this->id; }
        int SetId(int id) { this->id = id; }
        
        int GetLinkId(int i) { return links[i]; }
        
        // Compute the MAP (local subproblem in the projected subgradient algorithm).
        virtual void SolveMAP(const vector<double> &variable_log_potentials,
                              const vector<double> &additional_log_potentials,
                              vector<double> *variable_posteriors,
                              vector<double> *additional_posteriors,
                              double *value) = 0;
        

        int id;
        int name;
        
        vector<BinaryVariable*> variables;
        vector<MultiVariable*> multi_variables;
        vector<bool> negated;
        vector<int> links;
        vector<double> additional_log_potentials;
    };
    
    // AtMostOne factor. Only configurations with at most one 1 are legal
    class FactorAtMostOne: public Factor {
    public:
        FactorAtMostOne() {};
        int type() { return FactorTypes::FACTOR_ATMOSTONE; }
        
        // Compute the MAP (local subproblem in the projected subgradient algorithm
        void SolveMAP(const vector<double> &variable_log_potentials,
                      const vector<double> &additional_log_potentials,
                      vector<double> *variable_posteriors,
                      vector<double> *additional_posteriors,
                      double *value);
        
    };
    
    class FactorXOR: public Factor {
    public:
        FactorXOR() {};
        int type() { return FactorTypes::FACTOR_XOR; }
        
        void SolveMAP(const vector<double> &variable_log_potentials,
                      const vector<double> &additional_log_potentials,
                      vector<double> *variable_posteriors,
                      vector<double> *additional_posteriors,
                      double *value);
    };

    class FactorPAIR: public Factor {
    public:
        FactorPAIR() {};
        int type() { return FactorTypes::FACTOR_PAIR; }
        
        void SolveMAP(const vector<double> &variable_log_potentials,
                      const vector<double> &additional_log_potentials,
                      vector<double> *variable_posteriors,
                      vector<double> *additional_posteriors,
                      double *value);
    };
}


