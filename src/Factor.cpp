#include "Factor.h"

using namespace std;

namespace MRF_DD {
    
    void FactorAtMostOne::SolveMAP(const vector<double> &variable_log_potentials,
                                  const vector<double> &additional_log_potentials,
                                  vector<double> *variable_posteriors,
                                  vector<double> *additional_posteriors,
                                  double *value) {
        
        // Create a local copy of the log potentials.
        vector<double> log_potentials(variable_log_potentials);

        for (int f = 0; f < variables.size(); ++f) {
            if (negated[f]) {
                log_potentials[f] = -log_potentials[f];
            }
        }
        
        *value = 0.0;
        for (int f = 0; f < variables.size(); ++f) {
            if (negated[f]) {
                *value -= log_potentials[f];
            }
        }
        
        // find the variable with the largest potential.
        int first = -1;
        for (int f = 0; f < variables.size(); ++f) {
            if (first < 0 || log_potentials[f] > log_potentials[first]) {
                first = f;
            }
        }
        
        bool all_zeros = true; 
        // if all the potential are negtive, then all zero.
        if (log_potentials[first] > 0.0) {
            *value += log_potentials[first];
            all_zeros = false;
        }
        
        for (int f = 0; f < variables.size(); f++) {
            if (negated[f]) {
                (*variable_posteriors)[f] = 1.0;
            } else {
                (*variable_posteriors)[f] = 0.0;
            }
        }
        
        if (!all_zeros) {
            (*variable_posteriors)[first] = negated[first]? 0.0 : 1.0;
        }
    }
    
    void FactorPAIR::SolveMAP(const vector<double> &variable_log_potentials,
                                   const vector<double> &additional_log_potentials,
                                   vector<double> *variable_posteriors,
                                   vector<double> *additional_posteriors,
                                   double *value) {
        
        // Create a local copy of the log potentials.
        vector<double> log_potentials(variable_log_potentials);
        
        for (int f = 0; f < variables.size(); ++f) {
            if (negated[f]) {
                log_potentials[f] = -log_potentials[f];
            }
        }
        
        *value = 0.0;
        for (int f = 0; f < variables.size(); ++f) {
            if (negated[f]) {
                *value -= log_potentials[f];
            }
        }
        
        // find the variable with the largest potential.
        int first = -1;
        for (int f = 0; f < variables.size(); ++f) {
            if (first < 0 || log_potentials[f] > log_potentials[first]) {
                first = f;
            }
        }
        
        bool all_zeros = true;
        // if all the potential are negtive, then all zero.
        if (log_potentials[first] > 0.0) {
            *value += log_potentials[first];
            all_zeros = false;
        }
        
        for (int f = 0; f < variables.size(); f++) {
            if (negated[f]) {
                (*variable_posteriors)[f] = 1.0;
            } else {
                (*variable_posteriors)[f] = 0.0;
            }
        }
        
        if (!all_zeros) {
            (*variable_posteriors)[first] = negated[first]? 0.0 : 1.0;
        }
    }

    
    void FactorXOR::SolveMAP(const vector<double> &variable_log_potentials,
                                   const vector<double> &additional_log_potentials,
                                   vector<double> *variable_posteriors,
                                   vector<double> *additional_posteriors,
                                   double *value) {
        
        // Create a local copy of the log potentials.
        vector<double> log_potentials(variable_log_potentials);
        
        for (int f = 0; f < variables.size(); ++f) {
            if (negated[f]) {
                log_potentials[f] = -log_potentials[f];
            }
        }
        
        *value = 0.0;
        for (int f = 0; f < variables.size(); ++f) {
            if (negated[f]) {
                *value -= log_potentials[f];
            }
        }
        
        // find the variable with the largest potential.
        int first = -1;
        for (int f = 0; f < variables.size(); ++f) {
            if (first < 0 || log_potentials[f] > log_potentials[first]) {
                first = f;
            }
        }
        
        bool all_zeros = true;
        // if all the potential are negtive, then all zero.
        if (log_potentials[first] > 0.0) {
            *value += log_potentials[first];
            all_zeros = false;
        }
        
        for (int f = 0; f < variables.size(); f++) {
            if (negated[f]) {
                (*variable_posteriors)[f] = 1.0;
            } else {
                (*variable_posteriors)[f] = 0.0;
            }
        }
        
        if (!all_zeros) {
            (*variable_posteriors)[first] = negated[first]? 0.0 : 1.0;
        }
    }

    
	
}


