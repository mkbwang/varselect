#include "RcppArmadillo.h"

// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
Rcpp::List bselect(const arma::mat & x, const arma::colvec &y, int nvmax, int ncov) {
  return Rcpp::List::create(Rcpp::Named("covariates")=x,
                            Rcpp::Named("inner")=y);
}
