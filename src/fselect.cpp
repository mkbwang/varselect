#include "RcppArmadillo.h"
#include "utils.h"

// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

// [[Rcpp::export]]
Rcpp::List fselect(const arma::mat &x, const arma::colvec &y, int nvmax, int ncov) {
  
  int n_obs = x.n_rows;
  int nstep = (nvmax > ncov)? ncov:nvmax; // decide how many variables to select in the end
  // start from the model with only intercepts, calculate total sum of squares

  double TSS = get_tss(y); 
  double minSSE = TSS; // prepare this variable for the loops later
  
  // get estimated standard error from the full model, used for calculating different criteria
  double sigma_hat2 = get_sigmahat2(n_obs, x, y, ncov); 

  // initialize the vector that stores the selected covariates
  arma::Row<int> subset = arma::Row<int>(nstep, arma::fill::zeros);
  
  // the design matrix to be expanded with every round of selection
  arma::mat design_mat = arma::ones<arma::mat>(n_obs, 1);
  
  // store the values from four criteria after each round of selection
  arma::Row<double> Cp = arma::Row<double>(nstep, arma::fill::zeros);
  arma::Row<double> AIC = arma::Row<double>(nstep, arma::fill::zeros);
  arma::Row<double> BIC = arma::Row<double>(nstep, arma::fill::zeros);
  arma::Row<double> adjR2 = arma::Row<double>(nstep, arma::fill::zeros);
  
  // matrix that stores the coefficients after each round of selection
  // notice the plus one(8 covariates lead to 9 parameters, including intercept)
  arma::mat coefs = arma::mat(nstep+1, nstep, arma::fill::zeros);
  
  // initialize the pool of covariates to choose from
  // the ids are based on syntax of R, which starts indexing from one
  arma::Row<int> pool = arma::Row<int>(ncov, arma::fill::zeros);
  for (int i=0; i < ncov; i++){
    pool.col(i) = i + 1;
  }
  
  for (int i=0; i<nstep; i++){
    // start a new round of search
    int newcov_pos = 0;
    arma::colvec new_beta;
    minSSE = TSS;
    for (int j=0; j<ncov-i; j++){
      // searching for the new covariate to be added to the model
      int candidate = arma::as_scalar(pool.col(j));
      arma::mat cand_designmat = arma::join_horiz(design_mat, x.col(candidate - 1)); // notice the minus 1
      arma::colvec beta = arma::solve(cand_designmat.t() * cand_designmat, cand_designmat.t() * y);
      arma::colvec residuals = y - cand_designmat * beta;
      double sse = arma::as_scalar(residuals.t() * residuals);
      if (sse <= minSSE){
        // update when a new minimum SSE is found
        newcov_pos = j;
        minSSE = sse;

        new_beta = beta;// may have bugs if it is assigned by reference
      }
    }
    int new_cov = arma::as_scalar(pool.col(newcov_pos)); // the new covariate

    design_mat = arma::join_horiz(design_mat, x.col(new_cov - 1)); // expand the design matrix
    subset.col(i) = new_cov; // add to the subset vector
  
    coefs.col(i).head(i+2) = new_beta; // the new coefficients with the added covariate
  
    Cp.col(i) = get_Cp(n_obs, minSSE, i+1, sigma_hat2); // new Cp
    AIC.col(i) = get_AIC(n_obs, minSSE, i+1, sigma_hat2); // new AIC
    BIC.col(i) = get_BIC(n_obs, minSSE, i+1, sigma_hat2); // new BIC
    adjR2.col(i) = get_adjR2(n_obs, minSSE, i+1, TSS); // new adjR2
    pool.shed_col(newcov_pos); // remove the newly added covariate from the pool
  }
  
  return Rcpp::List::create(Rcpp::Named("subset")=subset,
                            Rcpp::Named("coefs")=coefs,
                            Rcpp::Named("Cp")=Cp,
                            Rcpp::Named("AIC")=AIC,
                            Rcpp::Named("BIC")=BIC,
                            Rcpp::Named("adjR2")=adjR2);
}
