#include "RcppArmadillo.h"

// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

// [[Rcpp::export]]
Rcpp::List fselect(const mat & x, const colvec &y, int nvmax, int ncov) {
  
  int n_obs = x.n_rows;
  mat design_mat = join_cols(ones<mat>(1, n_obs), x);
  int nstep = (nvmax > ncov)? ncov:nvmax; // decide how many variables to select in the end
  
  colvec residuals = y - mean(y);
  // start from the model with only intercepts, calculate total sum of squares
  double TSS = as_scalar(residuals.t() * residuals); 
  double minSSE = TSS; // prepare this variable for the loops later
  
  // initialize the vector that stores the selected covariates
  Row<int> subset = Row<int>(nstep, fill::zeros);
  // store the values from four criterions after each round of selection
  Row<double> Cp = Row<double>(nstep, fill::zeros);
  Row<double> AIC = Row<double>(nstep, fill::zeros);
  Row<double> BIC = Row<double>(nstep, fill::zeros);
  Row<double> adjR2 = Row<double>(nstep, fill::zeros);
  // matrix that stores the coefficients after each round of selection
  mat coefs = mat(nstep,nstep, fill::zeros);
  
  // initialize the pool of covariates to choose from
  Row<int> pool = Row<int>(ncov, fill::zeros);
  for (int i=0; i < ncov; i++){
    pool.col(i) = i + 1;
  }
  
  for (int i=0; i<nstep; i++){
    
  }
  
  return Rcpp::List::create(Rcpp::Named("covariates")=x,
                            Rcpp::Named("inner")=y);
}
