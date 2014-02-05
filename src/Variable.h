#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>

using namespace std;

namespace MRF_DD {

    class BinaryVariable {
    public:
        BinaryVariable() { id = -1; log_potential = 0.0; }
        virtual ~BinaryVariable() {}
        vector<double> value;

        
        int Degree() { return factors.size(); }
        
        void LinkToFactor(class Factor *factor, int link_id) {
            factors.push_back(factor);
            links.push_back(link_id);
        }

        bool IsLabeled() {
            return labeled;
        }
        
        void IsLabeled(bool label) {
            this->labeled = label;
        }
        
        void SetId(int id) {
            this->id = id;
        }
        int GetId() {
            return this->id;
        }
        
        int GetLinkId(int i) {
            return links[i];
        }
        
        class Factor *GetFactor(int i) {
            return factors[i];
        }

        // Get/Set log-potential.
        double GetLogPotential() {
            return log_potential;
        }
        void SetLogPotential(double log_potential) {
            this->log_potential = log_potential;
        }

        
        int id;
        double log_potential;
        bool labeled;
        int label;
        int assignment;
        vector<Factor*> factors;
        vector<int> links;
        vector<int> features;
        vector<int> values;
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
            return binary_variables[i]->GetLogPotential();
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


