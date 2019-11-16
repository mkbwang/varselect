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

double get_sigmahat2(int n_obs, const arma::mat &x, const arma::colvec &y){
  arma::mat total_design_mat = arma::join_horiz(arma::ones<arma::mat>(n_obs, 1), x);
  arma::mat full_coef = arma::solve(total_design_mat.t() * total_design_mat, 
                        total_design_mat.t() * y);
  arma::colvec residuals = y - total_design_mat * full_coef;
  return arma::as_scalar(residuals.t() * residuals)/(n_obs - 1);
}

// note: n_q doesn't include the intercept
double get_Cp(int n_obs, double sse, int n_q, double sigmahat2){
  return 1/n_obs * (sse + 2 * n_q * sigmahat2);
}

double get_AIC(int n_obs, double sse, int n_q, double sigmahat2){
  return 1/n_obs/sigmahat2 * (sse + 2 * n_q * sigmahat2);
}

double get_BIC(int n_obs, double sse, int n_q, double sigmahat2){
  return 1/n_obs/sigmahat2 * (sse + log(n_obs) * n_q * sigmahat2);
}

double get_adjR2(int n_obs, double sse, int n_q, double tss){
  return 1 - sse * (n_obs - 1) / (tss * (n_obs - n_q - 1));
}


#endif
