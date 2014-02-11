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
            FACTOR_DENSE,
            FACTOR_COUNT
        };
    };
    
    
    class Factor {
    public:
        Factor() {};
        virtual ~Factor() {}
        
        // Return the type
        virtual int type() = 0;
    
        
        // Return the number of binary variables linked to the factor.
        int Degree() { return variables.size(); }
        
        // Return a binary variable.
        BinaryVariable *GetVariable(int i) { return variables[i]; }
        
        bool GetValue(int i) { return values[i]; }
        
        virtual void Initialize(const vector<BinaryVariable*> &variables, int *link_id, int factor_key) {
            this->variables = variables;
            this->factor_key = factor_key;
            links.resize(variables.size());
            values.resize(variables.size());
            labels.resize(variables.size());
            for (int i = 0; i < variables.size(); i++) {
                auto var = variables[i];
                this->links[i] = *link_id;
                this->variables[i]->LinkToFactor(this, links[i]);
                values[i] = var->GetValue();
                labels[i] = var->GetLabel();
                ++(*link_id);
            }
        }
        
        int GetId() { return this->id; }
        int SetId(int id) { this->id = id; }
        
        int GetLinkId(int i) { return links[i]; }
        
        // Calculate objective of the factor.
        double CalcObjective(vector<double> additional_potential, bool labeled = false) {
            double objective = 0.0;
            if (labeled) {
                for (int i = 0; i < variables.size(); i++) {
                    auto var = variables[i];
                    objective += var->GetLabel() * (var->GetPotential() + additional_potential[i]);
                }
            } else {
                for (int i = 0; i < variables.size(); i++) {
                    auto var = variables[i];
                    objective += values[i] * (var->GetPotential() + additional_potential[i]);
                }
            }
        }
        
        // Get feature id for factor.
        virtual int GetFeatureOffset(vector<int> assignment) = 0;
        
        // Compute the MAP (local subproblem in the projected subgradient algorithm).
        virtual double SolveMAP(vector<double> additional_potential) = 0;
        
        // Miscellaneous.
        int id;
        int factor_key;
        
        vector<BinaryVariable*> variables;
        vector<MultiVariable*> multi_variables;
        
        // Store values of the variables.
        vector<int> values;
        vector<int> labels;
        vector<int> links;
        vector<double> potentials;

    };
    
    // AtMostOne factor. Only configurations with at most one 1 are legal
    class FactorAtMostOne: public Factor {
    public:
        FactorAtMostOne() {};
        int type() { return FactorTypes::FACTOR_ATMOSTONE; }
        double SolveMAP(vector<double> additional_potential);
        int GetFeatureOffset(vector<int> assignment);
    };
    
    class FactorDense: public Factor {
    public:
        FactorDense() {};
        int type() { return FactorTypes::FACTOR_DENSE; }
        double SolveMAP(vector<double> additional_potential);
        int GetFeatureOffset(vector<int> assignment);
    };
    
    class FactorXOR: public Factor {
    public:
        FactorXOR() {};
        int type() { return FactorTypes::FACTOR_XOR; }
        double SolveMAP(vector<double> additional_potential);
        int GetFeatureOffset(vector<int> assignment);
    };




}


