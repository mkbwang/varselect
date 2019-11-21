#ifndef UTILS_H
#define UTILS_H

#include "RcppArmadillo.h"
#include <cmath>

// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

double get_tss(const arma::colvec &y){
  arma::colvec residuals = y - arma::mean(y);
  return arma::as_scalar(residuals.t() * residuals);
}

double get_sigmahat2(int n_obs, const arma::mat &x, const arma::colvec &y, int ncov){
  arma::mat total_design_mat = arma::join_horiz(arma::ones<arma::mat>(n_obs, 1), x);
  arma::mat full_coef = arma::solve(total_design_mat.t() * total_design_mat, 
                        total_design_mat.t() * y);
  arma::colvec residuals = y - total_design_mat * full_coef;
  return arma::as_scalar(residuals.t() * residuals)/(n_obs - ncov - 1);
}

// note: n_q doesn't include the intercept
double get_Cp(int n_obs, double sse, int n_q, double sigmahat2){
  return sse / sigmahat2 - n_obs + 2 * (n_q + 1);
}

double get_AIC(int n_obs, double sse, int n_q, double sigmahat2){
  return n_obs * log(sse / n_obs / sigmahat2) + 2 * n_q;
}

double get_BIC(int n_obs, double sse, int n_q, double sigmahat2){
  return n_q * log(n_obs) + n_obs * log(sse / n_obs / sigmahat2);
}

double get_adjR2(int n_obs, double sse, int n_q, double tss){
  return 1 - sse * (n_obs - 1) / (tss * (n_obs - n_q - 1));
}


#endif
