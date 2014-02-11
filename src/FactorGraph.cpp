#include <iostream>
#include "FactorGraph.h"

namespace MRF_DD {

    void FactorGraph::GenFeature() {
        num_feature = 0;
        
        // Local features.
        num_feature += (4 * feature_keys.size());
        for (int i = 0; i < feature_keys.size(); i++) {
            feature_to_fid[i] = 4 * i;
        }
        
        // Factor features.
        for (int i = 0; i < factor_type.size(); i++) {
            // Generate features for AtMostOne factor.
            if (factor_type[i] == FactorTypes::FACTOR_ATMOSTONE) {
                factor_to_fid[i] = num_feature;
                num_feature += 1;
            }
            
            // Generate features for Dense factor.
            if (factor_type[i] == FactorTypes::FACTOR_DENSE) {
                factor_to_fid[i] = num_feature;
                num_feature += pow(2, factor_degree[i]);
            }
        }
    }
    
    void FactorGraph::Evaluate() {
        int tp = 0, fp = 0, tn = 0, fn = 0;
        int tp_best = 0, fp_best = 0, tn_best = 0, fn_best = 0;
        for (int i = 0; i < variables.size(); i++) {
            auto var = variables[i];

            if (var->GetType() == VariableTypes::VAR_TEST) {
                int label = var->GetLabel();
                int curt_vote[2];
                int best_vote[2];
                
                for (int j = 0; j < var->Degree(); j++) {
                    int lid = var->GetLinkId(j);
                    if (map_x[lid] > 0) {
                        curt_vote[1] += 1;
                    } else {
                        curt_vote[0] += 1;
                    }
                    if (best_x[lid]) {
                        best_vote[1] += 1;
                    } else {
                        best_vote[0] += 1;
                    }
                }
                int best_result, curt_result = -1;
                if (curt_vote[1] > curt_vote[0]) {
                    curt_result = 1;
                }
                if (best_vote[1] > best_vote[0]) {
                    best_result = 1;
                }
                
                if (label > 0) { // Positive instance.
                    if (curt_result > 0) {
                        tp += 1;
                    } else {
                        fn += 1;
                    }
                    if (best_result > 0) {
                        tp_best += 1;
                    } else {
                        fn_best += 1;
                    }
                } else { // Negative instance.
                    if (curt_result > 0) {
                        fp += 1;
                    } else {
                        tn += 1;
                    }
                    if (best_result > 0) {
                        fp_best += 1;
                    } else {
                        tn_best += 1;
                    }
                }
            }
        }
        
        // Calculate evaluation measures.
        double precision = double(tp) / (tp + fp);
        double recall = double(tp) / (tp + fn);
        double f1 = 2 * precision * recall / (precision + recall);
        
        cout << "Performance:" << endl;
        cout << "Prec: " << precision << endl;
        cout << "Recall " << recall << endl;
        cout << "F1 " << f1 << endl;
        
        cout << "" << endl;
        
        precision = double(tp_best) / (tp_best + fp_best);
        recall = double(tp_best) / (tp_best + fn_best);
        f1 = 2 * precision * recall / (precision + recall);
        
        cout << "Best performance:" << endl;
        cout << "Prec: " << precision << endl;
        cout << "Recall " << recall << endl;
        cout << "F1 " << f1 << endl;
        
        cout << "------------------------" << endl;
        
        
    }
    

    int FactorGraph::Train() {
        cout << "Start training" << endl;
        // Gradient.
        vector<double> dw;
        dw.resize(num_feature, 0.0);
        
        // Current Iteration.
        int t;
        
        // Dual objective function, max L(\lambda)
        double dual_obj = 0.0;
        double dual_obj_prev = 1e100;
        double dual_obj_best = 1e100;
        // Stores each factor contribution to the dual objective
        vector<double> dual_obj_factors(factors.size(), 0.0);
        
        // Primal objective function, min E(x)
        double primal_obj = 0.0;
        double primal_obj_best = -1e100;
        
        int num_interations_compute_dual = 50;
        
        // Compute extra score for variables that not connected to any factor
        double extra_score = 0.0;
        for (int i = 0; i < variables.size(); i++) {
            auto var = variables[i];
            int var_degree = var->Degree();
            double potential = var->GetPotential();
            if (var_degree == 0 && potential > 0) {
                extra_score += potential;
            }
        }
        dual_obj = extra_score;
        
        // Lagrangian multipliers.
        lambda.clear();
        lambda.resize(num_link, 0.0);
        
        // MAP assignment
        map_x.clear();
        map_x.resize(num_link, 0.0);
        average_x.clear();
        average_x.resize(variables.size(), 0.5);
        

        // Main loop.
        for (t = 0; t < max_iteration; ++t) {
            cout << "Iteration" << endl;
            // Set stepsize.
            double eta = init_eta / sqrt(static_cast<double>(t+1));
            
            // Compute dual value.
            for (int j = 0; j < factors.size(); ++j) {
                Factor *factor = factors[j];
                int factor_degree = factor->Degree();
                
                // Calculate addition potential.
                vector<double> additional_potential(factor_degree, 0.0);
                for (int i = 0; i < factor_degree; i++) {
                    auto var = factor->GetVariable(i);
                    int lid = factor->GetLinkId(i);
                    double dis_sim = 0.0;
                    if (var->GetLabel() != map_x[lid]) {
                        dis_sim += 1;
                    }
                    additional_potential[i] = (lambda[lid] - dis_sim);
                }
                
                // Solve the slave problem.
                double objective = factor->SolveMAP(additional_potential);
                
                // Calculate dw.
                for (int i = 0; i < factor_degree; i++) {
                    auto var = factor->variables[i];
                    for (int k = 0; k < feature_keys.size(); k++) {
                        int offset = 0;
                        if (var->GetLabel() > 0) {
                            offset = 1;
                        } else {
                            offset = 0;
                        }
                        dw[k + offset] += 1;
                        
                        if (var->GetValue() > 0) {
                            offset = 1;
                        } else {
                            offset = 0;
                        }
                        dw[k + offset] -= 1;
                    }
                }
                int feature_offset_labeled = factor->GetFeatureOffset(factor->values);
                int feature_offset_map = factor->GetFeatureOffset(factor->labels);
                dw[feature_offset_labeled] += 1;
                dw[feature_offset_map] -= 1;
                
                // Update dual objective.
                double delta = 0.0;
                for (int i = 0; i < factor_degree; i++) {
                    auto var = factor->variables[i];
                    
                    int lid = factor->GetLinkId(i);
                    int vid = var->GetId();
                    delta -= lambda[lid];
                    
                    // Update x and \sum x
                    sum_x[vid] += factor->values[i] - map_x[lid];
                    map_x[lid] = factor->values[i];
                }
                dual_obj += (objective + delta - dual_obj_factors[j]);
                dual_obj_factors[j] = objective + delta;
            }
            
            // Update w.
            for (int i = 0; i < feature_keys.size(); i++) {
                dw[i] /= (double)variables.size();
            }
            for (int i = 0; i < num_feature; i++) {
                w[i] += eta * (w[i] + dw[i]);
            }
            
            // Optimize and update Lagrange multipliers.
            for (int i = 0; i < variables.size(); i++) {
                auto var = variables[i];
                int var_degree = var->Degree();
                
                // Compute \sum \frac{1}{|F_v|} x_v
                if (var_degree == 0) {
                    average_x[i] = (var->GetPotential() > 0) ? 1.0 : 0.0;
                } else {
                    average_x[i] = sum_x[i] / static_cast<double>(var_degree);
                }
                
                // Calculate difference penalty.
                for (int j = 0; j < var_degree; j++) {
                    int lid = var->GetLinkId(j);
                    auto factor = var->GetFactor(j);
                    int fid = factor->GetId();
                    double diff_penalty = map_x[lid] - average_x[i];
                    
                    // Update lambda.
                    lambda[lid] += eta * diff_penalty;
                }
            }
            
            // Update dual objective and MAP assignment
            if (dual_obj_best < dual_obj) {
                dual_obj_best = dual_obj;
                best_x = map_x;
            }
            
            Evaluate();
            
        }
    }
    
    int FactorGraph::Inference() {
        cout << "Start inference" << endl;
        timeval start, end;
        gettimeofday(&start, NULL);

        // Current Iteration.
        int t;
        // Dual objective function, max L(\lambda)
        double dual_obj = 0.0;
        double dual_obj_best = 1e100;
        // Stores each factor contribution to the dual objective
        vector<double> dual_obj_factors(factors.size(), 0.0);
        // Primal objective function, min E(x)
        double primal_obj = 0.0;
        double primal_obj_best = -1e100;
        
        int num_interations_compute_dual = 50;

        // Compute extra score for variables that not connected to any factor
        double extra_score = 0.0;
        for (int i = 0; i < variables.size(); i++) {
            auto var = variables[i];
            int var_degree = var->Degree();
            double potential = var->GetPotential();
            if (var_degree == 0 && potential > 0) {
                extra_score += potential;
            }
        }
        dual_obj = extra_score;
        
        // Dual variables.
        lambda.clear();
        lambda.resize(num_link, 0.0);
        
        // MAP assignment
        map_x.clear();
        map_x.resize(num_link, 0.0);
        average_x.clear();
        average_x.resize(variables.size(), 0.5);

        // Main loop.
        for (t = 0; t < max_iteration; ++t) {
            cout << "Iteration" << t;
            // Set stepsize.
            double eta = init_eta / sqrt(static_cast<double>(t+1));
            
            // Compute dual value.
            for (int j = 0; j < factors.size(); ++j) {
                Factor *factor = factors[j];
                int factor_degree = factor->Degree();
                
                // Calculate addition potential.
                vector<double> additional_potential(factor_degree, 0.0);
                for (int i = 0; i < factor_degree; i++) {
                    int lid = factor->GetLinkId(i);
                    additional_potential[i] = lambda[lid];
                }
                // Solve the slave problem and update dual objective
                double objective = factor->SolveMAP(additional_potential);

                double delta = 0.0;
                for (int i = 0; i < factor_degree; i++) {
                    auto var = factor->variables[i];
                    
                    int lid = factor->GetLinkId(i);
                    int vid = var->GetId();
                    delta -= lambda[lid];
                    
                    // Update x and \sum x
                    sum_x[vid] += factor->values[i] - map_x[lid];
                    map_x[lid] = factor->values[i];
                }
                dual_obj += (objective + delta - dual_obj_factors[j]);
                dual_obj_factors[j] = objective + delta;
            }

            // Optimize and update Lagrange multipliers.
            for (int i = 0; i < variables.size(); i++) {
                auto var = variables[i];
                int var_degree = var->Degree();
                
                // Compute \sum \frac{1}{|F_v|} x_v
                if (var_degree == 0) {
                    average_x[i] = (var->GetPotential() > 0) ? 1.0 : 0.0;
                } else {
                    average_x[i] = sum_x[i] / static_cast<double>(var_degree);
                }
                
                // Calculate difference penalty.
                for (int j = 0; j < var_degree; j++) {
                    int lid = var->GetLinkId(j);
                    auto factor = var->GetFactor(j);
                    int fid = factor->GetId();
                    double diff_penalty = map_x[lid] - average_x[i];
                    
                    // Update lambda.
                    lambda[lid] += eta * diff_penalty;
                }
            }
            
            // Update dual objective and MAP assignment
            if (dual_obj_best < dual_obj) {
                dual_obj_best = dual_obj;
                best_x = map_x;
            }
            
            Evaluate();

        }       
    }
}