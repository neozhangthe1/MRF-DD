#include <iostream>
#include "FactorGraph.h"

namespace MRF_DD {

    void FactorGraph::ConvertToBinaryFactorGraph(FactorGraph *binary_graph) {
        vector<vector<BinaryVariable*>> converted_variables(multi_variables.size());
        for (int i = 0; i < multi_variables.size(); i++) {
            MultiVariable *multi_variable = multi_variables[i];
            converted_variables[i].resize(multi_variable->GetNumStates());
            for (int j = 0; j < multi_variable->GetNumStates(); j++) {
                converted_variables[i][j] = binary_graph->CreateBinaryVariable();
                //converted_variables[i][j] = SetLogPotential(multi_variable->GetLogPotential(j));
            }
            //binary_graph->CreateFactorXOR(converted_variables[i]);
        }
    }

    void FactorGraph::GetBestConfiguration(int num_variable, int num_states) {
        int offset = 0;
        double value = 0.0;
        int grid_size = 0;
        vector<vector<int>> best_states(grid_size, vector<int>(grid_size));

        for (int i = 0; i < num_variable; i++) {
            int best = -1;
            for (int k = 0; k < num_states; k++) {
               // if (best < 0 || posteriors[offset + k] > posteriors[offset + best]) {
                  //  best = k;
              //  }
            }
            offset += num_states;
            //best_states[i][j] = best;
        }

    }

    void FactorGraph::GenFeature() {
        num_features = 0;
        
        // local features: g(x; z)
        num_local_param = num_label * num_feature_type;
        num_features += num_local_param;
        
        // pair feature: g(x1, x2; z)
        int offset = 0;
        for (int x1 = 0; x1 < num_label; x1++) {
            for (int x2 = 0; x2 < num_label; x2++) {
                offset++;
            }
        }
        
        // dense feature: g(x1, x2, x3; z)
        for (int y1 = 0; y1 < num_label; y1++) {
            for (int y2 = y1; y2 < num_label; y2++) {
                for (int y3 = y2; y3 < num_label; y3++) {
                    offset++;
                }
            }
        }
        
    }
    double CalcGradient(double* gradient, int num_features, FactorGraph* graph) {
        // Objective
        double f;
        double *t1;
        double *t2;
        
        f = 0.0;
        for (int i = 0; i < num_features; i++) {
            gradient[i] = 0;
        }
        
        for (int i = 0; i < graph->variables.size(); i++) {
            BinaryVariable *var = graph->variables[i];
            for (int y = 0; y < num_features; y++) {
                for (int t = 0; t < num_features; t++) {
                    //gradient[] += var->value[t] * graph->variable;
                }
            }
        }

        for (int i = 0; i < graph->factors.size(); i++) {
            Factor *factor = graph->factors[i];
            for (int y = 0; y < num_features; y++) {
                //for (int t = 0; t < factor->num_features; t++) {
                 //   v *= exp()
                //}
            }
        }
        
    }

    int FactorGraph::Train() {
        // Gradient.
        double* dw;
        dw = new double[num_features + 1];
        
        int max_iter = 1000;

        // Initialization
        w.clear();
        w.resize(num_features, 0.0);
        lambdas.clear();
        lambdas.resize(num_links, 0.0);
        maps.clear();
        maps.resize(num_links, 0.0);
        maps_av.clear();
        maps_av.resize(variables.size(), 0.5);

        int num_times_increment = 0;
        double dual_obj_prev = 1e100;
        double extra_score = 0;
        double dual_obj = extra_score;

        vector<double> dual_obj_factors(factors.size(), 0.0);

        for (int t = 0; t < max_iter; t++) {
            // Set step size
            double eta = 0.0;

            // Projected gradient method

            // Optimize slave MRFs 
            for (int i = 0; i < factors.size(); ++i) {
                Factor* factor = factors[i];
                int factor_degree = factor->Degree();
                // Compute the MAP and update the dual objective
                double val;
                //factor->SolveMAP(&val);
                double delta = 0.0;
                for (int j = 0; j < factor_degree; ++j) {
                    int m = factor->GetLinkId(i);
                    delta -= lambdas[m];
                }
                dual_obj += val + delta - dual_obj_factors[i];
                dual_obj_factors[i] = val + delta;
            }

            // Update w
            
            

            // Update lambdas
            for (int i = 0; i < variables.size(); i++) {
                BinaryVariable *variable = variables[i];
                int variable_degree = variable->Degree();

                for (int j = 0; j < variable_degree; j++) {
                    int m = variable->GetLinkId(j);
                    Factor* factor = variable->GetFactor(j);
                    int k = factor->GetId();
                    double diff_penalty = maps_av[m];


                }
            }

        }

    }
    
    int FactorGraph::Inference(double lower_bound,
                             vector<double> *posteriors,
                             vector<double> *additional_posteriors,
                             double *value,
                             double *upper_bound) {
        timeval start, end;
        gettimeofday(&start, NULL);
        
        // Stopping criterion parameters.
        double residual_threshold_final = 1e-12;
        double residual_threshold = 1e-6;
        
        // Optimization status.
        bool optimal = false;
        bool reached_lower_bound = false;
        
        // Miscellaneous
        vector<double> log_potentials;
        vector<double> x0;
        vector<double> x;
        bool recompute_everything = true;
        
        int t;
        double dual_obj_best = 1e100;
        double primal_obj_best = -1e100;
        
        int num_interations_compute_dual = 50;

        // Compute extra score to account for variables that are not connected to any factor.
        double extra_score = 0.0;// ????
        for (int i = 0; i < variables.size(); ++i) {
            BinaryVariable* variable = variables[i];
            int degree = variable->Degree();
            double log_potential = variable->GetLogPotential();
            if (degree == 0 && log_potential > 0) {
                extra_score += log_potential;
            }
        }

        lambdas.clear();
        lambdas.resize(num_links, 0.0);
        // don't know?
        maps.clear();
        maps.resize(num_links, 0.0);
        maps_av.clear();
        maps_av.resize(variables.size(), 0.5);

        double dual_obj_prev = 1e100;
        double dual_obj = extra_score;
        // Stores each factor contribution to the dual objective
        vector<double> dual_obj_factors(factors.size(), 0.0);
        int psdd_max_iterations = 100;
        // Main loop.
        for (t = 0; t < psdd_max_iterations; ++t) {
            // Set stepsize.
            double eta = psdd_eta / sqrt(static_cast<double>(t+1));
            
            // Initialize all variables as inactive.
            for (int i = 0; i < variables.size(); ++i) {
                //variables_is_active[i] = false;
            }
            
            // Optimize over maps (Compute dual value)
            int num_inactive_factors = 0;
            for (int j = 0; j < factors.size(); ++j) {
                // if ((0 != (t % num_iterations_reset)) &&
                //     !recompute_everything && !factor_is_active[j]) {
                //     ++num_inactive_factors;
                //     continue;
                // }
                
                Factor *factor = factors[j];
                int factor_degree = factor->Degree();
                
                //vector<double> *cached_log_potentials = factor->GetMutableCachedVariableLogPotentials();
                //cached_log_potentials->resize(factor_degree);
                for (int i = 0; i < factor_degree; ++i) {
                    int m = factor->GetLinkId(i);
                    BinaryVariable* variable = factor->GetVariable(i);
                    int variable_degree = variable->Degree();
                    double val = variable->GetLogPotential() / static_cast<double>(variable->Degree()) + 2.0 * lambdas[m];
                    //(*cached_log_potentials)[i] = val;
                }
                //factor->ComputeCachedAdditionalLogPotentials(1.0);

                // Compute the MAP and update the dual objective.
                // Sub-problem 
                double val;
                //factor->SolveMAP(&val);
                double delta = 0.0; //- \sum lambdas
                for (int i = 0; i < factor_degree; ++i) {
                    int m = factor->GetLinkId(i);
                    delta -= lambdas[m];
                }
                //
                // Update global dual objective
                dual_obj += val + delta - dual_obj_factors[j];
                // Update local dual objective
                dual_obj_factors[j] = val + delta;

                //const vector<double> &variable_posteriors = factor->GetCachedVariablePosteriors();
                for (int i = 0; i < factor_degree; ++i) {
                    int m = factor->GetLinkId(i);
                    BinaryVariable* variable = factor->GetVariable(i);
                    int k = variable->GetId();
                    //maps_sum[k] += variable_posteriors[i] - maps[m];
                   // maps[m] = variable_posteriors[i];
                }

                //const vector<double> &factor_additional_posteriors = factor->GetCachedAdditionalPosteriors();
                //int offset = additional_factor_offsets[j];
                //for (int i = 0; i < factor_additional_posteriors.size(); ++i) {
                    //(*additional_posteriors)[offset] = factor_additional_posteriors[i];
                    //++offset;
                //}

            }

            // Optimize over maps_av and update Lagrange multipliers.
            double primal_residual = 0.0;
            for (int i = 0; i < variables.size(); ++i) {
                BinaryVariable *variable = variables[i];
                int variable_degree = variable->Degree();
                // maps_av: average MAP 你 
                //if (!variable_is_active[i]) {
                    if (variable_degree == 0) {
                        maps_av[i] = (variable->GetLogPotential() > 0)? 1.0 : 0.0;
                    }
                    // Make sure dual_residual = 0 and maps_av_[i] does not change.
                 //   continue;
               // }
                
                if (variable_degree == 0) {
                    maps_av[i] = (variable->GetLogPotential() > 0)? 1.0 : 0.0;
                } else {
                  //  maps_av[i] = maps_sum[i] / static_cast<double>(variable_degree);
                }
                for (int j = 0; j < variable_degree; ++j) {
                    int m = variable->GetLinkId(j);
                    Factor* factor = variable->GetFactor(j);
                    int k = factor->GetId();
                    // \nu_i^a - \nu_i
                    double diff_penalty = maps[m] - maps_av[i];
                  //  int l = indVinF[m];
                  //  vector<double> *cached_log_potentials = factor->GetMutableCachedVariableLogPotentials();
                 //   (*cached_log_potentials)[l] -= 2.0 * eta * diff_penalty;

                    // Update lambdas
                    lambdas[m] -= eta * diff_penalty;
           //         factor_is_active[k] = true;
                    primal_residual += diff_penalty * diff_penalty;
                }
            }

            primal_residual = sqrt(primal_residual / lambdas.size());


            // Compute relaxed primal objective.
            double primal_rel_obj = -1e100;
            //if (compute_primal_rel) {
                primal_rel_obj = 0.0;
                for (int i = 0; i < variables.size(); ++i) {
                    primal_rel_obj += maps_av[i] * variables[i]->GetLogPotential();
                }
                //for (int i = 0; i < additional_log_potentials.size(); ++i) {
                 //   primal_rel_obj += (*additional_posteriors)[i] * additional_log_potentials[i];
                //}
            //}

            if (dual_obj_best > dual_obj) {
                dual_obj_best = dual_obj;
                for (int i = 0; i < variables.size(); ++i) {
                    (*posteriors)[i] = maps_av[i];
                }
                if (dual_obj_best < lower_bound) {
                    reached_lower_bound = true;
                    break;
                }
            }

           // if (primal_rel_obj_best < primal_rel_obj) {
           //     primal_rel_obj_best = primal_rel_obj;
            //}

            if (primal_residual < residual_threshold) {
                for (int i = 0; i < variables.size(); ++i) {
                    (*posteriors)[i] = maps_av[i];
                }
                if (primal_residual < residual_threshold_final) {
                    optimal = true;
                    break;
                }
            }

            recompute_everything = false;
        }
        bool fractional = false;
        *value = 0.0;
        for (int i = 0; i < variables.size(); ++i) {
            //if (!NEARLY_BINARY((*posteriors)[i]. 1e-12)) fractional = true;
            *value += (*posteriors)[i] * variables[i]->GetLogPotential();
        }
        //for (int i = 0; i < additional_log_potentials.size(); ++i) {
          //  *value += (*additional_posteriors)[i] * additional_log_potentials[i];
      //  }
        
        *upper_bound = dual_obj_best;

        if (optimal) {
            if (!fractional)
            {
                /* code */
            }
        }       
    }
}