#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>

using namespace std;

namespace MRF_DD {
    
    struct VariableTypes {
        enum {
            VAR_TRAINING = 0,
            VAR_TEST,
            VAR_UNKNOWN,
        };
    };
    

    class BinaryVariable {
    public:
        BinaryVariable() { id = -1; potential = 0.0; }
        virtual ~BinaryVariable() {}

        
        int Degree() { return factors.size(); }
        
        void LinkToFactor(class Factor *factor, int link_id) {
            factors.push_back(factor);
            links.push_back(link_id);
        }
        
        // Get/Set id.
        void SetId(int id) {
            this->id = id;
        }
        int GetId() {
            return id;
        }

        // Get/Set type.
        void SetType(int type) {
            this->type = type;
        }
        bool GetType() {
            return type;
        }
        
        // Get/Set value.
        void SetValue(int value) {
            this->value = value;
        }
        int GetValue() {
            return value;
        }
        
        // Get/Set label.
        void SetLabel(bool label) {
            this->label = label;
        }
        int GetLabel() {
            return label;
        }
        
        // Get/Set link id
        int GetLinkId(int i) {
            return links[i];
        }
        
        // Get/Set factors.
        class Factor* GetFactor(int i) {
            return factors[i];
        }

        // Get/Set log-potential.
        double GetPotential() {
            return potential;
        }
        
        void SetLogPotential(double log_potential) {
            this->potential = potential;
        }

        // Calcuate potential of the variable
        double CalcPotential(vector<double> w);
        
        // id of the variable
        int id;
        // log potential of the variable
        double potential;
        // training, test or unknown
        int type;
        // value of the variable
        int value;
        // ground truth of the variable
        int label;
        // number of local features
        int num_feature;
        
        // factors connected to the variable
        vector<Factor*> factors;
        // links of the variable
        vector<int> links;
        // local feature id of the variable
        vector<int> features;
        // local feature values of the variable
        vector<int> feature_values;
    };
    
    // A multi-valued variable.
    // Each value is represented as a binary variable.
    class MultiVariable {
    public:
        // Number of states.
        int GetNumStates() { 
            return binary_variables.size(); 
        }

        BinaryVariable *GetState(int i) {
            return binary_variables[i];
        }

        const vector<BinaryVariable*> &GetState() {
            return binary_variables;
        }

        // Get/Set log-potentials.
        double GetLogPotential(int i) {
            return binary_variables[i]->GetPotential();
        }
        void SetLogPotential(int i, double log_potential) {
            this->binary_variables[i]->SetLogPotential(log_potential);
        }

        // Get/Set id.
        int GetId() {
            return id;
        }
        void SetId(int id) {
            this->id = id;
        }

        // Initialize states with binary variables.
        void Initialize(const vector<BinaryVariable*> &binary_variables) {
            this->binary_variables = binary_variables;
        }

        // Link to a factor.
        void LinkToFactor(class Factor* factor) {
            factors.push_back(factor);
        }

        int Degree() {
            return factors.size();
        }

    private:
        int id;
        vector<BinaryVariable*> binary_variables;
        vector<Factor*> factors;

    };


	
}


